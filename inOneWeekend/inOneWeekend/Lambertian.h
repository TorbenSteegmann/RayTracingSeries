#pragma once

#include "Material.h"
#include "Hittable.h"

class Lambertian : public Material
{
public:
	Lambertian(const Color& a) : albedo(a) {}

	virtual bool Scatter
	(const Ray& rIn, const HitRecord& rec, Color& attenuation, Ray& scattered) const override
	{
		auto scatterDirection = rec.normal + RandomUnitVector();

		if (scatterDirection.IsNearZero()) scatterDirection = rec.normal;

		scattered = Ray(rec.hitPoint, scatterDirection);
		attenuation = albedo;
		return true;
	}

private:
	Color albedo;
};

