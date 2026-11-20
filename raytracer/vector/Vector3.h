//
// Created by Thomas Cruveilher on 2026/11/20.
//

#ifndef UNTITLED1_VECTOR3_H
#define UNTITLED1_VECTOR3_H

#include <memory>

class Vector3 {
public:
    double x = 0;
    double y = 0;
    double z = 0;

    Vector3() = default;
    Vector3(double posX, double posY, double posZ) : x(posX), y(posY), z(posZ) {
    };

    ~Vector3() = default;

    Vector3 operator+(const Vector3 &other) const {
        return {x + other.x, y + other.y, z + other.z};
    }

    [[nodiscard]] double length() const {
        return sqrt(x * x + y * y + z * z);
    }

    Vector3 operator*(const double scaler) const {
        return {this->x * scaler, this->y * scaler, this->z * scaler};
    }

    Vector3 operator-(const Vector3 & target) const {
        return {x - target.x, y - target.y, z - target.z};
    }

    [[nodiscard]] static Vector3 normalize(const std::unique_ptr<Vector3> & a) {
        const double normalizedX = a->x / a->length();
        const double normalizedY = a->y / a->length();
        const double normalixedZ = a->z / a->length();
        return {normalizedX, normalizedY, normalixedZ};
    };;

    static double dotProduct(const std::unique_ptr<Vector3> & a, const std::unique_ptr<Vector3> & b) {
        return a->x * b->x + a->y * b->y + a->z * b->z;
    }

    static Vector3 cross(const std::unique_ptr<Vector3> & a, const std::unique_ptr<Vector3> & b) {
        const double z = a->x * b->y - (b->x * a -> y);
        const double y = b->x * a-> z - a->x * b->z;
        const double x = a->y * b->z - b->y * a->z;

        return {x, y, z};
    }

    bool operator==(const Vector3 &other) const {
        return x == other.x && y == other.y && z == other.z;
    };
};


#endif //UNTITLED1_VECTOR3_H
