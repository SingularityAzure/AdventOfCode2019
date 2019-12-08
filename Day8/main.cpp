#include <AzCore/basictypes.hpp>
#include <AzCore/IO/LogStream.hpp>
#include <AzCore/Time.hpp>

using namespace AzCore;

io::LogStream cout("day8.log");

constexpr i32 width = 25, height = 6;
constexpr i32 pixelsPerLayer = width * height;

Array<char> inputData;

#include <cstdio>

int main() {
    ClockTime start = Clock::now();
    cout << "Day 8:" << std::endl;

    {
        FILE *file = fopen("Day8/input.txt", "rb");
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
    i32 layers = inputData.size / pixelsPerLayer;
    cout << "There are " << layers << " layers." << std::endl;
    i32 minZeroes = INT32_MAX;
    i32 minLayer = -1;
    i32 minMultiple = 0;
    for (i32 i = 0; i < layers; i++) {
        i32 numZeroes = 0;
        i32 numOnes = 0;
        i32 numTwos = 0;
        for (i32 j = i * pixelsPerLayer; j < (i+1)*pixelsPerLayer; j++) {
            if (inputData[j] == '0') numZeroes++;
            if (inputData[j] == '1') numOnes++;
            if (inputData[j] == '2') numTwos++;
        }
        if (numZeroes < minZeroes) {
            minZeroes = numZeroes;
            minMultiple = numOnes*numTwos;
            minLayer = i;
        }
    }
    cout << "Part 1: The layer with the fewest zeroes (" << minZeroes << ") is layer " << minLayer << " with 1s * 2s = " << minMultiple << std::endl;

    Array<char> composite(pixelsPerLayer);

    for (i32 i = layers-1; i >= 0; i--) {
        for (i32 j = 0; j < pixelsPerLayer; j++) {
            char layerPixel = inputData[i*pixelsPerLayer+j];
            composite[j] = layerPixel == '2' ? composite[j] : layerPixel;
        }
    }

    cout << "Part 2 composite:\n";
    for (i32 i = 0; i < height; i++) {
        for (i32 j = 0; j < width; j++) {
            cout << (composite[j+i*width] == '0' ? ' ' : '#');
        }
        cout << "\n";
    }
    cout << std::endl;

    cout << "Total time taken: " << FormatTime(Clock::now()-start) << std::endl;
    return 0;
}
