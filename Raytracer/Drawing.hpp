#pragma once
#include <SDL3/SDL.h>
#include "VecUtils.hpp"

namespace Raytracer 
{
	constexpr int ResX = 640;
	constexpr int ResY = 480;

	void DrawPixel(SDL_Renderer* Renderer, int x, int y, ivec4 Color);
}
