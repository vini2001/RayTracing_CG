#ifndef COMMON_HPP
#define COMMON_HPP

#include "ray.hpp"
#include "vec3.hpp"
#include <cstdlib>

using namespace std;

const double infinity = numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions
inline double degreesToRadians(double degrees) {
    return degrees * pi / 180.0;
}

inline double randomDouble() { // [0,1)
    return rand() / (RAND_MAX + 1.0);
}

inline double randomDouble(double min, double max) { // min,max).
    return min + (max-min)*randomDouble();
}

#endif // !COMMON_HPP

