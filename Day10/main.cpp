#include <AzCore/IO/LogStream.hpp>
#include <AzCore/memory.hpp>
#include <AzCore/math.hpp>
#include <AzCore/Time.hpp>

using namespace AzCore;

io::LogStream cout("day10.log");

Array<char> inputData;

Array<Range<char>> asteroids;

// Ripped from a random separate project :D
i32 GreatestCommonDivisor(i32 a, i32 b) {
    if (a < 0) a *= -1;
    if (b < 0) b *= -1;
    i32 shift;
    if (a == 0) return b;
    if (b == 0) return a;
    shift = __builtin_ctz(a | b);
    a >>= __builtin_ctz(a);
    do {
        b >>= __builtin_ctz(b);
        if (a > b) {
            i32 t = b;
            b = a;
            a = t;
        }
        b = b - a;
    } while (b != 0);
    return a << shift;
}

bool LineIsOccluded(vec2i origin, vec2i target) {
    vec2i deltaP = target-origin;
    vec2i step = deltaP / GreatestCommonDivisor(deltaP.x, deltaP.y);
    for (vec2i pos = origin + step; pos != target; pos += step) {
        if (asteroids[pos.y][pos.x] == '#')
            return true;
    }
    return false;
}

i32 GetLineOfSightCount(vec2i origin) {
    i32 count = 0;
    for (i32 y = 0; y < asteroids.size; y++) {
        for (i32 x = 0; x < asteroids[y].size; x++) {
            if (asteroids[y][x] != '#')
                continue;
            vec2i pos = {x, y};
            if (pos == origin)
                continue;
            if (!LineIsOccluded(origin, pos))
                count++;
        }
    }
    return count;
}

struct Asteroid  {
    vec2i pos;
    i32 losCount;
};

Asteroid GetBestAsteroid() {
    Asteroid bestAsteroid = {{}, -1};

    for (i32 y = 0; y < asteroids.size; y++) {
        for (i32 x = 0; x < asteroids[y].size; x++) {
            if (asteroids[y][x] != '#')
                continue;
            vec2i pos = {x, y};
            i32 losCount = GetLineOfSightCount(pos);
            if (losCount > bestAsteroid.losCount) {
                bestAsteroid.losCount = losCount;
                bestAsteroid.pos = pos;
            }
        }
    }
    return bestAsteroid;
}

int main() {
    cout << "Day 10:" << std::endl;
    ClockTime start = Clock::now();
    inputData = FileContents("Day10/input.txt");
    if (inputData.size == 0) {
        cout << "Error loading input data." << std::endl;
        return 1;
    }
    asteroids = SeparateByValues(&inputData, {'\n'});

    cout << "Map has a width of " << asteroids[0].size << " and a height of " << asteroids.size << std::endl;

    Asteroid bestAsteroid = GetBestAsteroid();
    cout << "Part 1: The best asteroid, {" << bestAsteroid.pos.x << ", " << bestAsteroid.pos.y << "} has " << bestAsteroid.losCount << " asteroids in its line of sight." << std::endl;

    cout << "Total time taken: " << FormatTime(Clock::now() - start) << std::endl;
    return 0;
}
