#pragma once

#include "Material.h"
#include "Hittable.h"
#include "Vec3.h"

class Metal : public Material
{
public:
	explicit Metal(const Color& a, double f = 0) : albedo(a), fuzz(f < 1 ? f : 1) {}

	bool Scatter
	(const Ray& rIn, const HitRecord& rec, Color& attenuation, Ray& scattered) const override
	{
		Vec3 reflected = Reflect(UnitVector(rIn.direction()), rec.normal);
		scattered = Ray(rec.hitPoint, reflected + fuzz * RandomInUnitSphere());
		attenuation = albedo;
		return (DotProduct(scattered.direction(), rec.normal) > 0);
	}
private:
	Color albedo;
	double fuzz;
};

