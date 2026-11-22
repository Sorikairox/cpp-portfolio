//
// Created by Thomas Cruveilher on 2026/11/22.
//

#ifndef VECTOR3_TEST_SPHERE_H
#define VECTOR3_TEST_SPHERE_H

#include <complex>

#include "../vector/Vector3.h"
#include "../ray/Ray.h"

class Intersection {
    public:
        Vector3 intersectionPoint = {0, 0, 0};
};

struct Color {
    unsigned char r = 0, g = 0, b = 0, a = 0;
};

class Sphere {
    public:

    Vector3 origin = {0, 0, 0};
    double radius = 0;
    Color color = {0, 0, 0};

    Sphere(const Vector3 &origin, const double &radius): origin(origin), radius(radius) {};

    Sphere(const Vector3 &origin, const double &radius, const Color &color): origin(origin), radius(radius), color(color) {};

    bool doesIntersect(Ray &ray) const {
        const double b = 2.0 * Vector3::dotProduct(ray.direction, (ray.origin - this->origin));
        const double c = Vector3::dotProduct((ray.origin - this->origin), (ray.origin - this->origin)) - std::pow(this->radius, 2);
        const double delta = std::pow(b, 2) - 4 * c;

        if (delta < 0)
            return false;

        return true;
    };
};


#endif //VECTOR3_TEST_SPHERE_H
