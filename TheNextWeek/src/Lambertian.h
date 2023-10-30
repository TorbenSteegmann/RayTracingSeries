#pragma once

#include "Hittable.h"
#include "Material.h"
#include "Textures.h"

class Lambertian : public Material
{
public:
    explicit Lambertian(Color const& a) : albedo(std::make_shared<SolidColor>(a)) {}
    explicit Lambertian(std::shared_ptr<Texture> a) : albedo(a) {}

    bool Scatter(Ray const& rIn, HitRecord const& rec, Color& attenuation, Ray& scattered) const override
    {
        auto scatterDirection = rec.normal + RandomUnitVector();

        if (scatterDirection.IsNearZero())
            scatterDirection = rec.normal;

        scattered = Ray(rec.hitPoint, scatterDirection, rIn.Time());
        attenuation = albedo->Value(rec.u, rec.v, rec.hitPoint);
        return true;
    }

private:
    std::shared_ptr<Texture> albedo;
};
