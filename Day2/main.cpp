#include <AzCore/basictypes.hpp>
#include <AzCore/IO/LogStream.hpp>

AzCore::io::LogStream cout("day2.log");

const i64 program[] = {1,0,0,3,1,1,2,3,1,3,4,3,1,5,0,3,2,10,1,19,1,19,9,23,1,23,13,27,1,10,27,31,2,31,13,35,1,10,35,39,2,9,39,43,2,43,9,47,1,6,47,51,1,10,51,55,2,55,13,59,1,59,10,63,2,63,13,67,2,67,9,71,1,6,71,75,2,75,9,79,1,79,5,83,2,83,13,87,1,9,87,91,1,13,91,95,1,2,95,99,1,99,6,0,99,2,14,0,0};

constexpr size_t numOps = sizeof(program) / sizeof(u64);

i64 RunProgram(i8 params[2]) {
    i64 opcodes[numOps];
    memcpy(opcodes, program, sizeof(program));
    opcodes[1] = params[0];
    opcodes[2] = params[1];

    for (i32 i = 0; opcodes[i] != 99; i += 4) {
        i64 &op = opcodes[i];
        i64 &arg1 = opcodes[i+1];
        i64 &arg2 = opcodes[i+2];
        i64 &ret = opcodes[i+3];
        if (arg1 >= numOps || arg2 >= numOps || ret >= numOps) {
            cout << "Error: Invalid pointer " << arg1 << ", " << arg2 << ", " << ret << std::endl;
            return -1;
        }
        if (op == 1) {
            opcodes[ret] = opcodes[arg1] + opcodes[arg2];
        } else if (op == 2) {
            opcodes[ret] = opcodes[arg1] * opcodes[arg2];
        } else {
            cout << "Error: Unexpected opcode " << op << std::endl;
            return -1;
        }
    }
    return opcodes[0];
}

int main() {
    cout << "Day 2:" << std::endl;
    i8 params[2] = {12, 2};
    i64 out = RunProgram(params);
    if (out == -1) {
        return 1;
    }
    cout << "Part 1 output " << out << std::endl;
    params[0] = 0; params[1] = 0;
    while (out != 19690720) {
        if (params[0] == 100) {
            params[1]++;
            params[0] = 0;
        }
        if (params[1] == 100) {
            cout << "Error: Didn't find a solution." << std::endl;
            return 1;
        }
        out = RunProgram(params);
        if (out == -1) {
            return 1;
        }
        params[0]++;
    }
    cout << "Part 2 output " << (i32)params[0]-1 << (i32)params[1] << std::endl;
    return 0;
}