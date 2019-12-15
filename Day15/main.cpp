#include <AzCore/io.hpp>
#include <AzCore/memory.hpp>
#include <AzCore/Time.hpp>
#include <AzCore/math.hpp>
#include <AzCore/keycodes.hpp>

using namespace AzCore;

io::LogStream cout("day15.log");

Array<i64> input;

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

enum Tile {
    TILE_UNKNOWN=0,
    TILE_EMPTY=1,
    TILE_START=2,
    TILE_WALL=3,
    TILE_DROID=4,
    TILE_OXYGEN_SYSTEM=5
};

const char tileChar[] = " .x#DS";

const i32 width = 64, height = 48;

vec2i start(width/2, height/2);
vec2i oxygenSystem(-1, 0);

Array<u8> board(width*height);

u8& Board(vec2i pos) {
    if (pos.x >= width) {
        cout << "Out of width." << std::endl;
    }
    if (pos.y >= height) {
        cout << "Out of height." << std::endl;
    }
    return board[pos.y*width + pos.x];
}

void DrawBoard() {
    for (i32 y = 0; y < height; y++) {
        for (i32 x = 0; x < width; x++) {
            cout << tileChar[board[y*width+x]];
        }
        cout << "\n";
    }
    cout << std::endl;
}

enum Move {
    MOVE_UP=1,
    MOVE_DOWN=2,
    MOVE_LEFT=3,
    MOVE_RIGHT=4
};

vec2i moveDirs[] = {
    { 0, 0},
    { 0,-1},
    { 0, 1},
    {-1, 0},
    { 1, 0}
};

enum Status {
    STATUS_HIT_WALL=0,
    STATUS_MOVED=1,
    STATUS_MOVED_TO_OXYGEN_SYSTEM=2
};

struct Droid {
    Program program;
    vec2i pos;
    void Reset() {
        program.Reset();
        pos = vec2i(width, height)/2;
    }
    void Run() {
        vec2i moveTo = pos;
        Array<i64> queuedInputs;
        while (true) {
            Ret ret;
            ret = RunProgram(program);
            if (ret.halt) {
                cout << "Unexpected program halt!" << std::endl;
                return;
            }
            bool draw = false;
            if (ret.input != nullptr) {
                bool tryagain = true;
                while (tryagain) {
                    tryagain = false;
                    if (queuedInputs.size > 0) {
                        *ret.input = queuedInputs[0];
                        queuedInputs.Erase(0);
                        moveTo = pos + moveDirs[*ret.input];
                        break;
                    }
                    cout << "Enter a direction(u, d, l, r): " << std::flush;
                    char c;
                    std::cin >> c;
                    switch (c) {
                        case 'u':
                            *ret.input = MOVE_UP;
                            queuedInputs.Append({MOVE_LEFT,MOVE_RIGHT,MOVE_RIGHT,MOVE_LEFT,MOVE_UP,MOVE_DOWN});
                            break;
                        case 'd':
                            *ret.input = MOVE_DOWN;
                            queuedInputs.Append({MOVE_LEFT,MOVE_RIGHT,MOVE_RIGHT,MOVE_LEFT,MOVE_DOWN,MOVE_UP});
                            break;
                        case 'l':
                            *ret.input = MOVE_LEFT;
                            queuedInputs.Append({MOVE_UP,MOVE_DOWN,MOVE_DOWN,MOVE_UP,MOVE_LEFT,MOVE_RIGHT});
                            break;
                        case 'r':
                            *ret.input = MOVE_RIGHT;
                            queuedInputs.Append({MOVE_UP,MOVE_DOWN,MOVE_DOWN,MOVE_UP,MOVE_RIGHT,MOVE_LEFT});
                            break;
                        case 'q':
                            cout << "Quitting..." << std::endl;
                            return;
                        default:
                            tryagain = true;
                            break;
                    }
                    if (tryagain) {
                        cout << "Invalid input." << std::endl;
                        continue;
                    }
                    moveTo = pos + moveDirs[*ret.input];
                    cout << c << std::endl;
                }
            } else {
                switch (ret.output) {
                    case STATUS_HIT_WALL: {
                        Board(moveTo) = TILE_WALL;
                        if (queuedInputs.size & 1) queuedInputs.Erase(0);
                    } break;
                    case STATUS_MOVED_TO_OXYGEN_SYSTEM:
                        oxygenSystem = moveTo;
                        // Fall through
                    case STATUS_MOVED: {
                        Board(moveTo) = TILE_DROID;
                        if (pos == start) {
                            Board(pos) = TILE_START;
                        } else if (pos == oxygenSystem) {
                            Board(pos) = TILE_OXYGEN_SYSTEM;
                        } else {
                            Board(pos) = TILE_EMPTY;
                        }
                        pos = moveTo;
                    } break;
                    default:
                        cout << "Unexpected output: " << ret.output << std::endl;
                        return;
                }
                if (queuedInputs.size == 0) draw = true;
            }
            if (draw) {
                DrawBoard();
            }
        }
    }
};

int main() {
    ClockTime start = Clock::now();
    cout << "Day 15:" << std::endl;

    { // Read instructions from input.txt
        Array<char> inputText = FileContents("Day15/input.txt");
        Array<Range<char>> instructions = SeparateByValues(&inputText, {','});
        input.Reserve(instructions.size);
        for (Range<char> &range : instructions) {
            input.Append(StringToI64(range));
        }
    }
    Droid droid;
    droid.Reset();
    droid.Run();

    cout << "Total time taken: " << FormatTime(Clock::now() - start) << std::endl;

    return 0;
}
