#pragma once
#include <optional>

#include "VecUtils.hpp"
#include "Raytracer.hpp"

namespace Raytracer {
	RayPayload TraceRay(Scene& Scene, Ray Ray, float TMin, float TMax) 
	{
		float ClosestT = std::numeric_limits<float>::max();
		std::optional<Sphere> ClosestSphere;

		for (Sphere& s : Scene.Spheres)
		{
			auto [t1, t2] = Raytracer::RayIntersectSphere(Ray, s);
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

		if (!ClosestSphere)
			return RayPayload(ClosestT, Scene.BackgroundColor);
		return RayPayload(ClosestT, ClosestSphere->Color);
	}

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
}