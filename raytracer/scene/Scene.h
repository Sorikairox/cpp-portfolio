//
// Created by Thomas Cruveilher on 2026/11/22.
//

#ifndef VECTOR3_TEST_SCENE_H
#define VECTOR3_TEST_SCENE_H
#include <algorithm>
#include <vector>

#include "../Plane.h"
#include "../sphere/Sphere.h"
#include "../camera/Camera.h"
#include "../Entity.h"

const Color lightColor = { 255,  255, 255};
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

class Light {
public:
    Vector3 position;
    Vector3 direction;

    Light(Vector3 const &position, Vector3 const &direction): position(position), direction(direction) {}
};

class Scene {

    public:

    Camera camera = Camera({0,0,0}, {0, 100, 0}, 1920, 1080);
    std::vector<Sphere> spheres = {};
    std::vector<Plane> planes = {};
    std::vector<Light> lights = {};
    std::vector<Pixel *> pixels = {};

    explicit Scene(const Camera &camera) : camera(camera) {
        this->pixels.resize(camera.screenDimension.x * camera.screenDimension.y, nullptr);
    }

    void addSphere(const Sphere &sphere) { spheres.push_back(sphere); }

    void addLight(const Light &light) { lights.push_back(light); }

    void addPlane(const Plane &plane) { planes.push_back(plane); }

    void computeColor(Ray const& currentRay, const Pixel *currentPixel, Entity const &intersectedObject) {
        const Vector3 ambientColor =
                intersectedObject.color.albedoMultiplication(lightColor) * 0.5;
            Vector3 finalColor = ambientColor;

        for (Light const &light : this->lights) {
            // L
            currentRay.intersection.directionToLight = Vector3::normalize(light.position - currentRay.intersection.position);
            currentRay.intersection.lightHits = Vector3::dotProduct(currentRay.intersection.surfaceNormal,
                                                              currentRay.intersection.directionToLight);
            //H
            currentRay.intersection.specularIntensity = std::pow(
                std::max(0.0, Vector3::dotProduct(currentRay.intersection.surfaceNormal,
                    Vector3::normalize(currentRay.intersection.directionToLight -currentRay.direction)
                    )), 10);

            const Vector3 diffuseColor =
                    intersectedObject.color.albedoMultiplication(lightColor)
                    * 1 * std::max(0.0, currentRay.intersection.lightHits);

            const Vector3 specularColor = lightColor.albedoMultiplication(lightColor) * 1 * currentRay.intersection.specularIntensity;
            finalColor = finalColor + diffuseColor + specularColor;
        }


        currentPixel->blue = std::clamp(finalColor.z * 255, 0.0, 255.0);
        currentPixel->green = std::clamp(finalColor.y * 255, 0.0, 255.0);
        currentPixel->red = std::clamp(finalColor.x * 255, 0.0, 255.0);
    }

    void render() {
        auto currentRay = Ray(this->camera.position);
        const Entity *currentIntersectedObject = nullptr;
        for (int i = 0; i < this->camera.screenDimension.x; i++) {
            for (int j = 0; j < this->camera.screenDimension.y; j++) {
                const double currentIndex = i + (j * this->camera.screenDimension.x);
                Pixel *currentPixel = this->pixels[currentIndex];
                if (currentPixel== nullptr)
                    currentPixel = new Pixel{static_cast<unsigned>(255), static_cast<unsigned>(255), static_cast<unsigned>(255), 1};

                this->pixels[currentIndex] = currentPixel;
                currentPixel->x = i;
                currentPixel->y = j;
                currentPixel->blue = static_cast<unsigned>(0);
                currentPixel->green = static_cast<unsigned>(0);
                currentPixel->red = static_cast<unsigned>(0);
                currentRay.direction = this->camera.getScreenPixelVector(i, j);
                currentRay.intersection = {};
                currentIntersectedObject = nullptr;

                for (Sphere const &sphere : this->spheres) {
                    if (sphere.doesIntersect(&currentRay)) {
                        currentIntersectedObject = &sphere;
                    }
                }
                for (Plane const &plane : this->planes) {
                    if (plane.doesIntersect(&currentRay)) {
                        currentIntersectedObject = &plane;
                    }
                }
                if (currentIntersectedObject) {
                        computeColor(currentRay, currentPixel, *currentIntersectedObject);
                }
            }
        }
    }
};


#endif //VECTOR3_TEST_SCENE_H
