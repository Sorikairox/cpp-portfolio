//
// Created by Cruveilher Thomas on 2026/11/24.
//

#ifndef VECTOR3_TEST_ENTITY_H
#define VECTOR3_TEST_ENTITY_H
#include "Color.h"
#include "ray/Ray.h"

class Material {
public:
    double reflectivity = 0.5;
    Color color;
};

class Entity {
public:
    explicit Entity(const Vector3 &origin): origin{origin} {};
    virtual ~Entity() = default;

    void attachMaterial(Material *m) {
        this->material = m;
    }

    Vector3 origin;
    bool doesIntersect(Ray *ray) const;
    Material *material;
};

#endif //VECTOR3_TEST_ENTITY_H
