//
// Created by Thomas Cruveilher on 2026/11/22.
//

#ifndef VECTOR3_TEST_SCENE_H
#define VECTOR3_TEST_SCENE_H
#include <algorithm>
#include <vector>
#include <random>

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



struct Shading {
    Vector3 diffuse;   // ambient + diffuse
    Vector3 specular;  // highlights only, added after reflection mixing
};

class Scene {

    int sampleNumberPerPixel = 10;
    double pixelColorScale = 1.0 / this->sampleNumberPerPixel;
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

    [[nodiscard]] Shading computeShading(Ray const& currentRay, Entity const &intersectedObject) const {
        const Vector3 ambientColor =
                intersectedObject.material->color.albedoMultiplication(lightColor) * 0.5;
        Vector3 diffuseColor = ambientColor;
        Vector3 specularColor = {0,0,0};

        for (Light const &light : this->lights) {
            // L
            currentRay.intersection.directionToLight = Vector3::normalize(light.position - currentRay.intersection.position);
            currentRay.intersection.lightHits = Vector3::dotProduct(currentRay.intersection.surfaceNormal,
                                                              currentRay.intersection.directionToLight);

            auto shadowRay = Ray(currentRay.intersection.position + currentRay.intersection.surfaceNormal * 1e-4, currentRay.intersection.directionToLight);

            if (getIntersectEntity(shadowRay) != nullptr) {
                continue;
            }
            //H
            currentRay.intersection.specularIntensity = std::pow(
                std::max(0.0, Vector3::dotProduct(currentRay.intersection.surfaceNormal,
                    Vector3::normalize(currentRay.intersection.directionToLight -currentRay.direction)
                    )), intersectedObject.material->shininess);

            diffuseColor = diffuseColor + (
                    intersectedObject.material->color.albedoMultiplication(lightColor)
                    * 1 * std::max(0.0, currentRay.intersection.lightHits));

            specularColor = specularColor + lightColor.albedoMultiplication(intersectedObject.material->specularColor) * intersectedObject.material->specularStrength * currentRay.intersection.specularIntensity;
        }

        return Shading{diffuseColor, specularColor};
    }

    static Color toColor(const Vector3 &color) {
        return Color{static_cast<unsigned char>(std::clamp(color.x * 255, 0.0, 255.0)),
            static_cast<unsigned char>(std::clamp(color.y * 255, 0.0, 255.0)),
            static_cast<unsigned char>(std::clamp(color.z * 255, 0.0, 255.0)),
            255};
    }

    static Vector3 sampleSquare() {
        return {getRandomDouble() - 0.5, getRandomDouble() - 0.5, 0};
    }


    [[nodiscard]] const Entity *getIntersectEntity(Ray const& currentRay) const {
        const Entity *currentIntersectedObject = nullptr;
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
        return currentIntersectedObject;
    }

    // Returns linear color in [0, 1+] range; clamped only once in computePixelColor.
    Vector3 computeRayColor(Ray currentRay, int depth = 10) {
        if (depth == 0)
            return {0,0,0};

        const Entity *currentIntersectedObject = getIntersectEntity(currentRay);
        if (!currentIntersectedObject)
            return {0,0,0};

        const Shading shading = computeShading(currentRay, *currentIntersectedObject);
        const double reflectivity = currentIntersectedObject->material->reflectivity;

        if (reflectivity <= 0)
            return shading.diffuse + shading.specular;

        currentRay.intersection.reflectionDirection = Vector3::normalize(currentRay.direction - currentRay.intersection.surfaceNormal * 2 *
            Vector3::dotProduct(currentRay.direction,currentRay.intersection.surfaceNormal));

        Ray reflectionRay = Ray(currentRay.intersection.position + currentRay.intersection.surfaceNormal * 1e-4, currentRay.intersection.reflectionDirection);
        const Vector3 reflectionColor = computeRayColor(reflectionRay, depth - 1);

        // Mix only the diffuse part with the reflection; the highlight is added on top so it is never dimmed.
        const Vector3 mixed = shading.diffuse * (1 - reflectivity) + reflectionColor * reflectivity;
        return mixed + shading.specular;
    }

    Color computePixelColor(int i, int j) {
        auto currentRay = Ray(this->camera.position);
        Vector3 offset = sampleSquare();
        currentRay.direction = this->camera.getScreenPixelVector(i + offset.x, j + offset.y);

        currentRay.intersection = {};
        return toColor(computeRayColor(currentRay));
    }

    void render() {
        for (int i = 0; i < this->camera.screenDimension.x; i++) {
            for (int j = 0; j < this->camera.screenDimension.y; j++) {
                const double currentIndex = i + (j * this->camera.screenDimension.x);
                Pixel *currentPixel = this->pixels[currentIndex];
                if (currentPixel== nullptr)
                    currentPixel = new Pixel{static_cast<unsigned>(255), static_cast<unsigned>(255), static_cast<unsigned>(255), 1};

                this->pixels[currentIndex] = currentPixel;
                currentPixel->x = i;
                currentPixel->y = j;
                int redSum = 0;
                int greenSum = 0;
                int blueSum = 0;
                for (int sampleIndex = 0; sampleIndex < this->sampleNumberPerPixel; sampleIndex++) {
                    const Color sample = this->computePixelColor(i, j);
                    redSum += sample.r;
                    greenSum += sample.g;
                    blueSum += sample.b;
                }
                currentPixel->red = static_cast<unsigned char>(redSum * this->pixelColorScale);
                currentPixel->green = static_cast<unsigned char>(greenSum * this->pixelColorScale);
                currentPixel->blue = static_cast<unsigned char>(blueSum * this->pixelColorScale);
            }
        }
    }

    static double getRandomDouble() {
        thread_local std::mt19937 gen(std::random_device{}());
        thread_local std::uniform_real_distribution distrib(0.0, 1.0);
        return distrib(gen);
    }

    static double getRandomDouble(const double min, const double max) {
        // Returns a random real in [min,max).
        return min + (max-min)*getRandomDouble();
    }
};


#endif //VECTOR3_TEST_SCENE_H
