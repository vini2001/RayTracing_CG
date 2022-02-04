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
#include<thread>

typedef shared_ptr<Material> MaterialPtr;

using namespace std;

color **img;
const int imgWidth = 1500;
const auto aspectRatio = 3.0 / 2.0;
const int imgHeight = imgWidth / aspectRatio;
const int samplesPerPixel = 500;
const int maxDepth = 50;

int remainingRows = imgHeight;

ComponentList componentList;
p3 lookFrom(13, 2, 3);
p3 lookAt(0, 0, 0);
double distToFocus = 10.0; //(lookFrom-lookAt).length();
auto aperture = 0.1;
Camera camera(lookFrom, lookAt, v3(0, 1, 0), 40, aspectRatio, aperture, distToFocus);

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

void printRemaining() {
    cerr << "\rScanlines remaining: " << remainingRows << ' ' << flush;
}

ComponentList randomScene() {
    ComponentList componentList;

    MaterialPtr groundMaterial = make_shared<LambertianMaterial>(color(0.5, 0.5, 0.5));
    componentList.add(make_shared<Sphere>(v3(0, -1000, 0), 1000, groundMaterial));

    for(int a = -11; a < 11; a++) {
        for(int b = -11; b < 11; b++) {
            double chooseMat = randomDouble();

            p3 center(a + 0.9*randomDouble(), 0.2, b + 0.9*randomDouble());

            if((center - p3(4, 0.2, 0)).length() > 0.9) {
                if(chooseMat < 0.8) {
                    // diffuse
                    MaterialPtr matPtr = make_shared<LambertianMaterial>(color::random() * color::random());
                    componentList.add(make_shared<Sphere>(center, 0.2, matPtr));
                } else if(chooseMat < 0.95) {
                    // metal
                    double fuzz = randomDouble(0, 0.5);
                    color col = color::random(0.5, 1.0);
                    MaterialPtr matPtr = make_shared<MetalMaterial>(col, fuzz);
                    componentList.add(make_shared<Sphere>(center, 0.2, matPtr));
                } else {
                    MaterialPtr matPtr = make_shared<DialectricMaterial>(1.5);
                    componentList.add(make_shared<Sphere>(center, 0.2, matPtr));
                }
            }
        }
    }

    MaterialPtr mat1 = make_shared<DialectricMaterial>(1.5);
    componentList.add(make_shared<Sphere>(v3(0, 1, 0), 1.0, mat1));

    MaterialPtr mat2 = make_shared<LambertianMaterial>(color(0.4, 0.2, 0.1));
    componentList.add(make_shared<Sphere>(v3(-4, 1, 0), 1.0, mat2));

    MaterialPtr mat3 = make_shared<MetalMaterial>(color(0.7, 0.6, 0.5), 0.0);
    componentList.add(make_shared<Sphere>(v3(4, 1, 0), 1.0, mat3));

    return componentList;
}

void computeFor(int rowFrom, int rowTo){
    for(int row = rowFrom; row <= rowTo; row++) {
        for(int col = 0; col < imgWidth; ++col) {
            color pixelColor(0, 0, 0);
            for(int s = 0; s < samplesPerPixel; ++s) {
                auto u = double(col + randomDouble()) / (imgWidth-1);
                auto v = double(row + randomDouble()) / (imgHeight-1);
                // One ray for each pixel of the projection plane
                Ray r = camera.getRay(u, v);
                pixelColor += rayColor(r, componentList, maxDepth);
            }
            img[row][col] += pixelColor;
        }
        remainingRows --;
        printRemaining();
    }
}

int main() {

    img = new color*[imgHeight];
    for(int i = 0; i < imgHeight; i++) {
        img[i] = new color[imgWidth];
        for(int j = 0; j < imgWidth; j++) {
            img[i][j] = color(0, 0, 0);
        }
    }

    MaterialPtr diffuseGround = make_shared<LambertianMaterial>(color(0.27, 0.16, 0.1));
    MaterialPtr front = make_shared<LambertianMaterial>(color(0.3, 0.3, 0.7));
    MaterialPtr sides = make_shared<MetalMaterial>(color(0.8, 0.8, 0.8), 0.00);
    MaterialPtr top = make_shared<MetalMaterial>(color(0.14, 0.23, 0.8), 0.3);
    MaterialPtr glass = make_shared<DialectricMaterial>(1.5);

    // Components
    componentList = randomScene();

    // Render
    cout << "P3\n" << imgWidth << " " << imgHeight << "\n255\n";

    vector<thread*> threads;
    
    printRemaining();
    int batchSize = imgHeight / 20;
    for(int row = imgHeight; row >= 0; row -= batchSize) {
        int from = row-batchSize < 0 ? 0 : row-batchSize;
        int to = row - 1 < 0 ? 0 : row - 1;

        // cerr << from << " - " << to << endl;

        thread *th = new thread(computeFor, from, to);
        threads.push_back(th);
    }

    // join all threads
    for(auto th : threads) {
        th->join();
    }

    for(int row = imgHeight-1; row >= 0; --row) {
        for(int col = 0; col < imgWidth; ++col) {
            outputColor(img[row][col], samplesPerPixel);
        }
    }
    cerr << "\nDone.\n";
    return 0;
}