#include <AzCore/io.hpp>
#include <AzCore/memory.hpp>
#include <AzCore/Time.hpp>
#include <AzCore/math.hpp>
#include <AzCore/keycodes.hpp>

using namespace AzCore;

io::LogStream cout("day13.log");

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

Ret RunProgram(Program &program, Array<i64> inputs) {
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
                if (inputs.size > 0) {
                    HandleMutableMode(mode[0], args[0], program) = inputs[0];
                    inputs.Erase(0);
                } else {
                    cout << "Please enter an input: " << std::endl;
                    i64 in;
                    std::cin >> in;
                    cout << in << std::endl;
                    HandleMutableMode(mode[0], args[0], program) = in;
                }
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
    TILE_EMPTY=0,
    TILE_WALL=1,
    TILE_BLOCK=2,
    TILE_PADDLE=3,
    TILE_BALL=4
};

const char tileChar[] = " WBPo";

const i32 width = 48, height = 20;

i64 score = 0;
Array<u8> board(width*height);

void DrawBoard() {
    cout << "Score: " << score << "\n";
    for (i32 y = 0; y < height; y++) {
        for (i32 x = 0; x < width; x++) {
            cout << tileChar[board[y*width+x]];
        }
        cout << "\n";
    }
    cout << std::endl;
}

int main() {
    ClockTime start = Clock::now();
    cout << "Day 13:" << std::endl;

    { // Read instructions from input.txt
        Array<char> inputText = FileContents("Day13/input.txt");
        Array<Range<char>> instructions = SeparateByValues(&inputText, {','});
        input.Reserve(instructions.size);
        for (Range<char> &range : instructions) {
            input.Append(StringToI64(range));
        }
    }
    Program program;
    while (true) {
        Ret ret;
        ret = RunProgram(program, {});
        i32 x = ret.output;
        if (x > width) {
            cout << "X is too far out: " << x << std::endl;
            return 1;
        }
        if (ret.halt) break;
        ret = RunProgram(program, {});
        i32 y = ret.output;
        if (y > height) {
            cout << "Y is too far out: " << y << std::endl;
            return 1;
        }
        if (ret.halt) break;
        ret = RunProgram(program, {});
        i32 tile = ret.output;
        if (tile > 4) {
            cout << "Tile out of bounds: " << tile << std::endl;
            return 1;
        }
        board[y*width+x] = tile;
        if (ret.halt) break;
    }
    DrawBoard();
    i32 numBlockTiles = 0;
    for (i32 i = 0; i < board.size; i++) {
        if (board[i] == TILE_BLOCK)
            numBlockTiles++;
    }
    cout << "Part 1: Number of block tiles on exit: " << numBlockTiles << std::endl;

    program.Reset();
    program.opcodes[0] = 2; // Our two quarters >:)
    i32 ballX, paddleX;
    // bool playing = false;
    while (true) {
        i64 input = 0;
        for (i32 i = 0; i < board.size; i++) {
            if (board[i] == TILE_BALL) {
                ballX = i % width;
            }
            if (board[i] == TILE_PADDLE) {
                paddleX = i % width;
            }
        }
        if (ballX < paddleX) {
            input = -1;
        } else if (ballX > paddleX) {
            input = 1;
        }
        Ret ret;
        ret = RunProgram(program, {input});
        i32 x = ret.output;
        if (x > width) {
            cout << "X is too far out: " << x << std::endl;
            return 1;
        }
        if (ret.halt) break;
        ret = RunProgram(program, {});
        i32 y = ret.output;
        if (y > height) {
            cout << "Y is too far out: " << y << std::endl;
            return 1;
        }
        if (ret.halt) break;
        ret = RunProgram(program, {});
        i32 tile = ret.output;
        if (x == -1 && y == 0) {
            // Score
            score = tile;
        } else if (tile > 4) {
            cout << "Tile out of bounds: " << tile << std::endl;
            return 1;
        } else {
            board[y*width+x] = tile;
        }
        // if (playing) {
        //     DrawBoard();
        //     Thread::Sleep(Milliseconds(20));
        // } else {
        //     if (tile == TILE_BALL && y == 16) {
        //         playing = true;
        //         DrawBoard();
        //     }
        // }
        if (ret.halt) break;
    }
    cout << "Part 2: Score: " << score << std::endl;

    cout << "Total time taken: " << FormatTime(Clock::now() - start) << std::endl;

    return 0;
}
