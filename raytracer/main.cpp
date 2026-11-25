//
// Created by Thomas Cruveilher on 2026/11/20.
//


#include "scene/Scene.h"
#include "SDL3/SDL.h"

int main() {
    int w;
    int h;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Raytracer", 1920, 1080, SDL_WINDOW_FULLSCREEN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, nullptr);
    SDL_GetRenderOutputSize(renderer, &w, &h);

    auto goldMaterial = new Material{0, {212, 175, 55, 255}, 1, 32.0, {212, 175, 55, 255}};
    auto reflectiveSphere = Sphere({0, -30, 0}, 10);
    reflectiveSphere.attachMaterial(goldMaterial);

    auto redMaterial = new Material{0, {255, 0, 0, 255}};
    auto redSphere = Sphere({0, 0, 0}, 10);
    redSphere.attachMaterial(redMaterial);
    const auto camera = Camera({-50, 0, 0}, {0, 0, 0},  w, h);

    auto *scene = new Scene(camera);
    scene->addSphere(redSphere);
    scene->addSphere(reflectiveSphere);

    auto whitePlane = Plane{Vector3{0,0,-10}, Vector3{0,0,1}};
    whitePlane.attachMaterial(new Material{0, {255, 255, 255, 255}});

    scene->addLight(Light{Vector3{-50, 0, 500}, Vector3{0, 0, 0}});
    scene->addPlane(whitePlane);

    scene->render();
    SDL_RenderClear(renderer);
    for (Pixel const *pixel: scene->pixels) {
        SDL_SetRenderDrawColor(renderer, pixel->red, pixel->green, pixel->blue, pixel->alpha);
        SDL_RenderPoint(renderer, pixel->x, pixel->y);
    }

    SDL_Event event;
    while (true) {
        SDL_RenderPresent(renderer);
        while (SDL_WaitEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_KEY_DOWN :
                case SDL_EVENT_QUIT:
                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);
                    SDL_Quit();
                    break;
                default:
            }
        }
        exit(0);
    }
}