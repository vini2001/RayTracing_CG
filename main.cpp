#include <iostream>
#include "color.hpp"
#include "vec3.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "common.hpp"
#include "component_list.hpp"
#include "camera.hpp"

using namespace std;

color rayColor(const Ray& r, const ComponentList& componentList) {
    Sphere s(p3(0, 0, -1), 0.5);
    HitRecord hr;
    if(componentList.hit(r, 0, 10000000, hr)) {
        v3 N = hr.normal;
        return 0.5 * (N + color(1, 1, 1));
    }

    v3 unitDirection = r.direction().normalize();
    auto t = 0.5 * (unitDirection.y() + 1);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {

    const auto aspectRatio = 16.0 / 9.0;
    const int imgWidth = 1200;
    const int imgHeight = imgWidth / aspectRatio;
    const int samplesPerPixel = 10;

    // Components
    ComponentList componentList;
    componentList.add(make_shared<Sphere>(Sphere(p3(0, 0, -1), 0.5)));
    componentList.add(make_shared<Sphere>(Sphere(p3(-0.5, 0, -1.3), 0.5)));
    componentList.add(make_shared<Sphere>(Sphere(p3(0.5, 0, -1.3), 0.5)));
    componentList.add(make_shared<Sphere>(Sphere(p3(0, 0.35, -0.6), 0.15)));
    componentList.add(make_shared<Sphere>(Sphere(p3(0, -100.5, -1), 100)));

    Camera camera(aspectRatio);

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
                pixelColor += rayColor(r, componentList);
            }
            outputColor(pixelColor, samplesPerPixel);
        }
    }


    cerr << "\nDone.\n";
    return 0;
}