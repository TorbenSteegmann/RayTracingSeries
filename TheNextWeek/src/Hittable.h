#pragma once

#include "Ray.h"
#include "rtWeekend.h"

class Material;

struct HitRecord
{
	Point3 hitPoint;
	Vec3 normal;
	std::shared_ptr<Material> matPtr;
	double t;
	bool frontFace;

	inline void setFaceNormal(const Ray& r, const Vec3& outwardNormal)
	{
		frontFace = DotProduct(r.direction(), outwardNormal) < 0;
		normal = frontFace ? outwardNormal : -outwardNormal;
	}
};

class Hittable
{
public:
	virtual bool Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const = 0;
};