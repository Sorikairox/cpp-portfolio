//
// Created by Thomas Cruveilher on 2026/11/22.
//

#ifndef VECTOR3_TEST_SCENE_H
#define VECTOR3_TEST_SCENE_H
#include <vector>
#include "../sphere/Sphere.h"
#include "../camera/Camera.h"

class Pixel {
    public:
    mutable unsigned char red = 0;
    mutable unsigned char green = 0;
    mutable unsigned char blue = 0;
    unsigned char alpha = 0;
    int x = 0;
    int y = 0;
    Pixel() = default;
    Pixel(const unsigned char red, const unsigned char green, const unsigned char blue, const unsigned char alpha):
    red(red), green(green), blue(blue), alpha(alpha) {}
};


class Scene {

    public:

    Camera camera = Camera({0,0,0}, {0, 100, 0});
    std::vector<Sphere> spheres = {};
    std::vector<Pixel *> pixels = {};

    explicit Scene(const Camera &camera) : camera(camera) {
        this->pixels.resize(camera.screenDimension.x * camera.screenDimension.y, nullptr);
    }

    void addSphere(const Sphere &sphere) { spheres.push_back(sphere); }

    void render() {
        auto currentRay = Ray(this->camera.position);
        for (int i = 0; i < this->camera.screenDimension.x; i++) {
            for (int j = 0; j < this->camera.screenDimension.y; j++) {
                const double currentIndex = i + (j * this->camera.screenDimension.x);
                Pixel *currentPixel = this->pixels[currentIndex];
                if (currentPixel== nullptr)
                    currentPixel = new Pixel{static_cast<unsigned>(255), static_cast<unsigned>(255), static_cast<unsigned>(255), 1};

                this->pixels[currentIndex] = currentPixel;
                currentPixel->x = i;
                currentPixel->y = j;
                currentPixel->blue = static_cast<unsigned>(255);
                currentPixel->green = static_cast<unsigned>(255);
                currentPixel->red = static_cast<unsigned>(255);
                currentRay.direction = this->camera.getScreenPixelVector(i, j);
                currentRay.intersection = {};
                for (const Sphere &sphere : this->spheres) {
                    if (sphere.doesIntersect(&currentRay)) {
                        currentPixel->blue = sphere.color.b;
                        currentPixel->green = sphere.color.g;
                        currentPixel->red = sphere.color.r;
                    }
                }
            }
        }
    }
};


#endif //VECTOR3_TEST_SCENE_H
