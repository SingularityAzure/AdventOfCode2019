#include <AzCore/basictypes.hpp>
#include <AzCore/memory.hpp>
#include <AzCore/IO/LogStream.hpp>
#include <AzCore/math.hpp>
#include <AzCore/Time.hpp>

#include "movements.c"

using namespace AzCore;

io::LogStream cout("day3.log");

enum {
    DIR_UP=1,
    DIR_DOWN=2,
    DIR_RIGHT=3,
    DIR_LEFT=4
};

struct move {
    u16 distance;
    u16 direction;
};

Array<move> GetMoves(const char *text) {
    Array<move> moves;
    for (i32 i = 0; text[i] != 0;) {
        move thisMove{0};
        if (text[i] == 'U') {
            thisMove.direction = DIR_UP;
        } else if (text[i] == 'D') {
            thisMove.direction = DIR_DOWN;
        } else if (text[i] == 'R') {
            thisMove.direction = DIR_RIGHT;
        } else if (text[i] == 'L') {
            thisMove.direction = DIR_LEFT;
        } else {
            cout << "Error: Unexpected direction " << text[i] << std::endl;
            return {};
        }
        i++;
        while(text[i] != ',') {
            thisMove.distance *= 10;
            thisMove.distance += text[i]-'0';
            i++;
        }
        moves.Append(thisMove);
        i++;
    }
    return moves;
}

struct line {
    vec2i a, b;
    i32 startSteps;
};

Array<line> Lines(Array<move> &moves) {
    Array<line> lines;
    lines.Reserve(moves.size);
    vec2i pos = vec2i(0);
    vec2i posPrev = pos;
    i32 stepsTaken = 0;

    for (move m : moves) {
        if (m.direction == DIR_UP) {
            pos.y += m.distance;
        } else if (m.direction == DIR_DOWN) {
            pos.y -= m.distance;
        } else if (m.direction == DIR_LEFT) {
            pos.x -= m.distance;
        } else if (m.direction == DIR_RIGHT) {
            pos.x += m.distance;
        } else {
            cout << "Error: Unexpected direction " << m.direction << std::endl;
        }
        lines.Append({posPrev, pos, stepsTaken});
        stepsTaken += m.distance;
        posPrev = pos;
    }
    return lines;
}

struct intersect {
    vec2i pos;
    i32 steps;
};

bool Intersection(line line1, line line2, intersect &out) {
    bool vert1 = line1.a.x == line1.b.x;
    bool vert2 = line2.a.x == line2.b.x;
    if (vert1 == vert2) return false;
    if (max(line1.a.x, line1.b.x) < min(line2.a.x, line2.b.x)) return false;
    if (min(line1.a.x, line1.b.x) > max(line2.a.x, line2.b.x)) return false;
    if (max(line1.a.y, line1.b.y) < min(line2.a.y, line2.b.y)) return false;
    if (min(line1.a.y, line1.b.y) > max(line2.a.y, line2.b.y)) return false;
    if (vert1) {
        out.pos.x = line1.a.x;
        out.pos.y = line2.a.y;
        out.steps = line1.startSteps + abs(line2.a.y - line1.a.y) + line2.startSteps + abs(line1.a.x - line2.a.x);
    } else {
        out.pos.x = line2.a.x;
        out.pos.y = line1.a.y;
        out.steps = line1.startSteps + abs(line2.a.x - line1.a.x) + line2.startSteps + abs(line1.a.y - line2.a.y);
    }
    return true;
}

Array<intersect> Intersections(Array<line> &lines1, Array<line> &lines2) {
    Array<intersect> intersections;
    for (i32 i = 0; i < lines1.size; i++) {
        for (i32 j = 0; j < lines2.size; j++) {
            intersect intersection;
            if (Intersection(lines1[i], lines2[j], intersection)) {
                intersections.Append(intersection);
            }
        }
    }
    return intersections;
}

int main() {
    cout << "Day 3:" << std::endl;
    ClockTime start = Clock::now();
    Array<move> wire1Moves = GetMoves(wire1);
    Array<move> wire2Moves = GetMoves(wire2);

    cout << "wire 1 has " << wire1Moves.size << " moves, wire 2 has " << wire2Moves.size << " moves." << "\n";

    Array<line> wire1Lines = Lines(wire1Moves);
    Array<line> wire2Lines = Lines(wire2Moves);

    Array<intersect> intersections = Intersections(wire1Lines, wire2Lines);

    cout << "There are " << intersections.size << " intersections." << "\n";

    i32 minDist = INT32_MAX;
    i32 minSteps = INT32_MAX;
    intersect closestIntersection;
    intersect fastestIntersection;
    for (intersect p : intersections) {
        i32 dist = abs(p.pos.x) + abs(p.pos.y);
        if (dist > 0 && dist < minDist) {
            minDist = dist;
            closestIntersection = p;
        }
        if (p.steps > 0 && p.steps < minSteps) {
            minSteps = p.steps;
            fastestIntersection = p;
        }
    }

    cout << "The closest intersection " << closestIntersection.pos.x << ", " << closestIntersection.pos.y << " has a Manhattan Distance of " << minDist << "\n";
    cout << "The fastest intersection " << fastestIntersection.pos.x << ", " << fastestIntersection.pos.y << " has a total step number of " << minSteps << "\n";

    cout << "Total time taken: " << FormatTime(Clock::now() - start) << std::endl;
    return 0;
}