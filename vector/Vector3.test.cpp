//
// Created by Thomas Cruveilher on 2026/11/20.
//

#include <gtest/gtest.h>

#include "Vector3.h"

TEST(Vector3, SimpleAddition) {
    const auto a = std::make_unique<Vector3>(1, 2, 3);
    const auto b = std::make_unique<Vector3>(4, 5, 6);
    EXPECT_EQ(*a + *b, Vector3(5, 7, 9));
}

TEST(Vector3, CrossProduct) {
    const auto c = std::make_unique<Vector3>(1, 2, 3);
    const auto d = std::make_unique<Vector3>(4, 5, 6);
    // expect cross product vector
    EXPECT_EQ(Vector3::cross(c, d), Vector3(-3, 6, -3));
}

TEST(Vector3, DotProduct) {
    const auto a = std::make_unique<Vector3>(1, 2, 3);
    const auto b = std::make_unique<Vector3>(4, 5, 6);
    EXPECT_EQ(Vector3::dotProduct(a, b), 4 + 10 + 18);
}

TEST(Vector3, Length) {
    const auto a = std::make_unique<Vector3>(4, 3 , 0);
    EXPECT_EQ(a->length(), 5);
}

TEST(Vector3, ScalarMultiply) {
    const auto a = std::make_unique<Vector3>(1, 2, 3);
    EXPECT_EQ(*(a.get()) * 2.0, Vector3(2, 4, 6));
}

TEST(Vector3, Normalize) {
    auto a = std::make_unique<Vector3>(10, 0, 0);
    EXPECT_EQ(Vector3::normalize(a), Vector3(1, 0, 0));

    a= std::make_unique<Vector3>(10, 10, 10);
    const auto normalized = Vector3::normalize(a);
    EXPECT_NEAR(normalized.x, 0.58, 2);
    EXPECT_NEAR(normalized.y, 0.58, 2);
    EXPECT_NEAR(normalized.z, 0.58, 2);
}