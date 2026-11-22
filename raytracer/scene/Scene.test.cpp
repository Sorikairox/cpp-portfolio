//
// Tests for Scene::render.
//

#include "Scene.h"
#include "gtest/gtest.h"

namespace {
    // Camera at the origin looking down +y, so screen-up is +z and screen-right is +x.
    Camera makeCamera() {
        return Camera({0, 0, 0}, {0, 10, 0});
    }

    const Pixel *pixelAt(const Scene &scene, const size_t x, const size_t y) {
        const auto width = static_cast<size_t>(scene.camera.screenDimension.x);
        return scene.pixels.at(x + y * width);
    }

    void expectColor(const Pixel *pixel, const unsigned char red, const unsigned char green, const unsigned char blue) {
        ASSERT_NE(pixel, nullptr) << "pixel was never written";
        EXPECT_EQ(static_cast<unsigned char>(pixel->red), red);
        EXPECT_EQ(static_cast<unsigned char>(pixel->green), green);
        EXPECT_EQ(static_cast<unsigned char>(pixel->blue), blue);
    }
}

TEST(SceneRender, EmptySceneRendersEveryPixelWhite) {
    Scene scene(makeCamera());

    scene.render();

    const auto width = static_cast<size_t>(scene.camera.screenDimension.x);
    const auto height = static_cast<size_t>(scene.camera.screenDimension.y);
    ASSERT_EQ(scene.pixels.size(), width * height);

    for (size_t index = 0; index < scene.pixels.size(); index++) {
        const Pixel *pixel = scene.pixels[index];
        ASSERT_NE(pixel, nullptr) << "pixel " << index << " was never written";
        ASSERT_EQ(static_cast<unsigned char>(pixel->red), 255) << "pixel " << index;
        ASSERT_EQ(static_cast<unsigned char>(pixel->green), 255) << "pixel " << index;
        ASSERT_EQ(static_cast<unsigned char>(pixel->blue), 255) << "pixel " << index;
    }
}

TEST(SceneRender, SphereInFrontOfCameraRendersRedAndBackgroundStaysWhite) {
    Scene scene(makeCamera());
    // Radius 1 at distance 10: covers ~5.7° around the view center,
    // well inside the 60° vertical FOV, so the corners can't see it.
    scene.addSphere(Sphere({0, 10, 0}, 1, {255, 0, 0}));

    scene.render();

    const auto width = static_cast<size_t>(scene.camera.screenDimension.x);
    const auto height = static_cast<size_t>(scene.camera.screenDimension.y);

    // Center of the screen looks straight at the sphere.
    expectColor(pixelAt(scene, width / 2, height / 2), 255, 0, 0);

    // Corners miss the sphere and keep the background color.
    expectColor(pixelAt(scene, 0, 0), 255, 255, 255);
    expectColor(pixelAt(scene, width - 1, 0), 255, 255, 255);
    expectColor(pixelAt(scene, 0, height - 1), 255, 255, 255);
    expectColor(pixelAt(scene, width - 1, height - 1), 255, 255, 255);
}

TEST(SceneRender, EachSphereRendersWithItsOwnColor) {
    Scene scene(makeCamera());
    // Two radius-1 spheres at distance 10, one on each side of the view center.
    // On the image plane at distance 1, their centers project to x = -0.3 and x = +0.3.
    // With halfWidth = tan(30°) * 16/9 ≈ 1.026, that is pixel x ≈ (±0.3 + 1.026) / 2.052 * 1920:
    //   left  ≈ 679, right ≈ 1241.
    scene.addSphere(Sphere({-3, 10, 0}, 1, {0, 128, 255}));
    scene.addSphere(Sphere({3, 10, 0}, 1, {255, 200, 0}));

    scene.render();

    const auto height = static_cast<size_t>(scene.camera.screenDimension.y);
    const size_t centerY = height / 2;

    expectColor(pixelAt(scene, 679, centerY), 0, 128, 255);
    expectColor(pixelAt(scene, 1241, centerY), 255, 200, 0);

    // The view center passes between the two spheres (3 units from each center, radius 1).
    expectColor(pixelAt(scene, 960, centerY), 255, 255, 255);
}
