#ifndef VEC2_HPP
#define VEC2_HPP

#include <cmath>
#include "common.hpp"

using namespace std;

class vec2 {
    public:
        double p[2];

        vec2() : vec2(0, 0) {}
        vec2(double c0, double c1) {
            p[0] = c0;
            p[1] = c1;
        }
};
#endif