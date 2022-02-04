#ifndef VEC3_HPP
#define VEC3_HPP

#include <cmath>
#include <iostream>
#include "common.hpp"

using namespace std;

class vec3 {
    public:

        double p[3];

        vec3() : vec3(0, 0, 0) {}
        vec3(double c0, double c1, double c2) {
            p[0] = c0;
            p[1] = c1;
            p[2] = c2;
        }
        
        double x() const { return p[0]; }
        double y() const { return p[1]; }
        double z() const { return p[2]; }

        vec3 operator-() const { return vec3(-p[0], -p[1], -p[2]); }
        double operator[] (int i) const { return p[i]; }
        double& operator[] (int i) { return p[i]; }

        vec3& operator+= (const vec3& u) {
            p[0] += u.p[0];
            p[1] += u.p[1];
            p[2] += u.p[2];
            return *this;
        }

        vec3& operator+= (const double v) {
            p[0] += v;
            p[1] += v;
            p[2] += v;
            return *this;
        }

        vec3& operator*= (const double v) {
            p[0] *= v;
            p[1] *= v;
            p[2] *= v;
            return *this;
        }

        vec3& operator/=(const double t) {
            return *this *= 1/t;
        }

        double length() const {
            return sqrt(lengthSquared());
        }

        double lengthSquared() const {
            return p[0]*p[0] + p[1]*p[1] + p[2]*p[2];
        }

        inline ostream& print(ostream& os) const {
            os << p[0] << " " << p[1] << " " << p[2];
            return os;
        }

        inline vec3 operator+(vec3 &v) {
            return vec3(p[0] + v.p[0], p[1] + v.p[1], p[2] + v.p[2]);
        }

        inline vec3 operator-(vec3 &v) {
            return vec3(p[0] - v.p[0], p[1] - v.p[1], p[2] - v.p[2]);
        }

        inline vec3 operator*(vec3 &v) {
            return vec3(p[0] * v.p[0], p[1] * v.p[1], p[2] * v.p[2]);
        }

        inline vec3 operator/(vec3 &v) {
            return vec3(p[0] / v.p[0], p[1] / v.p[1], p[2] / v.p[2]);
        }

        inline vec3 operator/(double t) {
            return vec3(p[0] / t, p[1] / t, p[2] / t);
        }

        inline double dot(const vec3 &v) const {
            const vec3 &u = *this;
            return u.p[0] * v.p[0] + u.p[1] * v.p[1] + u.p[2] * v.p[2];
        }

        inline static vec3 cross(const vec3 &u, const vec3 &v) {
            return vec3(u.p[1] * v.p[2] - u.p[2] * v.p[1],
                        u.p[2] * v.p[0] - u.p[0] * v.p[2],
                        u.p[0] * v.p[1] - u.p[1] * v.p[0]);
        }

        inline vec3 normalize() {
            return (*this) / length();
        }

        inline vec3 sqrtv(){
            return vec3(sqrt(p[0]), sqrt(p[1]), sqrt(p[2]));
        }

        bool sameDirection(const vec3 &v) {
            return dot(v) > 0;
        }

        inline static vec3 random() {
            return vec3(randomDouble(), randomDouble(), randomDouble());
        }

        inline static vec3 random(double min, double max) {
            return vec3(randomDouble(min, max), randomDouble(min, max), randomDouble(min, max));
        }

        inline static vec3 randomInUnitSphere() {
            vec3 p;
            do {
                p = vec3::random(-1, 1);
            }while(p.lengthSquared() >= 1);
            return p;
        }

        inline static vec3 randomUnitVector() {
            return randomInUnitSphere().normalize();
        }

        inline static vec3 randomInHemisphere(const vec3& normal) {
            vec3 inUnitSphere = randomInUnitSphere();
            return inUnitSphere.sameDirection(normal) ? inUnitSphere : -inUnitSphere;
        }

        bool nearZero() const {
            const auto limit = 1e-8;
            return fabs(p[0]) < limit && fabs(p[1]) < limit && fabs(p[2]) < limit;
        }


        vec3 reflect(const vec3& normal) const;

        vec3 refract(const vec3& normal, double etaiOverEtat);

};

inline vec3 operator*(double t, const vec3 &v) {
    return vec3(v[0] * t, v[1] * t, v[2] * t);
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return vec3(u.p[0] + v.p[0], u.p[1] + v.p[1], u.p[2] + v.p[2]);
}

inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return vec3(u.p[0] - v.p[0], u.p[1] - v.p[1], u.p[2] - v.p[2]);
}

vec3 vec3::reflect(const vec3& normal) const {
    return *this - 2 * (dot(normal) * normal);
}

vec3 vec3::refract(const vec3& normal, double etaiOverEtat) {
    double cosTheta = fmin(-(*this).dot(normal), 1.0);
    vec3 rOutPerp = etaiOverEtat * ((*this) + cosTheta * normal);
    vec3 rOutParal = -sqrt(fabs(1.0 - rOutPerp.lengthSquared())) * normal;
    return rOutPerp + rOutParal;
}


using p3 = vec3;
using v3 = vec3;
using color = vec3;
#endif