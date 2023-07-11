#pragma once

#include "Hittable.h"
#include "Ray.h"

class Sphere : public Hittable
{
public:
	Sphere() {};
	Sphere(Point3 cen, double r) : center(cen), radius(r) {};

	virtual bool Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const override;

private:
	Point3 center;
	double radius;
};

