#ifndef LAMBERTIAN_MATERIAL_HPP
#define LAMBERTIAN_MATERIAL_HPP

#include "common.hpp"
#include "material.hpp"
#include "ray.hpp"

class LambertianMaterial : public Material {
    public:
        color col;
        LambertianMaterial(const color& col) : col(col) {}

        virtual bool scatter(const Ray& rIn, const HitRecord& rec, color& attenuation, Ray& scattered) const override {
            auto scatterDirection = rec.normal + vec3::randomUnitVector();

            // If the scatter direction is close to 0, NaN issues would occur.
            if(scatterDirection.nearZero()) 
                scatterDirection = rec.normal;

            scattered = Ray(rec.p, scatterDirection);
            attenuation = col;
            return true;
        }
};

#endif // !LAMBERTIAN_MATERIAL_HPP