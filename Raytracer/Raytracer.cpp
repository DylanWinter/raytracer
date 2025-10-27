#pragma once
#include <optional>
#include <iostream>

#include "VecUtils.hpp"
#include "Raytracer.hpp"

namespace {
	// Reflects ray direction over the normal and returns a new direction, normalized
	// Expects both ray direction and normal as unit vectors
	vec3 Reflect(vec3 RayDirection, vec3 Normal)
	{
		return VecUtils::normalize(2 * Normal * VecUtils::dot(RayDirection, Normal) - RayDirection);
	}

	// Uses the quadratic equation to determine where a ray collides with a sphere
	static std::pair<float, float> RayIntersectSphere(Ray& Ray, Sphere& s)
	{
		vec3 OriginToSphere = Ray.Origin - s.Origin;
		float a = VecUtils::length2(Ray.Direction);
		float b = 2 * VecUtils::dot(OriginToSphere, Ray.Direction);
		float c = VecUtils::length2(OriginToSphere) - (s.Radius * s.Radius);

		float Discriminant = b * b - 4 * a * c;
		if (Discriminant < 0)
			return std::pair<float, float>(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());

		float t1 = (-b - std::sqrt(Discriminant)) / (2 * a);
		float t2 = (-b + std::sqrt(Discriminant)) / (2 * a);
		return std::pair<float, float>(t1, t2);
	}

	static std::pair<float, std::optional<Sphere>> ClosestIntersection(Scene& Scene, Ray& Ray, float TMin = 1e-6, float TMax = std::numeric_limits<float>::max())
	{
		float ClosestT = std::numeric_limits<float>::max();
		std::optional<Sphere> ClosestSphere;

		for (Sphere& s : Scene.Spheres)
		{
			auto [t1, t2] = RayIntersectSphere(Ray, s);
			if (t1 > TMin && t1 < TMax && t1 < ClosestT)
			{
				ClosestT = t1;
				ClosestSphere = s;
			}
			if (t2 > TMin && t2 < TMax && t2 < ClosestT)
			{
				ClosestT = t2;
				ClosestSphere = s;
			}
		}

		return std::pair<float, std::optional<Sphere>>(ClosestT, ClosestSphere);
	}

	// Computes the intensity of light at a given point
	// Expects the normal and view direction as unit vectors
	float ComputeLighting(Scene& Scene, vec3 Point, vec3 Normal, vec3 ViewDirection, std::optional<float> Specular)
	{
		float Intensity = 0.0f;
		for (const Light& l : Scene.Lights)
		{
			// Ambient light is a simple add
			if (l.Type == LightType::Ambient)
			{
				Intensity += l.Intensity;
			}
			else
			{
				// Direction depends on what kind of light; Point lights must be computed, Directional is already known
				vec3 Direction = l.Type == LightType::Point ? (l.Position - Point) : l.Direction;
				Direction = VecUtils::normalize(Direction);

				// Shadow check; if the light source is obstructed, it does not contribute light
				float TMax = l.Type == LightType::Point ? 1 : std::numeric_limits<float>::max();
				Ray ShadowRay = Ray(Point + Normal * 1e-4f, Direction);
				auto [ShadowT, ShadowSphere] = ClosestIntersection(Scene, ShadowRay, 1e-6, TMax);
				if (ShadowSphere)
					continue;

				// Diffuse
				float NormalDotDirection = VecUtils::dot(Normal, Direction);
				if (NormalDotDirection > 0)
					Intensity += l.Intensity * NormalDotDirection;

				// Specular
				if (Specular.has_value())
				{
					const vec3 Reflected = Reflect(Direction, Normal);
					const float ReflectedDotView = VecUtils::dot(Reflected, ViewDirection);
					if (ReflectedDotView > 0)
					{
						Intensity += l.Intensity * 50.0f * std::pow(ReflectedDotView, Specular.value());
					}
				}
			}
		}

		return Intensity;
	}
}

namespace Raytracer {
	// Traces a ray through the scene
	RayPayload TraceRay(Scene& Scene, Ray R, float TMin, float TMax, int RecursionDepth) 
	{
		auto [ClosestT, ClosestSphere] = ClosestIntersection(Scene, R, TMin, TMax);
		if (!ClosestSphere)
			return RayPayload(ClosestT, Scene.BackgroundColor);
		
		// Compute local color
		const vec3 Point = R.Origin + (ClosestT * R.Direction);
		const vec3 Normal = VecUtils::normalize(Point - ClosestSphere->Origin);
		color4 LocalColor = ClosestSphere->Color * ComputeLighting(Scene, Point, Normal, -R.Direction, ClosestSphere->Specular);

		// Check if we should reflect; return if not
		if (RecursionDepth <= 0 || ClosestSphere->Reflective <= 0.0f)
			return RayPayload(ClosestT, LocalColor);

		// Recursively compute reflection
		Ray Reflected = Ray(Point + Normal * 1e-4f, Reflect(-R.Direction, Normal));
		color4 ReflectedColor = TraceRay(Scene, Reflected, 1e-6, std::numeric_limits<float>::max(), RecursionDepth - 1).Color;

		return RayPayload(ClosestT, LocalColor * (1 - ClosestSphere->Reflective) + ReflectedColor * ClosestSphere->Reflective);
	}
}