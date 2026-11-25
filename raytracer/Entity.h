//
// Created by Cruveilher Thomas on 2026/11/24.
//

#ifndef VECTOR3_TEST_ENTITY_H
#define VECTOR3_TEST_ENTITY_H
#include "Color.h"
#include "ray/Ray.h"

class Entity {
public:
    explicit Entity(const Vector3 &origin): origin{origin} {};
    Entity(const Vector3 &origin, const Color &color) : origin{origin}, color{color} {};
    virtual ~Entity() = default;

    Vector3 origin;
        Color color;
        bool doesIntersect(Ray *ray) const;
        double reflectivity = 0;
};

#endif //VECTOR3_TEST_ENTITY_H
