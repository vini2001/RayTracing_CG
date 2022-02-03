#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "vec3.hpp"
#include "ray.hpp"
#include "hittable.hpp"

class Sphere : public Hittable {
    public:
        p3 center;
        double radius;

        Sphere(p3 center, double radius) : center(center), radius(radius) {};

        // check ray colision with sphere
        // this function considers the vectorized sphere equation and solve it for t, t is the multiplier of the direction on the Ray's formula.
        bool hit(const Ray& r, double tMin, double tMax, HitRecord &rec) const override;
};


bool Sphere::hit(const Ray &r, double tMin, double tMax, HitRecord &rec) const {
    p3 oc = r.origin() - center;
    auto a = r.direction().lengthSquared();
    auto halfB = oc.dot(r.direction());
    auto c = oc.lengthSquared() - radius * radius;

    auto discriminant = halfB * halfB - a * c;
    if(discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);
    
    double root = (-halfB - sqrt(discriminant)) / a;
    // if root is out of valid range, try the second root
    if(root < tMin || root > tMax) {
        root = (-halfB + sqrt(discriminant)) / a;
        if(root < tMin || root > tMax) return false;
    }

    // save record of sphere hit with its normal
    rec.t = root;
    rec.p = r.at(root);
    v3 outwardNormal = (rec.p - center) / radius;
    rec.rayComingFromOutside = !outwardNormal.sameDirection(r.direction());
    rec.normal = rec.rayComingFromOutside ? outwardNormal : -outwardNormal;
    return true;
}
#endif // !SPHERE_HPP