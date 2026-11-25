//
// Created by Thomas Cruveilher on 2026/11/22.
//

#ifndef VECTOR3_TEST_SPHERE_H
#define VECTOR3_TEST_SPHERE_H

#include <complex>

#include "../Entity.h"
#include "../vector/Vector3.h"
#include "../ray/Ray.h"

class Sphere: public Entity {
public:
    double radius = 0;

    Sphere(const Vector3 &origin, const double &radius) : Entity{origin}, radius(radius) {
    };

    Sphere(const Vector3 &origin, const double &radius, const Color &color) : Entity{origin, color}, radius(radius) {};

    virtual bool doesIntersect(const Ray *ray) const {
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
        if (farthestHit < 1e-4)
            return false;

        const double closestHitInFront = nearestHit >= 0 ? nearestHit : farthestHit;

        if (closestHitInFront > ray->intersection.distance) {
            return false;
        }

        ray->intersection.distance = closestHitInFront;
        ray->intersection.position = ray->origin + ray->direction * ray->intersection.distance;
        // N
        ray->intersection.surfaceNormal = Vector3::normalize(ray->intersection.position - this->origin);
        return true;
    }
};


#endif //VECTOR3_TEST_SPHERE_H
