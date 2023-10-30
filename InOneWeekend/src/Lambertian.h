#pragma once

#include "Material.h"
#include "Hittable.h"

class Lambertian : public Material
{
public:
    explicit Lambertian(Color const& a) : albedo(a) {}

    bool Scatter(Ray const& rIn, HitRecord const& rec, Color& attenuation, Ray& scattered) const override
    {
        auto scatterDirection = rec.normal + RandomUnitVector();

        if (scatterDirection.IsNearZero())
            scatterDirection = rec.normal;

        scattered = Ray(rec.hitPoint, scatterDirection);
        attenuation = albedo;
        return true;
    }

private:
	Color albedo;
};

