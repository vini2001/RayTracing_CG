#ifndef METAL_HPP
#define METAL_HPP

class MetalMaterial : public Material {
    public:
        color col;
        double fuziness;
        MetalMaterial(const color& col, double fuziness) : col(col), fuziness(fuziness < 1.0 ? fuziness : 1.0) {} 

        virtual bool scatter(const Ray& rIn, const HitRecord& rec, color& attenuation, Ray& scattered, bool &isLight) const override {
            auto reflected = rIn.direction().normalize().reflect(rec.normal);
            scattered = Ray(rec.p, reflected + fuziness * vec3::randomInUnitSphere());
            attenuation = col;
            return (scattered.direction().sameDirection(rec.normal));
        }
};

#endif // !METAL_HPP