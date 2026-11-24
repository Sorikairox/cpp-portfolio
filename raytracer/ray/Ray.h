//
// Created by Thomas Cruveilher on 2026/11/22.
//

#ifndef VECTOR3_TEST_RAY_H
#define VECTOR3_TEST_RAY_H

#include "../vector/Vector3.h"

class Intersection {
public:
    double distance = INFINITY;
    Vector3 surfaceNormal = {0, 0, 0};
    Vector3 position = {0, 0, 0};
    Vector3 directionToLight = {0, 0, 0};
    double lightHits;
    double specularIntensity;
};

class Ray {
    public:
    Vector3 origin = {0, 0, 0};
    mutable Vector3 direction = {0, 0, 0};
    mutable Intersection intersection = {};

    explicit Ray(Vector3 origin): origin(origin) {}
    Ray(Vector3 origin, Vector3 direction) : origin(origin), direction(direction) {
        this->direction = Vector3::normalize(direction);
    }
};


#endif //VECTOR3_TEST_RAY_H
