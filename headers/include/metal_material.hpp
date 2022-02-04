#ifndef METAL_HPP
#define METAL_HPP

class MetalMaterial : public Material {
    public:
        color col;
        MetalMaterial(const color& col) : col(col) {} 

        virtual bool scatter(const Ray& rIn, const HitRecord& rec, color& attenuation, Ray& scattered) const override {
            auto reflected = rIn.direction().normalize().reflect(rec.normal);
            scattered = Ray(rec.p, reflected);
            attenuation = col;
            return (scattered.direction().sameDirection(rec.normal));
        }
};

#endif // !METAL_HPP