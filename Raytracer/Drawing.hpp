#pragma once
#include <SDL3/SDL.h>
#include "VecUtils.hpp"

namespace Colors {
    inline const vec4 White = { 1.0f, 1.0f, 1.0f, 1.0f };
    inline const vec4 Black = { 0.0f, 0.0f, 0.0f, 1.0f };
    inline const vec4 Red = { 1.0f, 0.0f, 0.0f, 1.0f };
    inline const vec4 Green = { 0.0f, 1.0f, 0.0f, 1.0f };
    inline const vec4 Blue = { 0.0f, 0.0f, 1.0f, 1.0f };
    inline const vec4 Yellow = { 1.0f, 1.0f, 0.0f, 1.0f };
    inline const vec4 Cyan = { 0.0f, 1.0f, 1.0f, 1.0f };
    inline const vec4 Magenta = { 1.0f, 0.0f, 1.0f, 1.0f };
    inline const vec4 Gray = { 0.502f, 0.502f, 0.502f, 1.0f }; 
    inline const vec4 Orange = { 1.0f, 0.647f, 0.0f, 1.0f };     
    inline const vec4 Purple = { 0.502f, 0.0f, 0.502f, 1.0f };  
    inline const vec4 Brown = { 0.647f, 0.165f, 0.165f, 1.0f }; 
}


namespace Drawing 
{
	constexpr int ResX = 400;
	constexpr int ResY = 400;

    constexpr int ViewportWidth = 1;
    constexpr int ViewportHeight = 1;
    constexpr int ViewportDistanceFromCamera = 1;

	void DrawPixel(SDL_Renderer* Renderer, int x, int y, color4 Color);

    vec3 CanvasToViewport(ivec2 CanvasPos);
}
