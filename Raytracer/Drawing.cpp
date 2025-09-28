#include <SDL3/SDL.h>
#include <iostream>

#include "Drawing.hpp"

namespace Raytracer 
{
	void DrawPixel(SDL_Renderer* Renderer, int x, int y, ivec4 Color) 
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
}