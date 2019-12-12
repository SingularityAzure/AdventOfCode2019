#include <AzCore/IO/LogStream.hpp>
#include <AzCore/math.hpp>
#include <AzCore/Time.hpp>
#include <AzCore/memory.hpp>

using namespace AzCore;

io::LogStream cout("day12.log");

typedef vec3_t<i32> vec;

i64 GreatestCommonDivisor(i64 a, i64 b) {
    if (a < 0) a *= -1;
    if (b < 0) b *= -1;
    i32 shift;
    if (a == 0) return b;
    if (b == 0) return a;
    shift = __builtin_ctzll(a | b);
    a >>= __builtin_ctzll(a);
    do {
        b >>= __builtin_ctzll(b);
        if (a > b) {
            i64 t = b;
            b = a;
            a = t;
        }
        b = b - a;
    } while (b != 0);
    return a << shift;
};

i64 LeastCommonMultiple(i64 a, i64 b) {
    return a*b / GreatestCommonDivisor(a, b);
}
i64 LeastCommonMultiple(i64 a, i64 b, i64 c) {
    return LeastCommonMultiple(LeastCommonMultiple(a, b), c);
}

vec input[4] = {
    {0, 6, 1},
    {4, 4, 19},
    {-11, 1, 8},
    {2, 19, 15}
};
// vec input[4] = {
//     {-1,  0, 2},
//     { 2,-10,-7},
//     { 4, -8, 8},
//     { 3,  5,-1}
// };

struct Body {
    vec pos, vel;
    i32 GetEnergy() {
        i32 result;
        result = abs(pos.x) + abs(pos.y) + abs(pos.z);
        // This is wrong but also just what the challenge says to do...
        result *= abs(vel.x) + abs(vel.y) + abs(vel.z);
        return result;
    }
};

Body bodies[4];

void ResetBodies() {
    for (i32 i = 0; i < 4; i++) {
        bodies[i].pos = input[i];
        bodies[i].vel = 0;
    }
}

void SimulateStep() {
    for (i32 i = 0; i < 4; i++) {
        Body &body1 = bodies[i];
        for (i32 j = i+1; j < 4; j++) {
            Body &body2 = bodies[j];
            vec deltaP = body2.pos-body1.pos;
            if (deltaP.x != 0) {
                i32 s = sign(deltaP.x);
                body1.vel.x += s;
                body2.vel.x -= s;
            }
            if (deltaP.y != 0) {
                i32 s = sign(deltaP.y);
                body1.vel.y += s;
                body2.vel.y -= s;
            }
            if (deltaP.z != 0) {
                i32 s = sign(deltaP.z);
                body1.vel.z += s;
                body2.vel.z -= s;
            }
        }
    }
    for (i32 i = 0; i < 4; i++) {
        bodies[i].pos += bodies[i].vel;
    }
}

i32 GetAllSystemEnergy() {
    i32 result = 0;
    for (i32 i = 0; i < 4; i++) {
        result += bodies[i].GetEnergy();
    }
    return result;
}

void MatchesInitalConditions(bool &x, bool &y, bool &z) {
    x = true;
    y = true;
    z = true;
    for (i32 i = 0; i < 4; i++) {
        if ((bodies[i].pos.x-input[i].x != 0 || bodies[i].vel.x) != 0) x = false;
        if ((bodies[i].pos.y-input[i].y != 0 || bodies[i].vel.y) != 0) y = false;
        if ((bodies[i].pos.z-input[i].z != 0 || bodies[i].vel.z) != 0) z = false;
    }
}

int main() {
    cout << "Day 12:" << std::endl;
    ClockTime start = Clock::now();

    ResetBodies();
    for (i32 i = 0; i < 1000; i++) {
        SimulateStep();
    }

    cout << "Part 1: All system energy after 1000 steps: " << GetAllSystemEnergy() << std::endl;
    i64 steps = 0;
    i64 stepsX=0, stepsY=0, stepsZ=0;
    ResetBodies();
    while (true) {
        SimulateStep();
        steps++;
        bool x, y, z;
        MatchesInitalConditions(x, y, z);
        if (!stepsX && x) stepsX = steps;
        if (!stepsY && y) stepsY = steps;
        if (!stepsZ && z) stepsZ = steps;
        if (stepsX && stepsY && stepsZ) break;
        if (steps%100000000 == 0) cout << "Steps so far: " << steps/100000000 << "00 million" << std::endl;
    }

    i64 neededSteps = LeastCommonMultiple(stepsX, stepsY, stepsZ);
    cout << "Part 2: System takes LCM(" << stepsX << "," << stepsY << "," << stepsZ << ") = " << neededSteps << " steps to go back to our initial conditions." << std::endl;

    cout << "Total time taken: " << FormatTime(Clock::now()-start) << std::endl;
    return 0;
}
