#include <AzCore/math.hpp>
#include <AzCore/IO/LogStream.hpp>

i32 data[] = {
    54172,
    58469,
    92948,
    143402,
    57563,
    54532,
    68042,
    89847,
    70872,
    54069,
    107310,
    146439,
    88851,
    142869,
    71309,
    89613,
    70338,
    87708,
    95305,
    134384,
    128250,
    134991,
    91270,
    127819,
    68650,
    102556,
    129882,
    68688,
    129939,
    137344,
    102624,
    90828,
    86487,
    91712,
    114866,
    75697,
    107599,
    99053,
    87511,
    128128,
    57772,
    69314,
    90771,
    145376,
    100730,
    142675,
    112731,
    83985,
    123565,
    127325,
    86597,
    121772,
    131992,
    148859,
    93348,
    77294,
    119763,
    74636,
    95592,
    79628,
    78861,
    68565,
    88820,
    134291,
    69262,
    128678,
    118216,
    52799,
    92731,
    61600,
    63477,
    64016,
    131872,
    131412,
    146579,
    104400,
    99110,
    63458,
    144393,
    54787,
    148622,
    91323,
    61137,
    106082,
    103644,
    63795,
    126648,
    61489,
    140964,
    110963,
    72696,
    124370,
    110466,
    139317,
    108440,
    148062,
    89992,
    145645,
    70556,
    95739
};

using namespace AzCore;

i32 moduleFuel(i32 in) {
    i32 totalFuel = max(in/3-2, 0);
    i32 extraFuel = totalFuel;
    while (extraFuel > 0) {
        extraFuel = max(extraFuel/3-2, 0);
        totalFuel += extraFuel;
    }
    return totalFuel;
}

int main() {
    io::LogStream cout("day1.log");
    cout << "Day 1:" << std::endl;
    i32 totalFuel = 0;
    for (i32 i : data) {
        totalFuel += max(i/3-2, 0);
    }
    cout << "total fuel needed (part1): " << totalFuel << std::endl;
    totalFuel = 0;
    for (i32 i : data) {
        totalFuel += moduleFuel(i);
    }
    cout << "total fuel needed (part2): " << totalFuel << std::endl;
    return 0;
}