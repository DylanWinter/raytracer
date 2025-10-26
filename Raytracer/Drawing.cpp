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
		
		auto Remap = [](float Value)
		{
			return Value / (Value + 1.0f);
		};
		SDL_SetRenderDrawColor(Renderer, Remap(Color.r) * 255, Remap(Color.g) * 255, Remap(Color.b) * 255, Color.a * 255);
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