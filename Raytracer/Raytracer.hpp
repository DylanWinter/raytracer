#pragma once
#include <vector>

#include "VecUtils.hpp"
#include "Drawing.hpp"

struct Ray
{
	vec3 Origin = vec3(0, 0, 0);
	vec3 Direction = vec3(1, 0, 0);
	float MaxDistance = std::numeric_limits<float>::max();

	Ray() = default;

	Ray(const vec3& Origin, const vec3& Direction, float MaxDistance = std::numeric_limits<float>::max())
		: Origin(Origin),
		Direction(VecUtils::normalize(Direction)),
		MaxDistance(MaxDistance) {}
};

struct RayPayload
{
	float t;
	color4 Color;

	RayPayload(float TValue, const color4& Color)
		: t(TValue), Color(Color) {}
};

struct Sphere
{
	vec3 Origin = vec3(0, 0, 0);
	float Radius = 1.0f;
	color4 Color = Colors::Magenta;

	Sphere() = default;

	Sphere(const vec3& Origin, float Radius, const color4& Color)
		: Origin(Origin),
		Radius(Radius),
		Color(Color) {}
};

struct Scene
{
	color4 BackgroundColor = color4(255, 255, 255, 255);

	vec3 Origin = vec3(0, 0, 0);
	std::vector<Sphere> Spheres{};
};

namespace Raytracer {
	RayPayload TraceRay(Scene& Scene, Ray Ray, float TMin = 1.0, float TMax = std::numeric_limits<float>::max());

	static std::pair<float, float> RayIntersectSphere(Ray& Ray, Sphere& s);
}