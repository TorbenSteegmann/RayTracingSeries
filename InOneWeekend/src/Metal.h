#pragma once

#include "Hittable.h"
#include "Material.h"
#include "Vec3.h"

class Metal : public Material
{
public:
    explicit Metal(Color const& a, double f = 0) : albedo(a), fuzz(f < 1 ? f : 1) {}

    bool Scatter(Ray const& rIn, HitRecord const& rec, Color& attenuation, Ray& scattered) const override
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
