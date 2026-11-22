//
// Created by Thomas Cruveilher on 2026/11/22.
//

#ifndef VECTOR3_TEST_SPHERE_H
#define VECTOR3_TEST_SPHERE_H

#include <complex>

#include "../vector/Vector3.h"
#include "../ray/Ray.h"

struct Color {
    unsigned char r = 0, g = 0, b = 0, a = 0;
};

class Sphere {
public:
    Vector3 origin = {0, 0, 0};
    double radius = 0;
    Color color = {0, 0, 0};

    Sphere(const Vector3 &origin, const double &radius) : origin(origin), radius(radius) {
    };

    Sphere(const Vector3 &origin, const double &radius, const Color &color) : origin(origin), radius(radius),
                                                                              color(color) {
    };

    bool doesIntersect(const Ray *ray) const {
        const double b = 2.0 * Vector3::dotProduct(ray->direction, (ray->origin - this->origin));
        const double c = Vector3::dotProduct((ray->origin - this->origin), (ray->origin - this->origin)) - std::pow(
                             this->radius, 2);
        const double delta = std::pow(b, 2) - 4 * c;

        if (delta < 0)
            return false;

        const double sqrtDelta = std::sqrt(delta);
        const double nearestHit = (-b - sqrtDelta) / 2.0; // nearest hit
        const double farthestHit = (-b + sqrtDelta) / 2.0; // farthest hit

        // Both negative: the sphere is behind the ray
        if (farthestHit < 0)
            return false;

        const double closestHitInFront = nearestHit >= 0 ? nearestHit : farthestHit;

        if (closestHitInFront > ray->intersection.distance) {
            return false;
        }

        ray->intersection.distance = closestHitInFront;
        ray->intersection.position = ray->origin + ray->direction * ray->intersection.distance;
        ray->intersection.normal = Vector3::normalize(ray->intersection.position - this->origin);

        return true;
    }
};


#endif //VECTOR3_TEST_SPHERE_H
