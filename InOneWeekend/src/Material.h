#pragma once

#include "Ray.h"
#include "Vec3.h"

struct HitRecord;

class Material
{
public:
    virtual bool Scatter(Ray const& rIn, HitRecord const& rec, Color& attenuation, Ray& scattered) const = 0;
};
