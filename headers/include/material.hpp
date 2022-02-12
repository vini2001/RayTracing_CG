#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "common.hpp"

struct HitRecord;

class Material {
    public:
        bool ghostMaterial = false;
        virtual bool scatter(const Ray& r, const HitRecord& rec, v3& attenuation, Ray& scattered, bool &isLight) const = 0;
};

#endif // !MATERIAL_HPP