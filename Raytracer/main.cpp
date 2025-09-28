#include <SDL3/SDL.h>
#include <iostream>

#include "Drawing.hpp"

int main(int argc, char* argv[]) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << "\n";
        return 1;
    }

    SDL_Window* Window = SDL_CreateWindow(
        "Raytracer", Raytracer::ResX, Raytracer::ResY, SDL_WINDOW_RESIZABLE);
    if (!Window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << "\n";
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* Renderer = SDL_CreateRenderer(Window, nullptr);
    SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);

    bool running = true;
    SDL_Event e;
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT)
                running = false;
        }

        SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
        SDL_RenderClear(Renderer);

        vec3 a{ 1, 2, 3 };
        vec3 b{ 4, 5, 6 };
        vec3 c = VecUtils::cross(a, b);
        std::cout << c.x << " " << c.y << " " << c.z << "\n"; 

        Raytracer::DrawPixel(Renderer, 4, 4, ivec4{ 255, 255, 255, 255 });

        SDL_RenderPresent(Renderer);
    }

    SDL_DestroyRenderer(Renderer);
    SDL_DestroyWindow(Window);
    SDL_Quit();
    return 0;
}
