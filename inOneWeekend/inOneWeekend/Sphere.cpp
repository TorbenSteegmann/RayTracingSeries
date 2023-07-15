#include "Sphere.h"

bool Sphere::Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const
{
	Vec3 oc = r.origin() - center;
	auto a = r.direction().LengthSquared();
	auto halfB = DotProduct(oc, r.direction());
	auto c = oc.LengthSquared() - radius * radius;
	auto discriminant = halfB * halfB - a * c;

	if (discriminant < 0) return false;

	auto sqrtd = sqrt(discriminant);
	auto root = (-halfB - sqrtd) / a;
	if (root < tMin || root > tMax)
	{
		root = (-halfB + sqrtd) / a;
		if (root < tMin || root > tMax) return false;
	}

	rec.t = root;
	rec.hitPoint = r.at(rec.t);
	Vec3 outwardNormal = (rec.hitPoint - center) / radius;
	rec.setFaceNormal(r, outwardNormal);
	rec.matPtr = matPtr;

	return true;
}