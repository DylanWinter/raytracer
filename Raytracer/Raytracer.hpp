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

enum class LightType
{
	Ambient, 
	Point,
	Directional
};

struct Sphere
{
	vec3 Origin = vec3(0, 0, 0);
	float Radius = 1.0f;
	color4 Color = Colors::Magenta;

	std::optional<float> Specular = std::nullopt;

	Sphere() = default;

	// If the given value of Specular is negative, Specular will be std::nullopt
	Sphere(const vec3& Origin = VEC3_ZERO, float Radius = 1.0f, const color4& Color = Colors::Red, float Specular = -1.0f)
		: Origin(Origin),
		Radius(Radius),
		Color(Color),
		Specular(Specular < 0.0f ? std::nullopt : std::optional<float>(Specular))
	{}
};

struct Light
{
	LightType Type = LightType::Point;
	float Intensity = 1.0f;
	vec3 Position = vec3(0.0f, 0.0f, 0.0f);
	vec3 Direction = vec3(1.0f, 0.0f, 0.0f);

	Light() = default;
	Light(LightType Type, const float Intensity, const vec3& Position, const vec3& Direction) :
		Type(Type),
		Intensity(Intensity),
		Position(Position), 
		Direction(Direction) {}
};

struct Scene
{
	color4 BackgroundColor = Colors::White;
	vec3 Origin = vec3(0, 0, 0);

	// Objects in the scene
	std::vector<Sphere> Spheres{};
	std::vector<Light> Lights{};

	Sphere AddSphere(const vec3& Origin = vec3(0.0f, 0.0f, 0.0f), float Radius = 1.0f, const color4& Color = Colors::Red, float Specular = -1.0f)
	{
		return Spheres.emplace_back(Origin, Radius, Color, Specular);
	}

	Light AddLight(LightType Type, float Intensity = 1.0f, const vec3& Position = vec3(0.0f, 0.0f, 0.0f), const vec3& Direction = vec3(1.0f, 0.0f, 0.0f))
	{
		return Lights.emplace_back(Type, Intensity, Position, Direction);
	}

	Light AddAmbientLight(float Intensity = 1.0f)
	{
		return AddLight(LightType::Ambient, Intensity);
	}

	Light AddPointLight(float Intensity = 1.0f, const vec3& Position = vec3(0.0f, 0.0f, 0.0f))
	{
		return AddLight(LightType::Point, Intensity, Position);
	}

	Light AddDirectionalLight(float Intensity = 1.0f, const vec3& Direction = vec3(1.0f, 0.0f, 0.0f))
	{
		return AddLight(LightType::Directional, Intensity, vec3(0.0f, 0.0f, 0.0f), Direction);
	}
};

namespace Raytracer {
	RayPayload TraceRay(Scene& Scene, Ray Ray, float TMin = 1e-6, float TMax = std::numeric_limits<float>::max());
}