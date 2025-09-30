#pragma once
#include <SDL3/SDL.h>
#include "VecUtils.hpp"

namespace Colors {
    inline const color4 White = { 255, 255, 255, 255 };
    inline const color4 Black = { 0,   0,   0,   255 };
    inline const color4 Red = { 255, 0,   0,   255 };
    inline const color4 Green = { 0,   255, 0,   255 };
    inline const color4 Blue = { 0,   0,   255, 255 };
    inline const color4 Yellow = { 255, 255, 0,   255 };
    inline const color4 Cyan = { 0,   255, 255, 255 };
    inline const color4 Magenta = { 255, 0,   255, 255 };
    inline const color4 Gray = { 128, 128, 128, 255 };
    inline const color4 Orange = { 255, 165, 0,   255 };
    inline const color4 Purple = { 128, 0,   128, 255 };
    inline const color4 Brown = { 165, 42,  42,  255 };
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
