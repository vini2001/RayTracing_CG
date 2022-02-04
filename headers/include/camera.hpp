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

        // vFov stands for vertical field of view, in degrees
        Camera(p3 lookFrom, p3 lookAt, vec3 vUp,
                 double vFov, double aspectRatio
        ) : aspectRatio(aspectRatio) {
            double theta = degreesToRadians(vFov);
            double h = tan(theta/2);
            double viewportHeight = 2.0 * h;
            double viewportWidth = aspectRatio * viewportHeight;

            vec3 w = (lookFrom - lookAt).normalize();
            vec3 u = vec3::cross(vUp, w).normalize();
            vec3 v = vec3::cross(w, u);

            double focalLength = 1.0; // distance between the projection plane and the projection point, not the same as focal distance

            origin = lookFrom;
            horizontalAxis = viewportWidth * u;
            verticalAxis = viewportHeight * v;
            lowerLeftCorner = origin - horizontalAxis/2 - verticalAxis/2 - w;
        }

        Ray getRay(double u, double v) const {
            return Ray(origin, lowerLeftCorner + u * horizontalAxis + v * verticalAxis - origin);
        }
};

#endif // !CAMERA_HPP