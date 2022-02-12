#ifndef GENERIC_MATERIAL_HPP
#define GENERIC_MATERIAL_HPP

#include "dialectric_material.hpp"
#include "lambertian_material.hpp"
#include "metal_material.hpp"

/* Make a mix of Lambertian, Metal and Dialectric */
class GenericMaterial : public Material {
    public:
        double reflectionCoefficient;
        double refractionCoefficient;
        double indexOfrefraction;
        TexturePtr col = nullptr;

        GenericMaterial(double reflectionCoefficient, double refractionCoefficient, double indexOfrefraction) : reflectionCoefficient(reflectionCoefficient), refractionCoefficient(refractionCoefficient), indexOfrefraction(indexOfrefraction), col(make_shared<SolidColor>(color(1.0, 1.0, 1.0))) {}
        GenericMaterial(double reflectionCoefficient, double refractionCoefficient, double indexOfrefraction, TexturePtr col) :  reflectionCoefficient(reflectionCoefficient), refractionCoefficient(refractionCoefficient), indexOfrefraction(indexOfrefraction), col(col) {}

        GenericMaterial(GenericMaterial &m) : reflectionCoefficient(m.reflectionCoefficient), refractionCoefficient(m.refractionCoefficient), indexOfrefraction(m.indexOfrefraction), col(m.col) {}

        virtual bool scatter(const Ray& rIn, const HitRecord& rec, color& attenuation, Ray& scattered, bool &isLight) const override {
            double randomCoefficient = randomDouble();
            if(randomCoefficient < reflectionCoefficient) {
                double fuzz = 0.0;
                MaterialPtr matPtr = make_shared<MetalMaterial>(col, fuzz);
                return matPtr->scatter(rIn, rec, attenuation, scattered, isLight);
            }else if(randomCoefficient < reflectionCoefficient + refractionCoefficient) {
                MaterialPtr matPtr = make_shared<DialectricMaterial>(indexOfrefraction);
                return matPtr->scatter(rIn, rec, attenuation, scattered, isLight);
            }else{
                MaterialPtr matPtr = make_shared<LambertianMaterial>(col);
                return matPtr->scatter(rIn, rec, attenuation, scattered, isLight);
            }
            return true;
        }
};

typedef shared_ptr<GenericMaterial> GenericMaterialPtr;


#endif // !GENERIC_MATERIAL_HPP