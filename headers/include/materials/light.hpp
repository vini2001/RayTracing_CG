#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "common.hpp"



class LightMaterial : public Material {
    public:
        TexturePtr col;
        double lightAttenuationConstant, lightAttenuationLinear, lightAttenuationQuadratic;

        LightMaterial(const color& col, double lightAttenuationConstant, double lightAttenuationLinear, double lightAttenuationQuadratic, bool ghostMaterial) : col(make_shared<SolidColor>(col)), lightAttenuationConstant(lightAttenuationConstant), lightAttenuationLinear(lightAttenuationLinear), lightAttenuationQuadratic(lightAttenuationQuadratic) {
            this->ghostMaterial = ghostMaterial;
        }
        LightMaterial(const TexturePtr& col, double lightAttenuationConstant, double lightAttenuationLinear, double lightAttenuationQuadratic, bool ghostMaterial) : col(col), lightAttenuationConstant(lightAttenuationConstant), lightAttenuationLinear(lightAttenuationLinear), lightAttenuationQuadratic(lightAttenuationQuadratic) {
            this->ghostMaterial = ghostMaterial;
        }

        virtual bool scatter(const Ray& rIn, const HitRecord& hr, color& attenuation, Ray& scattered, bool &isLight) const override {
            // Don't reflect any rays
            isLight = true;
            attenuation = lightAttenuationConstant * col->value(hr.uv, hr.p);
            return false;
        }
};

#endif // !LIGHT_HPP