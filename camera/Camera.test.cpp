//
// Created by Thomas Cruveilher on 2026/11/20.
//

#include "Camera.h"
#include "gtest/gtest.h"
#include "../vector/Vector3.h"

TEST(Camera, ForwardVector) {
    auto camera = std::make_unique<Camera>(Vector3{0, 0, 0}, Vector3{0, 100, 0});
    EXPECT_EQ(camera->forwardVector(), Vector3(0, 100, 0));
}
