#pragma once

#include "rtWeekend.h"
#include "Ray.h"
#include "Vec3.h"

struct HitRecord;

class Material
{
public:
	virtual bool Scatter
	(const Ray& rIn, const HitRecord& rec, Color& attenuation, Ray& scattered) const = 0;

};

