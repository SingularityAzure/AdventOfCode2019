#include <AzCore/IO/LogStream.hpp>
#include <AzCore/math.hpp>
#include <AzCore/Time.hpp>
#include <AzCore/memory.hpp>

using namespace AzCore;

io::LogStream cout("day12.log");

Array<vec3i> input = {
    {0, 6, 1},
    {4, 4, 19},
    {-11, 1, 8},
    {2, 19, 15}
};

struct Body {
    vec3i pos, vel;
    i32 GetEnergy() {
        i32 result;
        result = abs(pos.x) + abs(pos.y) + abs(pos.z);
        // This is wrong but also just what the challenge says to do...
        result *= abs(vel.x) + abs(vel.y) + abs(vel.z);
        return result;
    }
};

Array<Body> bodies(input.size);

void ResetBodies() {
    for (i32 i = 0; i < input.size; i++) {
        bodies[i].pos = input[i];
        bodies[i].vel = 0;
    }
}

void SimulateStep() {
    for (i32 i = 0; i < bodies.size; i++) {
        Body &body1 = bodies[i];
        for (i32 j = 0; j < bodies.size; j++) {
            Body &body2 = bodies[j];
            vec3i deltaP = body2.pos-body1.pos;
            if (deltaP.x != 0)
                body1.vel.x += sign(deltaP.x);
            if (deltaP.y != 0)
                body1.vel.y += sign(deltaP.y);
            if (deltaP.z != 0)
                body1.vel.z += sign(deltaP.z);
        }
    }
    for (i32 i = 0; i < bodies.size; i++) {
        bodies[i].pos += bodies[i].vel;
    }
}

i32 GetAllSystemEnergy() {
    i32 result = 0;
    for (i32 i = 0; i < bodies.size; i++) {
        result += bodies[i].GetEnergy();
    }
    return result;
}

int main() {
    cout << "Day 12:" << std::endl;
    ClockTime start = Clock::now();

    ResetBodies();
    for (i32 i = 0; i < 1000; i++) {
        SimulateStep();
    }

    cout << "Part 1: All system energy after 1000 steps: " << GetAllSystemEnergy() << std::endl;

    cout << "Total time taken: " << FormatTime(Clock::now()-start) << std::endl;
    return 0;
}
