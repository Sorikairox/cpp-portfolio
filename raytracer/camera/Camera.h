//
// Created by Thomas Cruveilher on 2026/11/20.
//

#ifndef VECTOR3_TEST_CAMERA_H
#define VECTOR3_TEST_CAMERA_H

#include <ios>

#include "../vector/Vector3.h"

class Camera {

    Vector3 position;
    Vector3 target;
    Vector3 forward;
    Vector3 right;
public:
    Vector3 &forwardVector() {
        return forward;
    }

    Vector3 &rightVector() {
        return right;
    };

    Camera(const Vector3 &pos, const Vector3 &tar): position(pos), target(tar) {
        forward = target - pos;
        if (Vector3::normalize(forward) == Vector3{0, 0, 1}) {
            right = Vector3::normalize(Vector3::cross(forward, Vector3{0, 1, 0}));
        } else {
            right = Vector3::normalize(Vector3::cross(forward, Vector3{0, 0, 1}));
        }
    };
};

#endif //VECTOR3_TEST_CAMERA_H
