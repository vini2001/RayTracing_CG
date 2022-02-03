#ifndef CAMERA_HPP
#define CAMERA_HPP
#include "common.hpp"

class Camera {
    private:
        p3 origin;
        v3 horizontalAxis, verticalAxis;
        p3 lowerLeftCorner;
        double aspectRatio;

    public:
    
        Camera(double aspectRatio) : aspectRatio(aspectRatio) {
            double viewportHeight = 2.0;
            double viewportWidth = aspectRatio * viewportHeight;
            double focalLength = 1.0; // distance between the projection plane and the projection point, not the same as focal distance

            origin = p3(0, 0, 0);
            horizontalAxis = v3(viewportWidth, 0, 0);
            verticalAxis = v3(0, viewportHeight, 0);
            lowerLeftCorner = origin - horizontalAxis/2 - verticalAxis/2 - v3(0, 0, focalLength);
        }

        Ray getRay(double u, double v) const {
            return Ray(origin, lowerLeftCorner + u * horizontalAxis + v * verticalAxis - origin);
        }
};

#endif // !CAMERA_HPP