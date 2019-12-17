#include <AzCore/IO/LogStream.hpp>
#include <AzCore/memory.cpp>
#include <AzCore/Time.hpp>

using namespace AzCore;

io::LogStream cout("day17.log");

#include "../intcode.cpp"

String output;
Array<Range<char>> lines;
Array<vec2i> intersections;
vec2i startingPos = -1;

enum Dir {
    DIR_UP=0,
    DIR_DOWN=1,
    DIR_LEFT=2,
    DIR_RIGHT=3
};

vec2i dirs[] = {
    { 0,-1},
    { 0, 1},
    {-1, 0},
    { 1, 0}
};

bool CanGoTo(vec2i pos) {
    if (pos.x < 0 || pos.x >= lines[0].size) return false;
    if (pos.y < 0 || pos.y >= lines.size) return false;
    return lines[pos.y][pos.x] == '#';
}

Array<String> GetAllMoves() {
    Array<String> allMoves;
    vec2i pos = startingPos;
    Dir dir = DIR_UP;
    i32 forward = 0;
    while (true) {
        if (CanGoTo(pos+dirs[dir])) {
            forward++;
            pos += dirs[dir];
            // if (forward == 2) {
            //     allMoves.Append(ToString(forward));
            //     forward = 0;
            // }
        } else {
            if (forward > 0) {
                allMoves.Append(ToString(forward));
                forward = 0;
            }
            if (dir < 2) {
                // We're vertical
                if (CanGoTo(pos+dirs[DIR_LEFT])) {
                    allMoves.Append(dir == DIR_UP ? "L" : "R");
                    dir = DIR_LEFT;
                } else if (CanGoTo(pos+dirs[DIR_RIGHT])) {
                    allMoves.Append(dir == DIR_UP ? "R" : "L");
                    dir = DIR_RIGHT;
                } else {
                    // Dead end
                    break;
                }
            } else {
                // We're horizontal
                if (CanGoTo(pos+dirs[DIR_UP])) {
                    allMoves.Append(dir == DIR_RIGHT ? "L" : "R");
                    dir = DIR_UP;
                } else if (CanGoTo(pos+dirs[DIR_DOWN])) {
                    allMoves.Append(dir == DIR_RIGHT ? "R" : "L");
                    dir = DIR_DOWN;
                } else {
                    // Dead end
                    break;
                }
            }
        }
    }

    return allMoves;
}

template<typename T>
String StringifyWithCommas(const Array<T> &array) {
    String result;
    result.Reserve(array.size*2);
    for (i32 i = 0; i < array.size; i++) {
        result += array[i];
        if (i < array.size-1) {
            result += ',';
        }
    }
    result += '\n';
    return result;
}

bool Collides(Range<String> candidate, Array<Range<String>> taken) {
    i32 c1 = candidate.index;
    i32 c2 = c1 + candidate.size;
    for (Range<String> i : taken) {
        i32 i1 = i.index;
        i32 i2 = i1 + i.size;
        if (c1 < i2 && c2 > i1) return true;
    }
    return false;
}

struct Sorting {
    i32 index;
    char c;
    bool operator<(Sorting &a) {
        return index < a.index;
    }
};

void AddSorted(Array<Range<String>> &toSort, Array<Sorting> &sorting, char c) {
    for (Range<String> r : toSort) {
        Sorting sort = {r.index, c};
        i32 pos = 0;
        for (; pos < sorting.size; pos++) {
            if (sort < sorting[pos]) break;
        }
        sorting.Insert(pos, sort);
    }
}

bool BreakUpMoves(Array<String> &allMoves, Array<String> &movesA, Array<String> &movesB, Array<String> &movesC, Array<char> &master) {
    Array<Range<String>> rangesA;
    i32 sizeA = 10;
    i32 posA1;
    for (; sizeA > 1; sizeA--) {
        continueA:
        if (sizeA <= 1) break;
        posA1 = 0;
        // for (; posA1 < allMoves.size-sizeA; posA1++) {
            rangesA.Clear();
            Range<String> range1 = allMoves.GetRange(posA1, sizeA);
            rangesA.Append(range1);
            for (i32 pos2 = posA1+sizeA; pos2 <= allMoves.size-sizeA; pos2++) {
                Range<String> range2 = allMoves.GetRange(pos2, sizeA);
                if (range1 == range2) {
                    rangesA.Append(range2);
                }
            }
            if (rangesA.size > 1) {
                // posA1++;
                sizeA--;
                goto startB;
            }
        // }
    }
    return false;
startB:
    Array<Range<String>> rangesB;
    i32 sizeB = 10;
    i32 posB1;
    for (; sizeB > 1; sizeB--) {
        posB1 = sizeA;
continueB:
        for (; posB1 < allMoves.size-sizeB; posB1++) {
            rangesB.Clear();
            Range<String> range1 = allMoves.GetRange(posB1, sizeB);
            if (Collides(range1, rangesA)) continue;
            rangesB.Append(range1);
            for (i32 pos2 = posB1+sizeB; pos2 <= allMoves.size-sizeB; pos2++) {
                Range<String> range2 = allMoves.GetRange(pos2, sizeB);
                if (Collides(range2, rangesA)) continue;
                if (range1 == range2) {
                    rangesB.Append(range2);
                }
            }
            if (rangesB.size > 1) {
                posB1++;
                goto startC;
            }
        }
    }
    goto continueA;
startC:
    Array<Range<String>> rangesC;
    i32 sizeC = 10;
    i32 posC1;
    for (; sizeC > 1; sizeC--) {
        posC1 = posB1 + sizeB;
continueC:
        for (; posC1 < allMoves.size-sizeC; posC1++) {
            rangesC.Clear();
            Range<String> range1 = allMoves.GetRange(posC1, sizeC);
            if (Collides(range1, rangesA)) continue;
            if (Collides(range1, rangesB)) continue;
            rangesC.Append(range1);
            for (i32 pos2 = posC1+sizeC; pos2 <= allMoves.size-sizeC; pos2++) {
                Range<String> range2 = allMoves.GetRange(pos2, sizeC);
                if (Collides(range2, rangesA)) continue;
                if (Collides(range2, rangesB)) continue;
                if (range1 == range2) {
                    rangesC.Append(range2);
                }
            }
            if (rangesC.size > 1) {
                posC1++;
                goto checkFinal;
            }
        }
    }
    goto continueB;
checkFinal:
    Array<bool> hit(allMoves.size, false);
    for (Range<String> r : rangesA) {
        for (i32 i = 0; i < r.size; i++)
            hit[i+r.index] = true;
    }
    for (Range<String> r : rangesB) {
        for (i32 i = 0; i < r.size; i++)
            hit[i+r.index] = true;
    }
    for (Range<String> r : rangesC) {
        for (i32 i = 0; i < r.size; i++)
            hit[i+r.index] = true;
    }
    for (i32 i = 0; i < hit.size; i++) {
        if (!hit[i]) {
            goto continueC;
        }
    }
    // Output everything
    movesA = rangesA[0];
    movesB = rangesB[0];
    movesC = rangesC[0];
    master.Clear();
    Array<Sorting> sorting;
    AddSorted(rangesA, sorting, 'A');
    AddSorted(rangesB, sorting, 'B');
    AddSorted(rangesC, sorting, 'C');
    for (i32 i = 0; i < sorting.size; i++) {
        master.Append(sorting[i].c);
    }
    return true;
}

void SendProgramString(Program &program, String string) {
    for (i32 i = 0; i < string.size;) {
        Ret ret = RunProgram(program);
        if (ret.input) {
            *ret.input = string[i++];
        }
    }
}

int main() {
    cout << "Day 17:" << std::endl;
    ClockTime start = Clock::now();

    GetInput("Day17/input.txt");
    Program program;
    while (true) {
        Ret ret = RunProgram(program);
        if (ret.halt) break;
        output.Append((char)ret.output);
    }
    lines = SeparateByValues(&output, {'\n'});
    i32 sumOfAlignmentParameters = 0;
    for (i32 y = 1; y < lines.size-1; y++) {
        for (i32 x = 1; x < lines[y].size-1; x++) {
            if (lines[y][x] != '#') continue;
            if (lines[y-1][x] == '#'
             && lines[y+1][x] == '#'
             && lines[y][x-1] == '#'
             && lines[y][x+1] == '#') {
                vec2i intersection = {x, y};
                sumOfAlignmentParameters += x * y;
                intersections.Append(intersection);
            }
        }
    }
    for (i32 y = 0; y < lines.size && startingPos.x == -1; y++) {
        for (i32 x = 0; x < lines[y].size; x++) {
            if (lines[y][x] == '^') {
                startingPos = {x, y};
                break;
            }
        }
    }
    cout << output << std::endl;
    cout << "Part 1: Sum of alignment parameters is " << sumOfAlignmentParameters << std::endl;

    Array<String> allMoves = GetAllMoves();

    cout << "All moves (size " << allMoves.size << "):\n";
    cout << StringifyWithCommas(allMoves) << std::endl;

    Array<String> movesA, movesB, movesC;
    Array<char> master;

    if (!BreakUpMoves(allMoves, movesA, movesB, movesC, master)) {
        cout << "Couldn't find a way to compress the moves..." << std::endl;
        return 1;
    }
    String masterProgram = StringifyWithCommas(master);
    String functionA = StringifyWithCommas(movesA);
    String functionB = StringifyWithCommas(movesB);
    String functionC = StringifyWithCommas(movesC);
    cout << "Master program: " << masterProgram;
    cout << "A: " << functionA;
    cout << "B: " << functionB;
    cout << "C: " << functionC << std::endl;

    program.Reset();
    program.opcodes[0] = 2;

    SendProgramString(program, masterProgram);
    SendProgramString(program, functionA);
    SendProgramString(program, functionB);
    SendProgramString(program, functionC);
    SendProgramString(program, "n\n");
    i64 dustCollected;
    while (true) {
        Ret ret = RunProgram(program);
        if (ret.halt) {
            break;
        }
        if (ret.input) {
            cout << "Unexpected input instruction!" << std::endl;
            return 1;
        }
        dustCollected = ret.output;
    }
    cout << "Part 2: Total dust collected: " << dustCollected << std::endl;

    cout << "Total time taken: " << FormatTime(Clock::now() - start) << std::endl;
    return 0;
}
