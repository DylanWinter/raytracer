#pragma once
#include <iostream>
#include <chrono>

#include <SDL3/SDL.h>

#include "Drawing.hpp"
#include "Raytracer.hpp"

int main(int argc, char* argv[]) {
    // SDL Setup
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << "\n";
        return 1;
    }
    SDL_Window* Window = SDL_CreateWindow(
        "Raytracer", Drawing::ResX, Drawing::ResY, SDL_WINDOW_RESIZABLE);
    if (!Window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << "\n";
        SDL_Quit();
        return 1;
    }
    SDL_Renderer* Renderer = SDL_CreateRenderer(Window, nullptr);
    SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);

    // Create scene
    Scene Scene;
    Scene.AddSphere(vec3(0, -1, 4), 1, Colors::Red, 100);
    Scene.AddSphere(vec3(2, 0, 5), 1, Colors::Blue, 1000);
    Scene.AddSphere(vec3(-2, 0, 5), 1, Colors::Green, 10);
    Scene.AddSphere(vec3(0, -1001, 0), 1000, Colors::Yellow, 10);
    Scene.AddAmbientLight(0.2f);
    Scene.AddPointLight(2.5f, vec3(2, 1, 0));

    // Main loop
    bool Running = true;
    SDL_Event e;
    while (Running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT)
                Running = false;
        }

        SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
        SDL_RenderClear(Renderer);

        auto StartTime = std::chrono::high_resolution_clock::now();
        
        // Rendering
        for (int x = -Drawing::ResX / 2; x < Drawing::ResX / 2; x++) 
        {
            for (int y = -Drawing::ResY / 2; y < Drawing::ResY / 2; y++) 
            {
                Ray ray = Ray(Scene.Origin, Drawing::CanvasToViewport(ivec2(x, y)));
                RayPayload Result = Raytracer::TraceRay(Scene, ray);
                Drawing::DrawPixel(Renderer, x + Drawing::ResX / 2, y + Drawing::ResY / 2, Result.Color);
            }
        }

        auto StopTime = std::chrono::high_resolution_clock::now();
        auto Duration = std::chrono::duration_cast<std::chrono::milliseconds>(StopTime - StartTime);
        std::cout << "Rendered in " << Duration.count() << " ms." << std::endl;

        SDL_RenderPresent(Renderer);
    }

    // Clean up
    SDL_DestroyRenderer(Renderer);
    SDL_DestroyWindow(Window);
    SDL_Quit();
    return 0;
}
