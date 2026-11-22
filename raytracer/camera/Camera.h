//
// Created by Thomas Cruveilher on 2026/11/20.
//

#ifndef VECTOR3_TEST_CAMERA_H
#define VECTOR3_TEST_CAMERA_H

#include <cmath>
#include <numbers>

#include "../vector/Vector2.h"
#include "../vector/Vector3.h"

class Camera {
    Vector3 target;
    Vector3 forward;
    Vector3 right;
    Vector3 up;
    double fovInDegrees = 60;

    Vector3 topLeftCorner;
    Vector3 pixelDirectionX;
    Vector3 pixelDirectionY;

public:
    Vector3 position;
    Vector2 screenDimension = {1920, 1080};
    Vector3 &forwardVector() {
        return forward;
    }

    Vector3 &rightVector() {
        return right;
    }

    Vector3 &upVector() {
        return up;
    }

    Camera(const Vector3 &pos, const Vector3 &tar) : position(pos), target(tar) {
        forward = Vector3::normalize(target - pos);
        if (Vector3::normalize(forward) == Vector3{0, 0, 1}) {
            right = Vector3::normalize(Vector3::cross(forward, Vector3{0, 1, 0}));
        } else {
            right = Vector3::normalize(Vector3::cross(forward, Vector3{0, 0, 1}));
        }
        up = Vector3::normalize(Vector3::cross(right, forward));
        Vector2 halfSize = this->getHalfSizes();
        //direction = position + forward
        // - right * halfwidth + up * halfHeight
        topLeftCorner = this->position + this->forward - this->right * halfSize.x + this->up * halfSize.y;

        // right * halfWidth * 2 / width * (i + 0.5)
        pixelDirectionX = this->right * (halfSize.x / this->screenDimension.x) * 2;
        // up * halfHeight * 2 / height *
        pixelDirectionY = (this->up * (halfSize.y * 2 / this->screenDimension.y) * -1) ;
    };

    [[nodiscard]] Vector2 getHalfSizes() const {
        const double halfHeight = std::tan(this->fovInDegrees *     std::numbers::pi / 180 / 2);
        const double aspect = this->screenDimension.x / this->screenDimension.y;
        const double halfWidth = halfHeight * aspect;
        return {halfWidth, halfHeight};
    }


    [[nodiscard]] Vector3 getScreenPixelVector(const double &screenX, const double &screenY) const {
        return Vector3::normalize(
            this->topLeftCorner + this->pixelDirectionX * (screenX + 0.5) + this->pixelDirectionY * (screenY + 0.5) - position);
    };

    // u = i + 0.5 / width
    // v = j + 0.5 / height
    // sx = 2u - 1
    // sy = 1 - 2v
    // x = sx * halfWidth
    // y = sx * halfHeight
    // direction = position + forward + right * x + up * y

    // direction = position + forward
    // + right * (halfWith * (2 ((i+0.5)/width) - 1))
    //                                + up * (halfHeight * (1 - 2((j + 0.5) / height)))


    // right * (halfWidth * (2 ((i+0.5)/width) - 1))
    // right * (halfWidth * (2A - 1))
    // right * ((halfWidth * 2A) - halfWidth)
    // right * (halfWidth * 2A) - right * halfWidth
    // right * halfWidth * 2 * (i + 0.5) / width - right * halfWidth
    // right * halfWidth * 2 / width * (i + 0.5) - right * halfwidth

    // up * (halfHeight * (1 - 2B))
    // up * (halfHeight - halfHeight * 2B)
    // up * halfHeight - up * halfHeight * 2B
    // up * halfHeight - up * halfHeight * 2 * ((j + 0.5) / height)
    // up * halfHeight - up * halfHeight * 2 * (j + 0.5) / height
    // up * halfHeight - up * halfHeight * 2 / height * (j + 0.5)


    // direction = position + forward
    // - right * halfwidth + up * halfHeight
    // + right * halfWidth * 2 / width * (i + 0.5)
    // - up * halfHeight * 2 / height * (j + 0.5)
};

#endif //VECTOR3_TEST_CAMERA_H
