#pragma once

#include "Hittable.h"
#include "Ray.h"

class Sphere : public Hittable
{
public:
	Sphere() {};
	Sphere(Point3 cen, double r, std::shared_ptr<Material> m) : center(cen), radius(r), matPtr(m) {};

	virtual bool Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const override;

private:
	Point3 center;
	double radius;
	std::shared_ptr<Material> matPtr;
};

