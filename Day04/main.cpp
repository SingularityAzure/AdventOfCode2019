#include <AzCore/IO/LogStream.hpp>
#include <AzCore/Time.hpp>

using namespace AzCore;

io::LogStream cout("day4.log");

i32 minRange = 165432;
i32 maxRange = 707912;

bool Acceptable1(i32 in) {
    i32 digits[6];
    for (i32 i = 0; i < 6; i++) {
        digits[5-i] = in % 10;
        in /= 10;
    }
    i32 min = -1;
    bool dual = false;
    for (i32 i = 0; i < 6; i++) {
        if (digits[i] < min) return false;
        if (digits[i] == min) dual = true;
        if (digits[i] > min) min = digits[i];
    }
    return dual;
}

bool Acceptable2(i32 in) {
    i32 digits[6];
    for (i32 i = 0; i < 6; i++) {
        digits[5-i] = in % 10;
        in /= 10;
    }
    i32 min = -1;
    i32 counts[10] = {0};
    for (i32 i = 0; i < 6; i++) {
            counts[digits[i]]++;
        if (digits[i] < min) return false;
        if (digits[i] > min) min = digits[i];
    }
    for (i32 i = 0; i < 10; i++) {
        if (counts[i] == 2)
            return true;
    }
    return false;
}

int main() {
    cout << "Day 4:" << std::endl;
    ClockTime start = Clock::now();

    i32 num = 0;
    for (i32 i = minRange; i < maxRange; i++) {
        if (Acceptable1(i))
            num++;
    }
    cout << "(part 1) Number of acceptable codes between " << minRange << " and " << maxRange << " is " << num << std::endl;
    num = 0;
    for (i32 i = minRange; i < maxRange; i++) {
        if (Acceptable2(i))
            num++;
    }
    cout << "(part 2) Number of acceptable codes between " << minRange << " and " << maxRange << " is " << num << std::endl;

    cout << "Total time spent: " << FormatTime(Clock::now() - start) << std::endl;
    return 0;
}