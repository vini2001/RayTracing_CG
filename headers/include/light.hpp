#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "common.hpp"



class LightMaterial : public Material {
    public:
        TexturePtr col;
        double lightIntensity;

        LightMaterial(const color& col, double lightIntensity) : col(make_shared<SolidColor>(col)), lightIntensity(lightIntensity)  {}
        LightMaterial(const TexturePtr& col, double lightIntensity) : col(col), lightIntensity(lightIntensity) {}

        virtual bool scatter(const Ray& rIn, const HitRecord& hr, color& attenuation, Ray& scattered, bool &isLight) const override {
            // Don't reflect any rays
            isLight = true;
            attenuation = lightIntensity * col->value(hr.uv, hr.p);
            return false;
        }
};

#endif // !LIGHT_HPP