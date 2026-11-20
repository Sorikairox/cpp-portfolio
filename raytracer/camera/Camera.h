//
// Created by Thomas Cruveilher on 2026/11/20.
//

#ifndef VECTOR3_TEST_CAMERA_H
#define VECTOR3_TEST_CAMERA_H

#include "../vector/Vector3.h"

class Camera {

    Vector3 position;
    Vector3 target;
    Vector3 forward;
public:
    Vector3 &forwardVector() {
        return forward;
    };

    Camera(Vector3 pos, Vector3 tar) {
        position = pos;
        target = tar;
        forward = target - pos;
    };
};

#endif //VECTOR3_TEST_CAMERA_H
