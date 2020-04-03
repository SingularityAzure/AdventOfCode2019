Array<i64> input;

void GetInput(String filename) {
    Array<char> inputText = FileContents(filename);
    Array<Range<char>> instructions = SeparateByValues(&inputText, {',', '\n'});
    input.Reserve(instructions.size);
    for (Range<char> &range : instructions) {
        if (range.size == 0) continue;
        input.Append(StringToI64(range));
    }
}

struct Program {
    Array<i64> opcodes;
    i32 cursor;
    i32 relativeBase;
    Program() { Reset(); }
    void Reset() {
        opcodes = input;
        cursor = 0;
        relativeBase = 0;
    }
};

i32 OpCount(i64 op) {
    switch (op) {
        case 1:
        case 2:
        case 7:
        case 8:
            return 4;
        case 3:
        case 4:
        case 9:
            return 2;
        case 5:
        case 6:
            return 3;
        default:
            return -1;
    }
}

struct Ret {
    i64 output;
    i64 *input = nullptr;
    bool halt;
};

i64 HandleMode(i32 mode, i64 arg, Program &program) {
    i64 out;
    Array<i64> &opcodes = program.opcodes;
    switch (mode) {
        case 0: // pointer
            if (arg >= INT32_MAX) cout << "Out of bounds!!!" << std::endl;
            if (arg >= opcodes.size) {
                opcodes.Resize(arg+1, 0);
            }
            out = opcodes[arg];
            break;
        case 1: // immediate
            out = arg;
            break;
        case 2: { // relative pointer
            i32 ptr = arg + program.relativeBase;
            if (ptr >= INT32_MAX) cout << "Out of bounds!!!" << std::endl;
            if (ptr >= opcodes.size) {
                opcodes.Resize(ptr+1, 0);
            }
            out = opcodes[ptr];
        } break;
    }
    return out;
}

i64& HandleMutableMode(i32 mode, i64 arg, Program &program) {
    i64 *out;
    Array<i64> &opcodes = program.opcodes;
    switch (mode) {
        case 0: // pointer
            if (arg >= INT32_MAX) cout << "Out of bounds!!!" << std::endl;
            if (arg >= opcodes.size) {
                opcodes.Resize(arg+1, 0);
            }
            out = &opcodes[arg];
            break;
        case 1: // immediate
            cout << "Argument must be mutable!!!" << std::endl;
            break;
        case 2: { // relative pointer
            i32 ptr = arg + program.relativeBase;
            if (ptr >= INT32_MAX) cout << "Out of bounds!!!" << std::endl;
            if (ptr >= opcodes.size) {
                opcodes.Resize(ptr+1, 0);
            }
            out = &opcodes[ptr];
        } break;
    }
    return *out;
}

Ret RunProgram(Program &program) {
    Ret ret;
    ret.halt = true;
    Array<i64> &opcodes = program.opcodes;
    for (i32 &i = program.cursor; opcodes[i] != 99;) {
        i32 mode[3];
        i64 args[3];
        i64 op = opcodes[i] % 100;
        {
            i32 opcode = opcodes[i] / 100;
            for (i32 j = 0; j < 3; j++) {
                args[j] = opcodes[i+j+1];
                mode[j] = opcode % 10;
                opcode /= 10;
            }
        }

        i64 lhs, rhs;
        if (op < 3 || op > 4) {
            lhs = HandleMode(mode[0], args[0], program);
            rhs = HandleMode(mode[1], args[1], program);
        }
        switch (op) {
            case 1: {
                HandleMutableMode(mode[2], args[2], program) = lhs + rhs;
            } break;
            case 2: {
                HandleMutableMode(mode[2], args[2], program) = lhs * rhs;
            } break;
            case 3: {
                ret.input = &HandleMutableMode(mode[0], args[0], program);
                i += OpCount(3);
                ret.halt = false;
                return ret;
            } break;
            case 4: {
                i64 out = HandleMode(mode[0], args[0], program);
                i += OpCount(4);
                ret.output = out;
                ret.halt = false;
                return ret;
            } break;
            case 5: {
                if (lhs) {
                    i = rhs;
                    continue;
                }
            } break;
            case 6: {
                if (!lhs) {
                    i = rhs;
                    continue;
                }
            } break;
            case 7: {
                HandleMutableMode(mode[2], args[2], program) = lhs < rhs;
            } break;
            case 8: {
                HandleMutableMode(mode[2], args[2], program) = lhs == rhs;
            } break;
            case 9: {
                program.relativeBase += HandleMode(mode[0], args[0], program);
            } break;
            default: {
                cout << "Error: Unexpected opcode " << ToString(op) << std::endl;
                return ret;
            } break;
        }
        i += OpCount(op);
    }
    return ret;
}
