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
#include <thread>
#include <fstream>
#include "light.hpp"

typedef shared_ptr<Material> MaterialPtr;

using namespace std;

char* inputFileName;
char* outputFileName;

color **img;
const int imgWidth = 600;
const auto aspectRatio = 16.0 / 9.0;
const int imgHeight = imgWidth / aspectRatio;
const int samplesPerPixel = 100;//00;
const int maxDepth = 50;

int remainingRows = imgHeight;

ComponentList componentList;
p3 lookFrom(13, 2, 3);
p3 lookAt(0, 0, 0);
vec3 vUp = v3(0, 1, 0);
double vFov = 50;
double distToFocus = 10.0; //(lookFrom-lookAt).length();
auto aperture = 0.00;
Camera camera;

color rayColor(const Ray& r, const ComponentList& componentList, int maxDepth, color bg) {

    // ray bounce limit exceeded
    if(maxDepth-- <= 0)
        return color(0, 0, 0);

    HitRecord hr;
    if(componentList.hit(r, 0.001, infinity, hr)) {
        Ray scattered;
        color attenuation;
        bool isLight = false;
        if(hr.matPtr->scatter(r, hr, attenuation, scattered, isLight)) {
            vec3 target = rayColor(scattered, componentList, maxDepth, bg);
            return attenuation * target;
        }else{
            if(isLight) return attenuation;
            return color(0, 0, 0);
        }
    }else{
        return bg;
    }
}

void printRemaining() {
    cerr << "\rScanlines remaining: " << remainingRows << ' ' << flush;
}

ComponentList randomScene(int extraBalls, float size) {
    int rowxcol = sqrt(extraBalls)/2;
    float mult = 11.0 / rowxcol;
    ComponentList componentList;

    TexturePtr earth = make_shared<ImageTexture>("textures/earth.jpeg");
    MaterialPtr earthMaterial = make_shared<LambertianMaterial>(earth);
    MaterialPtr lightMaterial = make_shared<LightMaterial>(color(1, 1, 1), 4);
    MaterialPtr earthDialectricMaterial = make_shared<DialectricMaterial>(0.005, earth);

    CheckerTexturePtr checker = make_shared<CheckerTexture>(color(0.007, 0.1568, 0.32115), color(0.7, 0.7, 0.7));
    checker->setScale(0.7);
    MaterialPtr groundMaterial = make_shared<LambertianMaterial>(checker);
    componentList.add(make_shared<Sphere>(v3(0, -1000, 0), 1000, groundMaterial));

    for(int a = -rowxcol; a < rowxcol; a++) {
        for(int b = -rowxcol; b < rowxcol; b++) {
            double chooseMat = randomDouble();

            p3 center(mult*a + mult*0.9*randomDouble(), 0.2, mult*b + mult*0.9*randomDouble());

            if((center - p3(4, 0.2, 0)).length() > 0.9) {
                if(chooseMat < 0.8) {
                    // diffuse
                    MaterialPtr matPtr = make_shared<LambertianMaterial>(color::random() * color::random());
                    componentList.add(make_shared<Sphere>(center, size, matPtr));
                } else if(chooseMat < 0.95) {
                    // metal
                    double fuzz = randomDouble(0, 0.5);
                    color col = color::random(0.5, 1.0);
                    MaterialPtr matPtr = make_shared<MetalMaterial>(col, fuzz);
                    componentList.add(make_shared<Sphere>(center, size, matPtr));
                } else {
                    MaterialPtr matPtr = make_shared<DialectricMaterial>(1.5);
                    componentList.add(make_shared<Sphere>(center, size, matPtr));
                }
            }
        }
    }

    MaterialPtr mat1 = make_shared<DialectricMaterial>(1.5);
    componentList.add(make_shared<Sphere>(v3(0, 1, 0), 1.0, mat1));

    MaterialPtr mat2 = make_shared<LambertianMaterial>(color(0.4, 0.2, 0.1));
    componentList.add(make_shared<Sphere>(v3(-4, 1, 0), 1.0, mat2));

    MaterialPtr mat3 = make_shared<MetalMaterial>(color(0.7, 0.6, 0.5), 0.0);
    componentList.add(make_shared<Sphere>(v3(0, 4, 0), 2.0, earthDialectricMaterial));
    // componentList.add(make_shared<Sphere>(v3(0, 5, 0), 2.5, mat3));

    componentList.add(make_shared<Sphere>(v3(-16, 30, 0), 12.5, lightMaterial));

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

                // color background = color(0.62, 0.72, 0.96);
                color background = color(0.01, 0.01, 0.01);
                pixelColor += rayColor(r, componentList, maxDepth, background);
            }
            img[row][col] += pixelColor;
        }
        remainingRows --;
        printRemaining();
    }
}

bool validateArgs(int argc, char** argv) {
    if(argc < 3) {
        cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << endl;
        return false;
    }
    inputFileName = argv[1];
    outputFileName = argv[2];

    // if outputfile doesn't have .ppm extension, add it
    if(strcmp(outputFileName + strlen(outputFileName) - 4, ".ppm") != 0) {
        char* newOutputFile = new char[strlen(outputFileName) + 5];
        strcpy(newOutputFile, outputFileName);
        strcat(newOutputFile, ".ppm");
        outputFileName = newOutputFile;
    }
    return true;
}

void processInputFile(ifstream &inputFile) {
    string line;

    // Camera Inputs
    getline(inputFile, line);
    lookFrom = p3(line);

    getline(inputFile, line);
    lookAt = p3(line);

    getline(inputFile, line);
    vUp = p3(line);
    
    getline(inputFile, line);
    vFov = stod(line);

    camera = Camera(lookFrom, lookAt, vUp, vFov, aspectRatio, aperture, distToFocus);

    // Lights Inputs
    getline(inputFile, line);
    int numLights = stoi(line);

    for(int i = 0; i < numLights; i++) {
        getline(inputFile, line);
        vector<string> lightDetails = split(line);
        p3 lightPos = p3(stod(lightDetails[0]), stod(lightDetails[1]), stod(lightDetails[2]));
        p3 lightColor = p3(stod(lightDetails[3]), stod(lightDetails[4]), stod(lightDetails[5]));
        double attenuationConstant = stod(lightDetails[6]);
        double attenuationLinear = stod(lightDetails[7]); // atennuation proportional to distance from light
        double attenuationQuadratic = stod(lightDetails[8]); // atennuation proportional to distance from light squared
    }
    
    // TODO
    // Pigments Inputs

    // TODO
    // Materials Inputs

    // TODO
    // Objects Inputs
    


    while (getline(inputFile, line)){
        cout << line << endl;
    }
}

int main(int argc, char** argv) {

    if(!validateArgs(argc, argv))
        return -1;

    // create file
    ofstream outputFile;
    ifstream inputFile(inputFileName);
    outputFile.open(outputFileName);

    if (inputFile.is_open()) {
        processInputFile(inputFile);
        inputFile.close();
    }else{
        cout << "Unable to open file" << endl;
        inputFile.close();
        return -1;
    }


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
    componentList = randomScene(16, 0.5);

    // Render
    outputFile << "P3\n" << imgWidth << " " << imgHeight << "\n255\n";

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
            outputColor(outputFile, img[row][col], samplesPerPixel);
        }
    }
    cerr << "\nDone.\n";

    outputFile.close();
    return 0;
}