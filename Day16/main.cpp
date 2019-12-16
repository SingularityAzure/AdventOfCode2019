#include <AzCore/IO/LogStream.hpp>
#include <AzCore/memory.hpp>
#include <AzCore/math.hpp>
#include <AzCore/Time.hpp>

using namespace AzCore;

io::LogStream cout("day16.log");

Array<i32> input;

inline Array<i32> Phase(const Array<i32> &in) {
    Array<i32> out(in.size);
    for (i32 i = 0; i < out.size; i++) {
        out[i] = 0;
        for (i32 j = i; j < in.size;) {
            for (i32 k = 0; k < i+1; k++, j++) {
                if (j >= in.size) break;
                out[i] += in[j];
            }
            j += i+1;
            for (i32 k = 0; k < i+1; k++, j++) {
                if (j >= in.size) break;
                out[i] -= in[j];
            }
            j += i+1;
        }
        if (out[i] < 0) out[i] = -out[i];
        out[i] %= 10;
    }
    return out;
}

inline void PhaseSecondHalf(Array<i32> &in) {
    i32 sum = 0;
    for (i32 i = in.size-1; i >= in.size/2; i--) {
        sum += in[i];
        in[i] = sum % 10;
    }
}

int main() {
    cout << "Day 16:" << std::endl;
    ClockTime start = Clock::now();
    { // Get input digits as i32s
        Array<char> inputData = FileContents("Day16/input.txt");
        input.Reserve(inputData.size);
        for (i32 i = 0; i < inputData.size; i++) {
            if (inputData[i] >= '0' && inputData[i] <= '9') {
                input.Append(inputData[i]-'0');
            }
        }
    }
    Array<i32> part1 = input;
    for (i32 i = 0; i < 100; i++) {
        part1 = Phase(part1);
    }

    cout << "Part 1: First 8 digits after 100 phases: ";
    for (i32 i = 0; i < 8; i++) {
        cout << part1[i];
    }
    cout << std::endl;

    i32 messageOffset = 0;
    {
        i32 multiplier = 1;
        for (i32 i = 6; i >= 0; i--) {
            messageOffset += input[i]*multiplier;
            multiplier *= 10;
        }
    }
    cout << "Message offset is " << messageOffset << std::endl;
    i32 actualSize = input.size * 10000;
    cout << "Actual size of array needed for processing is " << actualSize << std::endl;
    Array<i32> part2;
    part2.Reserve(actualSize);
    for (i32 i = 0; i < 10000; i++) {
        part2.Append(input);
    }

    for (i32 i = 0; i < 100; i++) {
        PhaseSecondHalf(part2);
    }
    cout << "Part 2: Message is ";
    for (i32 i = 0; i < 8; i++) {
        cout << part2[i+messageOffset];
    }
    cout << std::endl;

    cout << "Total time taken: " << FormatTime(Clock::now() - start) << std::endl;

    return 0;
}
