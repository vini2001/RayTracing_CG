#ifndef COLOR_HPP
#define COLOR_HPP

#include "vec3.hpp"
#include <iostream>

using namespace std;

void output(const vec3& color) {
    cout << static_cast<int>(255.999 * clamp(color.x(), 0.0, 1.0)) << " "
       << static_cast<int>(255.999 * clamp(color.y(), 0.0, 1.0)) << " "
       << static_cast<int>(255.999 * clamp(color.z(), 0.0, 1.0)) << "\n";
}

void outputColor(color pixelColor, int samplesPerPixel) {
    output(pixelColor / samplesPerPixel);
}

#endif

