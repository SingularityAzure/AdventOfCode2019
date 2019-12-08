#include <AzCore/IO/LogStream.hpp>
#include <AzCore/memory.hpp>
#include <AzCore/Time.hpp>

using namespace AzCore;

io::LogStream cout("day6.log");

Array<char> inputData;

struct Body;

struct BodyIterator {
    Body *it;
    void operator++();
    Body* operator*() { return it; }
    bool operator!=(const BodyIterator& other) { return it != other.it; }
};

struct Body {
    Body *orbits = nullptr;

    BodyIterator begin() {
        return {this};
    }
    BodyIterator end() {
        return {nullptr};
    }

    i32 GetNumOrbits() {
        i32 result = -1; // We don't count ourself
        for ([[maybe_unused]]Body* body : *this) {
            result++;
        }
        return result;
    }
};

void BodyIterator::operator++() {
    it = it->orbits;
}

Map<String, Body*> bodies;

bool ReadInputData() {
    for (i32 i = 0; i < inputData.size;) {
        String body1 = "XXX", body2 = "XXX";
        for (i32 j = 0; inputData[i] != ')'; i++, j++) {
            body1[j] = inputData[i];
        }
        i++;
        for (i32 j = 0; i < inputData.size && inputData[i] != '\n'; i++, j++) {
            body2[j] = inputData[i];
        }
        i++;
        Body *pBody1, *pBody2;
        if (bodies.count(body1) == 0) {
            pBody1 = new Body();
            bodies[body1] = pBody1;
        } else {
            pBody1 = bodies[body1];
        }
        if (bodies.count(body2) == 0) {
            pBody2 = new Body();
            bodies[body2] = pBody2;
        } else {
            pBody2 = bodies[body2];
        }
        if (pBody2->orbits) {
            cout << "Error: body " << body2 << " already has an orbit!" << std::endl;
            return false;
        }
        pBody2->orbits = pBody1;
    }
    return true;
}

int main() {

    cout << "Day 6:" << std::endl;
    ClockTime start = Clock::now();

    inputData = FileContents("Day6/input.txt");
    if (inputData.size == 0) {
        cout << "Failed to retrieve file contents." << std::endl;
        return 1;
    }
    if (!ReadInputData()) {
        return 1;
    }
    cout << "Number of bodies: " << bodies.size() << std::endl;
    i64 totalOrbits = 0;
    for (auto body : bodies) {
        totalOrbits += body.second->GetNumOrbits();
    }
    cout << "Total number of orbits (part 1): " << totalOrbits << std::endl;

    Body *you = bodies["YOU"];
    Body *san = bodies["SAN"];
    Body *commonBody = nullptr;
    Set<Body*> bodiesYouAreOrbiting;
    for (Body* body : *you) {
        bodiesYouAreOrbiting.emplace(body);
    }
    for (Body* body : *san) {
        if (bodiesYouAreOrbiting.count(body) == 1) {
            commonBody = body;
            break;
        }
    }
    if (commonBody == nullptr) {
        cout << "Error: No common body found!" << std::endl;
        return 1;
    }

    i32 numTransfers = -4; // Don't include YOU or SAN

    for (Body* body : *you) {
        numTransfers++;
        if (body == commonBody) {
            break;
        }
    }
    for (Body* body : *san) {
        numTransfers++;
        if (body == commonBody) {
            break;
        }
    }

    cout << "Total number of transfers needed to get from YOU to SAN (part 2): " << numTransfers << std::endl;

    cout << "Total time taken: " << FormatTime(Clock::now() - start) << std::endl;

    for (auto body : bodies) {
        delete body.second;
    }
    return 0;
}
