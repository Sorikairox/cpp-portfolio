//
// Created by Thomas Cruveilher on 2026/11/20.
//


#include "scene/Scene.h"
#include "SDL3/SDL.h"

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Raytracer", 1920, 1080, SDL_WINDOW_FULLSCREEN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, nullptr);

    auto sphere = Sphere({0, 25, 0}, 2, {255, 0, 0, 255});
    const auto camera = Camera({0, 0, 0}, {0, 50, 0});
    auto *scene = new Scene(camera);
    scene->addSphere(sphere);

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
                case SDL_EVENT_KEY_DOWN:
                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);
                    SDL_Quit();
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