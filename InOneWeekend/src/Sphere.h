#pragma once

#include <utility>

#include "Hittable.h"
#include "Ray.h"

class Sphere : public Hittable
{
public:
    Sphere() = default;
    // stationary Sphere (is this realistically gonna make a difference in time? I doubt it TODO Replace later)
    Sphere(Point3 cen, double r, std::shared_ptr<Material> m) : center(cen), radius(r), matPtr(std::move(m)){};
    bool Hit(Ray const& r, double tMin, double tMax, HitRecord& rec) const override;

private:
    Point3 center;
    double radius;
    std::shared_ptr<Material> matPtr;
};

inline bool Sphere::Hit(Ray const& r, double tMin, double tMax, HitRecord& rec) const
{
    Vec3 oc = r.origin() - center;
    auto a = r.direction().LengthSquared();
    auto halfB = DotProduct(oc, r.direction());
    auto c = oc.LengthSquared() - radius * radius;
    auto discriminant = halfB * halfB - a * c;

    if (discriminant < 0)
        return false;

    auto sqrtd = sqrt(discriminant);
    auto root = (-halfB - sqrtd) / a;
    if (root < tMin || root > tMax)
    {
        root = (-halfB + sqrtd) / a;
        if (root < tMin || root > tMax)
            return false;
    }

    rec.t = root;
    rec.hitPoint = r.at(rec.t);
    Vec3 outwardNormal = (rec.hitPoint - center) / radius;
    rec.setFaceNormal(r, outwardNormal);
    rec.matPtr = matPtr;

    return true;
}
