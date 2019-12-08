#include <AzCore/basictypes.hpp>
#include <AzCore/IO/LogStream.hpp>
#include <AzCore/Time.hpp>
#include <AzCore/memory.hpp>

using namespace AzCore;

io::LogStream cout("day8.log");

constexpr i32 width = 25, height = 6;
constexpr i32 pixelsPerLayer = width * height;

Array<char> inputData;

int main() {
    ClockTime start = Clock::now();
    cout << "Day 8:" << std::endl;

    inputData = FileContents("Day8/input.txt");
    if (inputData.size == 0) {
        cout << "Failed to retrieve file contents." << std::endl;
        return 1;
    }
    i32 layers = inputData.size / pixelsPerLayer;
    cout << "There are " << layers << " layers." << std::endl;
    i32 minZeroes = INT32_MAX;
    i32 minLayer = -1;
    i32 minMultiple = 0;
    for (i32 i = 0; i < layers; i++) {
        Range<char> layer = inputData.GetRange(i * pixelsPerLayer, pixelsPerLayer);
        i32 numZeroes = layer.Count('0');
        if (numZeroes < minZeroes) {
            minZeroes = numZeroes;
            minMultiple = layer.Count('1') * layer.Count('2');
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
