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

TEST(SphereDoesIntersect, RayAimedAtCenterHits) {
    const Sphere sphere({0, 0, 0}, 2);
    Ray ray({0, 0, -5}, {0, 0, 1});
    // b = -10, c = 25 - 4 = 21, delta = 100 - 84 = 16
    EXPECT_TRUE(sphere.doesIntersect(ray));
}

TEST(SphereDoesIntersect, RayPassingFarAwayMisses) {
    const Sphere sphere({0, 0, 0}, 1);
    Ray ray({0, 5, -5}, {0, 0, 1});
    // b = -10, c = 50 - 1 = 49, delta = 100 - 196 = -96
    EXPECT_FALSE(sphere.doesIntersect(ray));
}

TEST(SphereDoesIntersect, TangentRayCountsAsHit) {
    const Sphere sphere({0, 0, 0}, 1);
    Ray ray({0, 1, -5}, {0, 0, 1});
    // b = -10, c = 26 - 1 = 25, delta = 100 - 100 = 0 (exactly)
    EXPECT_TRUE(sphere.doesIntersect(ray));
}

TEST(SphereDoesIntersect, RayJustInsideTangentHits) {
    const Sphere sphere({0, 0, 0}, 1);
    Ray ray({0, 0.999, -5}, {0, 0, 1});
    EXPECT_TRUE(sphere.doesIntersect(ray));
}

TEST(SphereDoesIntersect, RayJustOutsideTangentMisses) {
    const Sphere sphere({0, 0, 0}, 1);
    Ray ray({0, 1.001, -5}, {0, 0, 1});
    EXPECT_FALSE(sphere.doesIntersect(ray));
}

TEST(SphereDoesIntersect, SubtractsRadiusSquaredInC) {
    // c must be |O - C|² - r², not |O - C|² · r².
    // Radius < 1 makes the two differ in outcome:
    //   correct: c = 26 - 0.25 = 25.75, delta = 100 - 103 = -3  -> miss
    //   wrong:   c = 26 · 0.25 = 6.5,   delta = 100 - 26  = 74  -> hit
    const Sphere sphere({0, 0, 0}, 0.5);
    Ray ray({0, 1, -5}, {0, 0, 1});
    EXPECT_FALSE(sphere.doesIntersect(ray));
}

TEST(SphereDoesIntersect, RayStartingInsideSphereHits) {
    const Sphere sphere({0, 0, 0}, 3);
    Ray ray({0, 0, 0}, {1, 0, 0});
    // b = 0, c = -9, delta = 36
    EXPECT_TRUE(sphere.doesIntersect(ray));
}

TEST(SphereDoesIntersect, SphereAwayFromOriginHits) {
    const Sphere sphere({10, 0, 0}, 2);
    Ray ray({0, 0, 0}, {1, 0, 0});
    EXPECT_TRUE(sphere.doesIntersect(ray));
}

TEST(SphereDoesIntersect, SphereAwayFromOriginMissesPerpendicularRay) {
    const Sphere sphere({10, 0, 0}, 2);
    Ray ray({0, 0, 0}, {0, 1, 0});
    EXPECT_FALSE(sphere.doesIntersect(ray));
}

TEST(SphereDoesIntersect, DiagonalRayHits) {
    const Sphere sphere({0, 0, 0}, 1);
    Ray ray({-5, -5, 0}, Vector3::normalize({1, 1, 0}));
    EXPECT_TRUE(sphere.doesIntersect(ray));
}


TEST(SphereDoesIntersect, DirectionLengthDoesNotChangeResult) {
    const Sphere sphere({0, 0, 0}, 2);
    Ray unitHit({0, 1, -5}, {0, 0, 1});
    Ray longHit({0, 1, -5}, {0, 0, 7});
    Ray unitMiss({0, 3, -5}, {0, 0, 1});
    Ray longMiss({0, 3, -5}, {0, 0, 7});
    EXPECT_TRUE(sphere.doesIntersect(unitHit));
    EXPECT_TRUE(sphere.doesIntersect(longHit));
    EXPECT_FALSE(sphere.doesIntersect(unitMiss));
    EXPECT_FALSE(sphere.doesIntersect(longMiss));
}

// Disabled on purpose: the discriminant alone tests the infinite LINE, so a
// sphere behind the ray still gives delta >= 0. Enable this once you also
// check that the hit distance t is positive.
TEST(SphereDoesIntersect, DISABLED_SphereBehindRayMisses) {
    const Sphere sphere({0, 0, 0}, 1);
    Ray ray({0, 0, 5}, {0, 0, 1});
    EXPECT_FALSE(sphere.doesIntersect(ray));
}
