#ifndef VEC4_HPP
#define VEC4_HPP

#include <cmath>
#include "common.hpp"

using namespace std;

class vec4 {
    public:
        double p[4];

        vec4() : vec4(0, 0, 0, 0) {}
        vec4(double c0, double c1, double c2, double c3) {
            p[0] = c0;
            p[1] = c1;
            p[2] = c2;
            p[3] = c3;
        }

        double operator[] (int i) const { return p[i]; }
        double& operator[] (int i) { return p[i]; }

        inline double dot(const vec3 &v) const {
            const vec4 &u = *this;
            return u.p[0] * v.p[0] + u.p[1] * v.p[1] + u.p[2] * v.p[2] + u.p[3] * 1.0;
        }
};
#endif