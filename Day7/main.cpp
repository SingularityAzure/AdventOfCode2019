#include <AzCore/basictypes.hpp>
#include <AzCore/IO/LogStream.hpp>
#include <AzCore/Time.hpp>

using namespace AzCore;

io::LogStream cout("day7.log");

Array<i64> program = {3,8,1001,8,10,8,105,1,0,0,21,30,51,72,81,94,175,256,337,418,99999,3,9,101,5,9,9,4,9,99,3,9,1001,9,3,9,1002,9,2,9,1001,9,2,9,1002,9,5,9,4,9,99,3,9,1002,9,4,9,101,4,9,9,102,5,9,9,101,3,9,9,4,9,99,3,9,1002,9,4,9,4,9,99,3,9,102,3,9,9,1001,9,4,9,4,9,99,3,9,1001,9,2,9,4,9,3,9,101,2,9,9,4,9,3,9,101,2,9,9,4,9,3,9,1001,9,1,9,4,9,3,9,101,1,9,9,4,9,3,9,101,1,9,9,4,9,3,9,101,1,9,9,4,9,3,9,1002,9,2,9,4,9,3,9,101,1,9,9,4,9,3,9,1002,9,2,9,4,9,99,3,9,102,2,9,9,4,9,3,9,1002,9,2,9,4,9,3,9,1001,9,1,9,4,9,3,9,102,2,9,9,4,9,3,9,1001,9,2,9,4,9,3,9,1002,9,2,9,4,9,3,9,101,1,9,9,4,9,3,9,101,1,9,9,4,9,3,9,101,2,9,9,4,9,3,9,102,2,9,9,4,9,99,3,9,102,2,9,9,4,9,3,9,102,2,9,9,4,9,3,9,1002,9,2,9,4,9,3,9,1001,9,1,9,4,9,3,9,1002,9,2,9,4,9,3,9,102,2,9,9,4,9,3,9,102,2,9,9,4,9,3,9,102,2,9,9,4,9,3,9,102,2,9,9,4,9,3,9,102,2,9,9,4,9,99,3,9,1001,9,2,9,4,9,3,9,101,1,9,9,4,9,3,9,1001,9,1,9,4,9,3,9,1001,9,1,9,4,9,3,9,102,2,9,9,4,9,3,9,101,1,9,9,4,9,3,9,1001,9,2,9,4,9,3,9,1002,9,2,9,4,9,3,9,1001,9,1,9,4,9,3,9,101,2,9,9,4,9,99,3,9,101,2,9,9,4,9,3,9,101,2,9,9,4,9,3,9,101,1,9,9,4,9,3,9,1001,9,1,9,4,9,3,9,1002,9,2,9,4,9,3,9,1001,9,1,9,4,9,3,9,1001,9,1,9,4,9,3,9,1001,9,2,9,4,9,3,9,1001,9,1,9,4,9,3,9,101,1,9,9,4,9,99};

void ErrPtr(i64 in) {
    cout << "Error: Invalid pointer " << in << std::endl;
}

i32 OpCount(i64 op) {
    switch (op) {
        case 1:
        case 2:
        case 7:
        case 8:
            return 4;
        case 3:
        case 4:
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

Ret RunProgram(Array<i64> &opcodes, i32 &i, Array<i64> inputs) {
    Ret ret;
    ret.halt = true;
    for (; opcodes[i] != 99;) {
        i32 mode[3];
        i64 args[3];
        i32 op = opcodes[i] % 100;
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
            if (!mode[0] && args[0] >= opcodes.size) {
                ErrPtr(args[0]);
                return ret;
            }
            if (!mode[1] && args[1] >= opcodes.size) {
                ErrPtr(args[1]);
                return ret;
            }
            lhs = mode[0] ? args[0] : opcodes[args[0]];
            rhs = mode[1] ? args[1] : opcodes[args[1]];
        }
        switch (op) {
            case 1: {
                opcodes[args[2]] = lhs + rhs;
            } break;
            case 2: {
                opcodes[args[2]] = lhs * rhs;
            } break;
            case 3: {
                if (inputs.size > 0) {
                    opcodes[args[0]] = inputs[0];
                    inputs.Erase(0);
                } else {
                    cout << "Please enter an input: " << std::endl;
                    i64 in;
                    std::cin >> in;
                    cout << in << std::endl;
                    opcodes[args[0]] = in;
                }
            } break;
            case 4: {
                i64 out = mode[0] ? args[0] : opcodes[args[0]];
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
                opcodes[args[2]] = lhs < rhs;
            } break;
            case 8: {
                opcodes[args[2]] = lhs == rhs;
            } break;
            default: {
                cout << "Error: Unexpected opcode " << op << std::endl;
                return ret;
            } break;
        }
        i += OpCount(op);
    }
    return ret;
}

struct Amplifier {
    Array<i64> opcodes = program;
    i32 programPos = 0;
};

struct PhaseSettings {
    i64 phase[5];
};

int main() {
    ClockTime start = Clock::now();
    cout << "Day 7:" << std::endl;
    i64 highestOutput = -1;
    PhaseSettings highestSettings = {0, 0, 0, 0, 0};

    for (i32 a = 0; a < 5; a++) {
        for (i32 b = 0; b < 5; b++) {
            if (b == a) continue;
            for (i32 c = 0; c < 5; c++) {
                if (c == a || c == b) continue;
                for (i32 d = 0; d < 5; d++) {
                    if (d == a || d == b || d == c) continue;
                    for (i32 e = 0; e < 5; e++) {
                        if (e == a || e == b || e == c || e == d) continue;
                        // What a stack!
                        Amplifier amp[5];
                        i64 outputA = RunProgram(amp[0].opcodes, amp[0].programPos, {a, 0}).output;
                        i64 outputB = RunProgram(amp[1].opcodes, amp[1].programPos, {b, outputA}).output;
                        i64 outputC = RunProgram(amp[2].opcodes, amp[2].programPos, {c, outputB}).output;
                        i64 outputD = RunProgram(amp[3].opcodes, amp[3].programPos, {d, outputC}).output;
                        i64 outputE = RunProgram(amp[4].opcodes, amp[4].programPos, {e, outputD}).output;
                        if (outputE > highestOutput) {
                            highestOutput = outputE;
                            highestSettings.phase[0] = a;
                            highestSettings.phase[1] = b;
                            highestSettings.phase[2] = c;
                            highestSettings.phase[3] = d;
                            highestSettings.phase[4] = e;
                        }
                    }
                }
            }
        }
    }

    cout << "Highest output (part 1):" << highestOutput << ", settings: ";
    for (i32 i = 0; i < 5; i++) {
        cout << highestSettings.phase[i];
    }
    cout << std::endl;

    highestOutput = -1;

    for (i32 a = 5; a <= 9; a++) {
        for (i32 b = 5; b <= 9; b++) {
            if (b == a) continue;
            for (i32 c = 5; c <= 9; c++) {
                if (c == a || c == b) continue;
                for (i32 d = 5; d <= 9; d++) {
                    if (d == a || d == b || d == c) continue;
                    for (i32 e = 5; e <= 9; e++) {
                        if (e == a || e == b || e == c || e == d) continue;
                        // Code duplication FTW
                        Amplifier amp[5];
                        i64 feedback = 0;
                        bool first = true;
                        typedef Array<i64> Ar;
                        while (true) {
                            i64 outputA = RunProgram(amp[0].opcodes, amp[0].programPos, first ? Ar({a, 0}) : Ar({feedback})).output;
                            i64 outputB = RunProgram(amp[1].opcodes, amp[1].programPos, first ? Ar({b, outputA}) : Ar({outputA})).output;
                            i64 outputC = RunProgram(amp[2].opcodes, amp[2].programPos, first ? Ar({c, outputB}) : Ar({outputB})).output;
                            i64 outputD = RunProgram(amp[3].opcodes, amp[3].programPos, first ? Ar({d, outputC}) : Ar({outputC})).output;
                            Ret outputE = RunProgram(amp[4].opcodes, amp[4].programPos, first ? Ar({e, outputD}) : Ar({outputD}));
                            first = false;
                            if (outputE.halt) {
                                break;
                            } else {
                                feedback = outputE.output;
                            }
                        }

                        if (feedback > highestOutput) {
                            highestOutput = feedback;
                            highestSettings.phase[0] = a;
                            highestSettings.phase[1] = b;
                            highestSettings.phase[2] = c;
                            highestSettings.phase[3] = d;
                            highestSettings.phase[4] = e;
                        }
                    }
                }
            }
        }
    }

    cout << "Highest output (part 2):" << highestOutput << ", settings: ";
    for (i32 i = 0; i < 5; i++) {
        cout << highestSettings.phase[i];
    }
    cout << std::endl;

    cout << "Total time taken: " << FormatTime(Clock::now() - start) << std::endl;

    return 0;
}
