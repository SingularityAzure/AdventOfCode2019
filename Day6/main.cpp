#include <AzCore/IO/LogStream.hpp>
#include <AzCore/memory.hpp>

using namespace AzCore;

io::LogStream cout("day6.log");

Array<char> inputData;

struct Body {
    Body *orbits = nullptr;
    i32 GetNumOrbits() {
        i32 result = -1; // We don't count ourself
        Body *it = this;
        while (it != nullptr) {
            result++;
            it = it->orbits;
        }
        return result;
    }
};

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

#include <cstdio>

int main() {

    cout << "Day 6:" << std::endl;

    {
        FILE *file = fopen("Day6/input.txt", "rb");
        if (!file) {
            cout << "Failed to get input data." << std::endl;
            return 1;
        }
        fseek(file, 0, SEEK_END);
        inputData.Resize(ftell(file));
        fseek(file, 0, SEEK_SET);
        if ((i32)fread(inputData.data, 1, inputData.size, file) != inputData.size) {
            cout << "Didn't load whole file?" << std::endl;
            return 1;
        }
        fclose(file);
    }
    if (!ReadInputData()) {
        return 1;
    }
    cout << "Number of bodies: " << bodies.size() << std::endl;
    i64 totalOrbits = 0;
    for (auto body : bodies) {
        totalOrbits += body.second->GetNumOrbits();
    }
    cout << "Total number of orbits: " << totalOrbits << std::endl;

    for (auto body : bodies) {
        delete body.second;
    }
    return 0;
}
