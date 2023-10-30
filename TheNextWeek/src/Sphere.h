#pragma once

#include <cmath>

#include "AxisAlignedBoundingBox.h"
#include "Hittable.h"
#include "Ray.h"
#include "rtWeekend.h"

class Sphere : public Hittable
{
public:
    Sphere() = default;
    // stationary Sphere (is this realistically gonna make a difference in time? I doubt it TODO Replace later)
    Sphere(Point3 cenS, double r, std::shared_ptr<Material> m) : centerStart(cenS), radius(r), matPtr(std::move(m)), isMoving(false){};
    // moving Sphere
    Sphere(Point3 cenS, Point3 cenE, double r, std::shared_ptr<Material> m) : centerStart(cenS), radius(r), matPtr(std::move(m)), isMoving(true)
    {
        centerVec = cenE - cenS;
    };

    bool Hit(Ray const& r, double tMin, double tMax, HitRecord& rec) const override;
    bool BoundingBox(double timeStart, double timeEnd, AxisAlignedBoundingBox& outputBox) const override;


private:
    Point3 centerStart;
    double radius;
    std::shared_ptr<Material> matPtr;
    bool isMoving;
    Vec3 centerVec;

    Point3 centerCurrent(double time) const { return centerStart + time * centerVec; }

    static void SphereUV(Point3 const& p, double& u, double& v)
    {
        auto theta = std::acos(-p.y());
        auto phi = std::atan2(-p.z(), p.x()) + pi;

        u = phi / (2 * pi);
        v = theta / pi;
    }
};

inline bool Sphere::Hit(Ray const& r, double tMin, double tMax, HitRecord& rec) const
{
    Point3 center = isMoving ? centerCurrent(r.Time()) : centerStart;
    Vec3 oc = r.Origin() - center;
    auto a = r.Direction().LengthSquared();
    auto halfB = DotProduct(oc, r.Direction());
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
    rec.hitPoint = r.At(rec.t);
    Vec3 outwardNormal = (rec.hitPoint - center) / radius;
    rec.FaceNormal(r, outwardNormal);
    rec.matPtr = matPtr;
    SphereUV(outwardNormal, rec.u, rec.v);

    return true;
}

inline bool Sphere::BoundingBox(double timeStart, double timeEnd, AxisAlignedBoundingBox& outputBox) const
{
    AxisAlignedBoundingBox boxStart = {centerStart - Vec3(radius, radius, radius), centerStart + Vec3(radius, radius, radius)};
    AxisAlignedBoundingBox boxEnd = {centerCurrent(timeEnd) - Vec3(radius, radius, radius), centerCurrent(timeEnd) + Vec3(radius, radius, radius)};
    outputBox = SurroundingBox(boxStart, boxEnd);
    return true;
}
