// AoC8.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>


class layer {
    std::vector<short> pixels;
    int width;
    int height;
    layer(int width, int height) : width(width), height(height), pixels(std::vector<short>(width * height)) {};
public:
    layer(int width, int height, std::vector<short> pixels) : width(width), height(height), pixels(pixels) {}
    short getAt(int index) { return pixels[index]; }
    short getAt(int w, int h) { return pixels[h * width + w]; }
    
    int countNum(short num) { return std::count(pixels.begin(), pixels.end(), num); }

    std::ostream& getOutput(std::ostream& output) { 
        for (auto digit : pixels) {
            output << digit;
        }
        return output;
    }

    friend layer operator+(layer const&, layer const&);
    friend std::ostream& operator<<(std::ostream&, layer const&);
    
};

layer operator+(layer const& l1, layer const& l2) {
    layer out(l1.width, l1.height);
    for (int i = 0; i < out.pixels.size(); i++) {
        out.pixels[i] = l1.pixels[i] == 2 ? l2.pixels[i] : l1.pixels[i];
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, layer const& l) {
    int index = 0;
    int first = true;
    for (auto pixel : l.pixels) {
        if (index % l.width == 0 && !first) {
            out << std::endl;
        }
        out << pixel;
        index++;
        first = false;
    }
    return out;
}

class image {
    std::vector<layer> layers;
public:
    image(int width, int height, std::vector<short> pixels) {
        int length = width * height;
        while (pixels.size() > 0) {
            std::vector<short> layerPixels(length);
            std::copy(pixels.begin(), pixels.begin() + (length), layerPixels.begin());
            layers.push_back(layer(width, height, layerPixels));
            pixels.erase(pixels.begin(), pixels.begin() + (length));
        }
    }
    layer* getLayer(int index) { return &layers[index]; }
    int getNumLayers() { return layers.size(); }
};

int main()
{
    int width = 25;
    int height = 6;

    std::ifstream inputStream("Input.txt");
    std::vector<short> input{};
    char digit;
    while (inputStream >> digit) {
        input.push_back(digit-'0');
    }

    image pic(width, height, input);
    std::cout << pic.getNumLayers() << std::endl;

    layer* leastLayer = nullptr;
    int leastIndex = 0;
    {
        int leastCount = INT_MAX;
        for (int i = 0; i < pic.getNumLayers(); i++) {
            if (pic.getLayer(i)->countNum(0) < leastCount) {
                leastLayer = pic.getLayer(i);
                leastCount = leastLayer->countNum(0);
                leastIndex = i;
            }
        }
    }
    std::cout << "Least 0 layer: " << leastIndex << " - " << leastLayer->countNum(1) * leastLayer->countNum(2) << std::endl;

    layer finalLayer(width, height, std::vector<short>(width * height, 2));
    //std::cout << finalLayer << std::endl;
    for (int i = 0; i < pic.getNumLayers(); i++) {
        finalLayer = finalLayer + *(pic.getLayer(i));
        //std::cout << finalLayer << std::endl;
    }
    std::ofstream outFile("Output.txt");
    finalLayer.getOutput(outFile);
    std::cout << finalLayer << std::endl;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
