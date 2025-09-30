#pragma once
#include <SDL3/SDL.h>
#include <iostream>

#include "Drawing.hpp"

namespace Drawing 
{
	void DrawPixel(SDL_Renderer* Renderer, int x, int y, color4 Color) 
	{
		if (x > ResX) 
		{
			std::cout << "DrawPixel given invalid x coordinate: " << x << std::endl;
			return;
		}
		if (y > ResY)
		{
			std::cout << "DrawPixel given invalid y coordinate: " << y << std::endl;
			return;
		}
		SDL_SetRenderDrawColor(Renderer, Color.r, Color.g, Color.b, Color.a);
		SDL_RenderPoint(Renderer, x, y);
	}

	vec3 CanvasToViewport(ivec2 CanvasPos) 
	{
		return vec3(
			CanvasPos.x * (static_cast<float>(ViewportWidth) / ResX), 
			-CanvasPos.y * (static_cast<float>(ViewportHeight) / ResY), 
			ViewportDistanceFromCamera);
	}
}