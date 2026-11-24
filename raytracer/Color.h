//
// Created by Cruveilher Thomas on 2026/11/24.
//

#ifndef VECTOR3_TEST_COLOR_H
#define VECTOR3_TEST_COLOR_H
#include "vector/Vector3.h"

struct Color {
    unsigned char r = 0;
    unsigned char g = 0;
    unsigned char b = 0;
    unsigned char a = 0;

    [[nodiscard]] Vector3 albedoMultiplication(const Color &other) const {
        const Vector3 thisAlbedo = {
            static_cast<double>(this->r) / 255, static_cast<double>(this->g) / 255, static_cast<double>(this->b) / 255
        };

        const Vector3 otherAlbedo = {
            static_cast<double> (other.r) / 255,
            static_cast<double> (other.g) / 255,
            static_cast<double> (other.b) / 255
        };

        return thisAlbedo * otherAlbedo;
    }
};

#endif //VECTOR3_TEST_COLOR_H
