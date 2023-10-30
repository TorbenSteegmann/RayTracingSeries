#pragma once

#include "Hittable.h"
#include "Ray.h"
#include "Textures.h"
#include "Vec3.h"

class Material
{
public:
    virtual bool Scatter(Ray const& rIn, HitRecord const& rec, Color& attenuation, Ray& scattered) const = 0;
    virtual Color Emitted(double u, double v, Point3 const& p) const { return {0, 0, 0}; }
};


class DiffuseLight : public Material
{
public:
    DiffuseLight(std::shared_ptr<Texture> a) : emit(a) {}
    DiffuseLight(Color c) : emit(std::make_shared<SolidColor>(c)) {}

    virtual bool Scatter(Ray const& rIn, HitRecord const& rec, Color& attenuation, Ray& scattered) const override { return false; }
    virtual Color Emitted(double u, double v, Point3 const& p) const override { return emit->Value(u, v, p); }


private:
    std::shared_ptr<Texture> emit;
};

class Isotropic : public Material
{
public:
    Isotropic(Color c) : albedo(std::make_shared<SolidColor>(c)) {}
    Isotropic(std::shared_ptr<Texture> a) : albedo(a) {}

    virtual bool Scatter(Ray const& rIn, HitRecord const& rec, Color& attenuation, Ray& scattered) const override
    {
        scattered = Ray(rec.hitPoint, RandomInUnitSphere(), rIn.Time());
        attenuation = albedo->Value(rec.u, rec.v, rec.hitPoint);
        return true;
    }

private:
    std::shared_ptr<Texture> albedo;
};
