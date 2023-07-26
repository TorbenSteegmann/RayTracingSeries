#pragma once

#include "Material.h"
#include "Ray.h"
#include "Hittable.h"

class Dielectric : public Material
{
public:
	explicit Dielectric(double refid) : refractionIndex(refid) { };

	bool Scatter
	(const Ray& rIn, const HitRecord& rec, Color& attenuation, Ray& scattered) const override
	{
		attenuation = Color(1.0, 1.0, 1.0);
		double refractionRatio = rec.frontFace ? (1.0 / refractionIndex) : refractionIndex;
		Vec3 unitDirection = UnitVector(rIn.direction());
		double cosTheta = std::fmin(DotProduct(-unitDirection, rec.normal), 1.0);
		double sinTheta = sqrt(1.0 - cosTheta * cosTheta);

		bool cannotRefract = refractionRatio * sinTheta > 1.0;
		Vec3 direction;

		if (cannotRefract || Reflectance(cosTheta, refractionRatio) > RandomDouble()) 
			direction = Reflect(unitDirection, rec.normal);
		else direction = Refract(unitDirection, rec.normal, refractionRatio);

		scattered = Ray(rec.hitPoint, direction);
		return true;
	}

private:
	double refractionIndex;

	static double Reflectance(double cosine, double refId)
	{
		//Schlick approximation for reflectance
		auto r0 = (1 - refId) / (1 + refId);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}
};