//
// Created by Thomas Cruveilher on 2026/11/20.
//

#include "Camera.h"
#include "gtest/gtest.h"
#include "../vector/Vector3.h"
#include <cmath>

TEST(Camera, ForwardVector) {
    auto camera = std::make_unique<Camera>(Vector3{0, 0, 0}, Vector3{0, 100, 0});
    EXPECT_EQ(camera->forwardVector(), Vector3(0, 1, 0));
}

TEST(Camera, RightVector) {
    auto camera = Camera(Vector3{0, 0, 0}, Vector3{0, 100, 0});
    EXPECT_EQ(camera.rightVector(), Vector3(1, 0, 0));

    auto cameraLookingStraightUp = Camera{Vector3{0, 0, 0}, Vector3{0, 0, 100}};
    EXPECT_EQ(cameraLookingStraightUp.rightVector(), Vector3(-1, 0, 0));
}

TEST(Camera, UpVector) {
    auto camera = Camera(Vector3{0, 0, 0}, Vector3{0, 100, 0});
    EXPECT_EQ(camera.upVector(), Vector3(0, 0, 1));
}
static void expectVectorNear(const Vector3 &actual, const Vector3 &expected, double tolerance = 1e-9) {
    EXPECT_NEAR(actual.x, expected.x, tolerance);
    EXPECT_NEAR(actual.y, expected.y, tolerance);
    EXPECT_NEAR(actual.z, expected.z, tolerance);
}

TEST(Camera, UpVectorLookingAlongY) {
    auto camera = Camera{Vector3{0, 0, 0}, Vector3{0, 100, 0}};
    expectVectorNear(camera.upVector(), Vector3(0, 0, 1));
}

TEST(Camera, UpVectorLookingAlongX) {
    auto camera = Camera{Vector3{0, 0, 0}, Vector3{5, 0, 0}};
    expectVectorNear(camera.upVector(), Vector3(0, 0, 1));
}

TEST(Camera, UpVectorLookingStraightUpFromOffsetPosition) {
    auto camera = Camera{Vector3{1, 2, 3}, Vector3{1, 2, 13}};
    expectVectorNear(camera.upVector(), Vector3(0, 1, 0));
}

TEST(Camera, UpVectorWhenTilted) {
    auto camera = Camera{Vector3{0, 0, 0}, Vector3{0, 1, 1}};
    const double half = std::sqrt(2.0) / 2.0;
    expectVectorNear(camera.upVector(), Vector3(0, -half, half));
}

TEST(Camera, UpVectorIsOrthogonalToForwardAndRight) {
    auto camera = Camera{Vector3{3, -2, 7}, Vector3{-4, 9, 1}};
    EXPECT_NEAR(Vector3::dotProduct(camera.upVector(), camera.forwardVector()), 0, 1e-9);
    EXPECT_NEAR(Vector3::dotProduct(camera.upVector(), camera.rightVector()), 0, 1e-9);
    EXPECT_NEAR(camera.upVector().length(), 1, 1e-9);
}

// ---- getScreenPixelVector ----
// Default screen: 1920x1080, vertical FOV 60 degrees.
// Camera at origin looking along +X with Z up gives:
//   forward = (1, 0, 0), right = (0, -1, 0), up = (0, 0, 1)
// Fractional pixel inputs such as -0.5 or 1919.5 land exactly on screen edges.

namespace {
    constexpr double kScreenPixelVectorTolerance = 1e-9;
    constexpr double kPi = std::numbers::pi_v<double>;

    double halfHeightForDefaultFov() {
        return std::tan((60.0 / 2.0) * kPi / 180.0);
    }

    double halfWidthForDefaultFov() {
        return halfHeightForDefaultFov() * (1920.0 / 1080.0);
    }

    void expectVector3Near(const Vector3 &actual, const Vector3 &expected) {
        EXPECT_NEAR(actual.x, expected.x, kScreenPixelVectorTolerance);
        EXPECT_NEAR(actual.y, expected.y, kScreenPixelVectorTolerance);
        EXPECT_NEAR(actual.z, expected.z, kScreenPixelVectorTolerance);
    }

    double vector3Length(const Vector3 &v) {
        return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    }
}

TEST(CameraGetScreenPixelVector, ScreenCenterPointsForward) {
    const Camera camera({0, 0, 0}, {1, 0, 0});
    const Vector3 ray = camera.getScreenPixelVector(959.5, 539.5);
    expectVector3Near(ray, {1, 0, 0});
}

TEST(CameraGetScreenPixelVector, ReturnsUnitVector) {
    const Camera camera({0, 0, 0}, {1, 0, 0});
    EXPECT_NEAR(vector3Length(camera.getScreenPixelVector(0, 0)), 1.0, kScreenPixelVectorTolerance);
    EXPECT_NEAR(vector3Length(camera.getScreenPixelVector(1919, 1079)), 1.0, kScreenPixelVectorTolerance);
    EXPECT_NEAR(vector3Length(camera.getScreenPixelVector(123, 456)), 1.0, kScreenPixelVectorTolerance);
}

TEST(CameraGetScreenPixelVector, TopEdgeIsHalfVerticalFovAboveForward) {
    const Camera camera({0, 0, 0}, {1, 0, 0});
    const Vector3 ray = camera.getScreenPixelVector(959.5, -0.5);
    EXPECT_NEAR(ray.y, 0.0, kScreenPixelVectorTolerance);
    EXPECT_GT(ray.z, 0.0);
    EXPECT_NEAR(ray.z / ray.x, halfHeightForDefaultFov(), kScreenPixelVectorTolerance);
}

TEST(CameraGetScreenPixelVector, BottomEdgeIsHalfVerticalFovBelowForward) {
    const Camera camera({0, 0, 0}, {1, 0, 0});
    const Vector3 ray = camera.getScreenPixelVector(959.5, 1079.5);
    EXPECT_NEAR(ray.y, 0.0, kScreenPixelVectorTolerance);
    EXPECT_LT(ray.z, 0.0);
    EXPECT_NEAR(ray.z / ray.x, -halfHeightForDefaultFov(), kScreenPixelVectorTolerance);
}

TEST(CameraGetScreenPixelVector, LeftEdgeUsesAspectRatio) {
    const Camera camera({0, 0, 0}, {1, 0, 0});
    const Vector3 ray = camera.getScreenPixelVector(-0.5, 539.5);
    EXPECT_NEAR(ray.z, 0.0, kScreenPixelVectorTolerance);
    EXPECT_GT(ray.y, 0.0); // left of the camera is +Y
    EXPECT_NEAR(ray.y / ray.x, halfWidthForDefaultFov(), kScreenPixelVectorTolerance);
}

TEST(CameraGetScreenPixelVector, RightEdgeUsesAspectRatio) {
    const Camera camera({0, 0, 0}, {1, 0, 0});
    const Vector3 ray = camera.getScreenPixelVector(1919.5, 539.5);
    EXPECT_NEAR(ray.z, 0.0, kScreenPixelVectorTolerance);
    EXPECT_LT(ray.y, 0.0); // right of the camera is -Y
    EXPECT_NEAR(ray.y / ray.x, -halfWidthForDefaultFov(), kScreenPixelVectorTolerance);
}

TEST(CameraGetScreenPixelVector, TopLeftCornerPointsUpAndLeft) {
    const Camera camera({0, 0, 0}, {1, 0, 0});
    const Vector3 ray = camera.getScreenPixelVector(-0.5, -0.5);
    const Vector3 expected = Vector3::normalize(
        Vector3{1, halfWidthForDefaultFov(), halfHeightForDefaultFov()});
    expectVector3Near(ray, expected);
}

TEST(CameraGetScreenPixelVector, OppositeCornersAreSymmetric) {
    const Camera camera({0, 0, 0}, {1, 0, 0});
    const Vector3 topLeft = camera.getScreenPixelVector(0, 0);
    const Vector3 bottomRight = camera.getScreenPixelVector(1919, 1079);
    EXPECT_NEAR(topLeft.x, bottomRight.x, kScreenPixelVectorTolerance);
    EXPECT_NEAR(topLeft.y, -bottomRight.y, kScreenPixelVectorTolerance);
    EXPECT_NEAR(topLeft.z, -bottomRight.z, kScreenPixelVectorTolerance);
}

TEST(CameraGetScreenPixelVector, DoesNotDependOnCameraPosition) {
    const Camera atOrigin({0, 0, 0}, {1, 0, 0});
    const Camera moved({10, -4, 7}, {11, -4, 7});
    expectVector3Near(atOrigin.getScreenPixelVector(0, 0), moved.getScreenPixelVector(0, 0));
    expectVector3Near(atOrigin.getScreenPixelVector(1500, 300), moved.getScreenPixelVector(1500, 300));
}

TEST(CameraGetScreenPixelVector, DoesNotDependOnTargetDistance) {
    const Camera near({0, 0, 0}, {1, 0, 0});
    const Camera far({0, 0, 0}, {100, 0, 0});
    expectVector3Near(near.getScreenPixelVector(0, 0), far.getScreenPixelVector(0, 0));
    expectVector3Near(near.getScreenPixelVector(1500, 300), far.getScreenPixelVector(1500, 300));
}

TEST(CameraGetScreenPixelVector, FollowsRotatedCamera) {
    // Looking along +Y with Z up: right = (1, 0, 0), up = (0, 0, 1)
    const Camera camera({1, 2, 3}, {1, 12, 3});
    expectVector3Near(camera.getScreenPixelVector(959.5, 539.5), {0, 1, 0});

    const Vector3 rightEdge = camera.getScreenPixelVector(1919.5, 539.5);
    EXPECT_GT(rightEdge.x, 0.0);
    EXPECT_NEAR(rightEdge.z, 0.0, kScreenPixelVectorTolerance);
    EXPECT_NEAR(rightEdge.x / rightEdge.y, halfWidthForDefaultFov(), kScreenPixelVectorTolerance);
}
