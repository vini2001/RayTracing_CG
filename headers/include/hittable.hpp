#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include "ray.hpp"
#include "common.hpp"
#include "material.hpp"

struct HitRecord {
    p3 p;
    v3 normal;
    double t;
    bool rayComingFromOutside;
    shared_ptr<Material> matPtr;
};

class Hittable {
    public:
        virtual bool hit(const Ray& r, double tMin, double tMax, HitRecord &rec) const = 0;
};

#endif // !HITTABLE_HPP