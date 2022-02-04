#include <iostream>
#include "color.hpp"
#include "vec3.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "common.hpp"
#include "component_list.hpp"
#include "camera.hpp"
#include "lambertian_material.hpp"
#include "metal_material.hpp"
#include "dialectric_material.hpp"

typedef shared_ptr<Material> MaterialPtr;

using namespace std;

color rayColor(const Ray& r, const ComponentList& componentList, int maxDepth) {

    // ray bounce limit exceeded
    if(maxDepth-- <= 0)
        return color(0, 0, 0);

    HitRecord hr;
    if(componentList.hit(r, 0.001, infinity, hr)) {
        Ray scattered;
        color attenuation;
        if(hr.matPtr->scatter(r, hr, attenuation, scattered)) {
            vec3 target = rayColor(scattered, componentList, maxDepth);
            return attenuation * target;
        }
        return color(0, 0, 0);
    }

    v3 unitDirection = r.direction().normalize();
    auto t = 0.5 * (unitDirection.y() + 1);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {

    const auto aspectRatio = 16.0 / 9.0;
    const int imgWidth = 500;
    const int imgHeight = imgWidth / aspectRatio;
    const int samplesPerPixel = 100;
    const int maxDepth = 50;

    MaterialPtr diffuseGround = make_shared<LambertianMaterial>(color(0.27, 0.16, 0.1));
    MaterialPtr front = make_shared<LambertianMaterial>(color(0.3, 0.3, 0.7));
    MaterialPtr sides = make_shared<MetalMaterial>(color(0.8, 0.8, 0.8), 0.00);
    MaterialPtr top = make_shared<MetalMaterial>(color(0.14, 0.23, 0.8), 0.3);
    MaterialPtr glass = make_shared<DialectricMaterial>(1.5);

    // Components
    ComponentList componentList;
    componentList.add(make_shared<Sphere>(Sphere(p3(0, 0, -1), 0.5, front)));
    componentList.add(make_shared<Sphere>(Sphere(p3(-1.2, 0, -1.3), 0.35, sides)));
    componentList.add(make_shared<Sphere>(Sphere(p3(1.2, 0, -1.3), -0.35, glass)));
    componentList.add(make_shared<Sphere>(Sphere(p3(0, 0.35, -0.6), 0.15, top)));
    componentList.add(make_shared<Sphere>(Sphere(p3(0, -100.5, -1), 100, diffuseGround)));

    p3 lookFrom(-2,1.3,1);
    p3 lookAt(0, 0, -1);
    double distToFocus = (lookFrom-lookAt).length();
    auto aperture = 2.0;
    Camera camera(lookFrom, lookAt, v3(0, 1, 0), 40, aspectRatio, aperture, distToFocus);

    // Render
    cout << "P3\n" << imgWidth << " " << imgHeight << "\n255\n";

    for(int row = imgHeight-1; row >= 0; --row) {
        cerr << "\rScanlines remaining: " << row << ' ' << flush;
        for(int col = 0; col < imgWidth; ++col) {
            color pixelColor(0, 0, 0);
            for(int s = 0; s < samplesPerPixel; ++s) {
                auto u = double(col + randomDouble()) / (imgWidth-1);
                auto v = double(row + randomDouble()) / (imgHeight-1);
                // One ray for each pixel of the projection plane
                Ray r = camera.getRay(u, v);
                pixelColor += rayColor(r, componentList, maxDepth);
            }
            outputColor(pixelColor, samplesPerPixel);
        }
    }


    cerr << "\nDone.\n";
    return 0;
}