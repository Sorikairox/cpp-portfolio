//
// Created by Cruveilher Thomas on 2026/11/24.
//

#ifndef VECTOR3_TEST_PLANE_H
#define VECTOR3_TEST_PLANE_H
#include "sphere/Sphere.h"
#include "vector/Vector3.h"


class Plane: public Entity {
public:
    Vector3 normal = {0, 0, 1};

    Plane(const Vector3 &origin, const Vector3 &normal, Color color) : Entity{origin, color}, normal(normal) {}
    bool doesIntersect(const Ray *ray) const {
        double closestDistance = (Vector3::dotProduct((this->origin - ray->origin), this->normal) / Vector3::dotProduct(ray->direction, this->normal));

        if (closestDistance <= 0 || closestDistance > ray->intersection.distance) {
            return false;
        }

        ray->intersection.distance = closestDistance;
        ray->intersection.position = ray->origin + ray->direction * closestDistance;
        ray->intersection.surfaceNormal = this->normal;

        return true;
    }
};


#endif //VECTOR3_TEST_PLANE_H
