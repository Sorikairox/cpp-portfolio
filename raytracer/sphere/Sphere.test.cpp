//
// Tests for Sphere::doesIntersect.
//
// Ray: P(t) = O + t·D. Sphere: |P - C|² = r².
// Substituting gives a·t² + b·t + c = 0 with:
//   a = D·D
//   b = 2 · D·(O - C)
//   c = (O - C)·(O - C) - r²
// The ray's line hits the sphere when the discriminant b² - 4ac >= 0.
//

#include "Sphere.h"
#include "../ray/Ray.h"
#include "gtest/gtest.h"
#include <cmath>

TEST(SphereDoesIntersect, RayAimedAtCenterHits) {
    const Sphere sphere({0, 0, 0}, 2);
    Ray ray({0, 0, -5}, {0, 0, 1});
    // b = -10, c = 25 - 4 = 21, delta = 100 - 84 = 16
    EXPECT_TRUE(sphere.doesIntersect(&ray));
}

TEST(SphereDoesIntersect, RayPassingFarAwayMisses) {
    const Sphere sphere({0, 0, 0}, 1);
    Ray ray({0, 5, -5}, {0, 0, 1});
    // b = -10, c = 50 - 1 = 49, delta = 100 - 196 = -96
    EXPECT_FALSE(sphere.doesIntersect(&ray));
}

TEST(SphereDoesIntersect, TangentRayCountsAsHit) {
    const Sphere sphere({0, 0, 0}, 1);
    Ray ray({0, 1, -5}, {0, 0, 1});
    // b = -10, c = 26 - 1 = 25, delta = 100 - 100 = 0 (exactly)
    EXPECT_TRUE(sphere.doesIntersect(&ray));
}

TEST(SphereDoesIntersect, RayJustInsideTangentHits) {
    const Sphere sphere({0, 0, 0}, 1);
    Ray ray({0, 0.999, -5}, {0, 0, 1});
    EXPECT_TRUE(sphere.doesIntersect(&ray));
}

TEST(SphereDoesIntersect, RayJustOutsideTangentMisses) {
    const Sphere sphere({0, 0, 0}, 1);
    Ray ray({0, 1.001, -5}, {0, 0, 1});
    EXPECT_FALSE(sphere.doesIntersect(&ray));
}

TEST(SphereDoesIntersect, SubtractsRadiusSquaredInC) {
    // c must be |O - C|² - r², not |O - C|² · r².
    // Radius < 1 makes the two differ in outcome:
    //   correct: c = 26 - 0.25 = 25.75, delta = 100 - 103 = -3  -> miss
    //   wrong:   c = 26 · 0.25 = 6.5,   delta = 100 - 26  = 74  -> hit
    const Sphere sphere({0, 0, 0}, 0.5);
    Ray ray({0, 1, -5}, {0, 0, 1});
    EXPECT_FALSE(sphere.doesIntersect(&ray));
}

TEST(SphereDoesIntersect, RayStartingInsideSphereHits) {
    const Sphere sphere({0, 0, 0}, 3);
    Ray ray({0, 0, 0}, {1, 0, 0});
    // b = 0, c = -9, delta = 36
    EXPECT_TRUE(sphere.doesIntersect(&ray));
}

TEST(SphereDoesIntersect, SphereAwayFromOriginHits) {
    const Sphere sphere({10, 0, 0}, 2);
    Ray ray({0, 0, 0}, {1, 0, 0});
    EXPECT_TRUE(sphere.doesIntersect(&ray));
}

TEST(SphereDoesIntersect, SphereAwayFromOriginMissesPerpendicularRay) {
    const Sphere sphere({10, 0, 0}, 2);
    Ray ray({0, 0, 0}, {0, 1, 0});
    EXPECT_FALSE(sphere.doesIntersect(&ray));
}

TEST(SphereDoesIntersect, DiagonalRayHits) {
    const Sphere sphere({0, 0, 0}, 1);
    Ray ray({-5, -5, 0}, Vector3::normalize({1, 1, 0}));
    EXPECT_TRUE(sphere.doesIntersect(&ray));
}


TEST(SphereDoesIntersect, DirectionLengthDoesNotChangeResult) {
    const Sphere sphere({0, 0, 0}, 2);
    Ray unitHit({0, 1, -5}, {0, 0, 1});
    Ray longHit({0, 1, -5}, {0, 0, 7});
    Ray unitMiss({0, 3, -5}, {0, 0, 1});
    Ray longMiss({0, 3, -5}, {0, 0, 7});
    EXPECT_TRUE(sphere.doesIntersect(&unitHit));
    EXPECT_TRUE(sphere.doesIntersect(&longHit));
    EXPECT_FALSE(sphere.doesIntersect(&unitMiss));
    EXPECT_FALSE(sphere.doesIntersect(&longMiss));
}

// The discriminant alone tests the infinite LINE, so a sphere behind the ray
// still gives delta >= 0. doesIntersect must also reject negative t.
TEST(SphereDoesIntersect, SphereBehindRayMisses) {
    const Sphere sphere({0, 0, 0}, 1);
    Ray ray({0, 0, 5}, {0, 0, 1});
    EXPECT_FALSE(sphere.doesIntersect(&ray));
}

// ---------------------------------------------------------------------------
// Intersection data written to the ray
// ---------------------------------------------------------------------------

static void expectVectorNear(const Vector3 &actual, const Vector3 &expected, double tolerance = 1e-9) {
    EXPECT_NEAR(actual.x, expected.x, tolerance);
    EXPECT_NEAR(actual.y, expected.y, tolerance);
    EXPECT_NEAR(actual.z, expected.z, tolerance);
}

TEST(SphereIntersection, HeadOnHitStoresNearestPoint) {
    const Sphere sphere({0, 0, 0}, 2);
    Ray ray({0, 0, -5}, {0, 0, 1});
    // roots t = 3 and t = 7, the nearest is 3
    ASSERT_TRUE(sphere.doesIntersect(&ray));
    EXPECT_NEAR(ray.intersection.distance, 3, 1e-9);
    expectVectorNear(ray.intersection.position, {0, 0, -2});
    expectVectorNear(ray.intersection.normal, {0, 0, -1});
}

TEST(SphereIntersection, OffCenterHitHasTiltedNormal) {
    const Sphere sphere({0, 0, 0}, 1);
    Ray ray({0, 0.6, -5}, {0, 0, 1});
    // surface at y = 0.6 is at z = -0.8, so t = 4.2
    ASSERT_TRUE(sphere.doesIntersect(&ray));
    EXPECT_NEAR(ray.intersection.distance, 4.2, 1e-9);
    expectVectorNear(ray.intersection.position, {0, 0.6, -0.8});
    expectVectorNear(ray.intersection.normal, {0, 0.6, -0.8});
}

TEST(SphereIntersection, NormalIsRelativeToSphereCenter) {
    const Sphere sphere({10, 0, 0}, 2);
    Ray ray({0, 0, 0}, {1, 0, 0});
    ASSERT_TRUE(sphere.doesIntersect(&ray));
    EXPECT_NEAR(ray.intersection.distance, 8, 1e-9);
    expectVectorNear(ray.intersection.position, {8, 0, 0});
    expectVectorNear(ray.intersection.normal, {-1, 0, 0});
}

TEST(SphereIntersection, TangentHitIsPerpendicularToRay) {
    const Sphere sphere({0, 0, 0}, 1);
    Ray ray({0, 1, -5}, {0, 0, 1});
    ASSERT_TRUE(sphere.doesIntersect(&ray));
    EXPECT_NEAR(ray.intersection.distance, 5, 1e-9);
    expectVectorNear(ray.intersection.position, {0, 1, 0});
    expectVectorNear(ray.intersection.normal, {0, 1, 0});
    EXPECT_NEAR(Vector3::dotProduct(ray.intersection.normal, ray.direction), 0, 1e-9);
}

TEST(SphereIntersection, NormalIsUnitLength) {
    const Sphere sphere({0, 0, 0}, 1);
    Ray ray({-5, -5, 0}, {1, 1, 0});
    ASSERT_TRUE(sphere.doesIntersect(&ray));
    EXPECT_NEAR(ray.intersection.distance, 5 * std::sqrt(2.0) - 1, 1e-9);
    EXPECT_NEAR(ray.intersection.normal.length(), 1, 1e-9);
    expectVectorNear(ray.intersection.normal, {-1 / std::sqrt(2.0), -1 / std::sqrt(2.0), 0});
}

TEST(SphereIntersection, HitPointLiesOnRayAndSphere) {
    const Sphere sphere({1, 2, 3}, 1.5);
    Ray ray({-4, 0, 1}, {5, 2.3, 2});
    ASSERT_TRUE(sphere.doesIntersect(&ray));
    const Intersection &hit = ray.intersection;
    EXPECT_NEAR((hit.position - sphere.origin).length(), sphere.radius, 1e-9);
    expectVectorNear(hit.position, ray.origin + ray.direction * hit.distance);
}

TEST(SphereIntersection, DirectionLengthDoesNotChangeDistance) {
    const Sphere sphere({0, 0, 0}, 2);
    Ray unitRay({0, 0, -5}, {0, 0, 1});
    Ray longRay({0, 0, -5}, {0, 0, 7});
    ASSERT_TRUE(sphere.doesIntersect(&unitRay));
    ASSERT_TRUE(sphere.doesIntersect(&longRay));
    EXPECT_NEAR(longRay.intersection.distance, unitRay.intersection.distance, 1e-9);
    expectVectorNear(longRay.intersection.position, unitRay.intersection.position);
}

TEST(SphereIntersection, RayStartingInsideUsesExitPoint) {
    // roots are t = -3 (behind) and t = 3 (in front): the hit is the exit point
    const Sphere sphere({0, 0, 0}, 3);
    Ray ray({0, 0, 0}, {1, 0, 0});
    ASSERT_TRUE(sphere.doesIntersect(&ray));
    EXPECT_NEAR(ray.intersection.distance, 3, 1e-9);
    expectVectorNear(ray.intersection.position, {3, 0, 0});
    expectVectorNear(ray.intersection.normal, {1, 0, 0});
}

TEST(SphereIntersection, MissLeavesIntersectionUntouched) {
    const Sphere sphere({0, 0, 0}, 1);
    Ray ray({0, 5, -5}, {0, 0, 1});
    ASSERT_FALSE(sphere.doesIntersect(&ray));
    EXPECT_EQ(ray.intersection.distance, INFINITY);
    expectVectorNear(ray.intersection.position, {0, 0, 0});
    expectVectorNear(ray.intersection.normal, {0, 0, 0});
}

TEST(SphereIntersection, SphereBehindLeavesIntersectionUntouched) {
    const Sphere sphere({0, 0, 0}, 1);
    Ray ray({0, 0, 5}, {0, 0, 1});
    ASSERT_FALSE(sphere.doesIntersect(&ray));
    EXPECT_EQ(ray.intersection.distance, INFINITY);
}
