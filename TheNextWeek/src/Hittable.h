#pragma once

#include "AxisAlignedBoundingBox.h"
#include "Ray.h"

// ToDo:: add scaling of objects
class Material;

struct HitRecord
{
    Point3 hitPoint;
    Vec3 normal;
    std::shared_ptr<Material> matPtr;
    double t;
    double u;
    double v;
    bool frontFace;

    inline void FaceNormal(Ray const& r, Vec3 const& outwardNormal)
    {
        frontFace = DotProduct(r.Direction(), outwardNormal) < 0;
        normal = frontFace ? outwardNormal : -outwardNormal;
    }
};

class Hittable
{
public:
    virtual bool Hit(Ray const& r, double tMin, double tMax, HitRecord& rec) const = 0;
    virtual bool BoundingBox(double timeStart, double timeEnd, AxisAlignedBoundingBox& outputBox) const = 0;
};

class Translate : public Hittable
{
public:
    Translate(std::shared_ptr<Hittable> o, Vec3 const& os) : obj(o), offset(os) {}
    virtual bool Hit(Ray const& r, double tMin, double tMax, HitRecord& rec) const override;
    virtual bool BoundingBox(double timeStart, double timeEnd, AxisAlignedBoundingBox& outputBox) const override;

private:
    std::shared_ptr<Hittable> obj;
    Vec3 offset;
};

bool Translate::Hit(Ray const& r, double tMin, double tMax, HitRecord& rec) const
{
    Ray movedRay(r.Origin() - offset, r.Direction(), r.Time());
    if (!obj->Hit(movedRay, tMin, tMax, rec))
        return false;

    rec.hitPoint += offset;
    rec.FaceNormal(movedRay, rec.normal);

    return true;
}
bool Translate::BoundingBox(double timeStart, double timeEnd, AxisAlignedBoundingBox& outputBox) const
{
    if (!obj->BoundingBox(timeStart, timeEnd, outputBox))
        return false;

    outputBox = AxisAlignedBoundingBox(outputBox.Min() + offset, outputBox.Max() + offset);

    return true;
}

class RotateY : public Hittable
{
public:
    RotateY(std::shared_ptr<Hittable> o, double angle);

    virtual bool Hit(Ray const& r, double tMin, double tMax, HitRecord& rec) const override;

    virtual bool BoundingBox(double timeStart, double timeEnd, AxisAlignedBoundingBox& outputBox) const override;

private:
    std::shared_ptr<Hittable> obj;
    double sinTheta;
    double cosTheta;
    bool hasBox;
    AxisAlignedBoundingBox box;
};

RotateY::RotateY(std::shared_ptr<Hittable> o, double angle) : obj(o)
{
    auto radians = DegreesToRadians(angle);
    sinTheta = std::sin(radians);
    cosTheta = std::cos(radians);
    hasBox = obj->BoundingBox(0, 1, box);
    Point3 min(infinity, infinity, infinity);
    Point3 max(-infinity, -infinity, -infinity);

    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            for (int k = 0; k < 2; ++k)
            {
                auto x = i * box.Max().x() + (1 - i) * box.Min().x();
                auto y = j * box.Max().y() + (1 - j) * box.Min().y();
                auto z = k * box.Max().z() + (1 - k) * box.Min().z();

                auto newX = cosTheta * x + sinTheta * z;
                auto newZ = -sinTheta * x + cosTheta * z;

                Vec3 tester(newX, y, newZ);

                for (int c = 0; c < 3; ++c)
                {
                    min[c] = std::fmin(min[c], tester[c]);
                    max[c] = std::fmax(max[c], tester[c]);
                }
            }
        }
    }

    box = AxisAlignedBoundingBox(min, max);
}

bool RotateY::Hit(Ray const& r, double tMin, double tMax, HitRecord& rec) const
{
    auto origin = r.Origin();
    auto direction = r.Direction();

    origin[0] = cosTheta * r.Origin()[0] - sinTheta * r.Origin()[2];
    origin[2] = sinTheta * r.Origin()[0] + cosTheta * r.Origin()[2];

    direction[0] = cosTheta * r.Direction()[0] - sinTheta * r.Direction()[2];
    direction[2] = sinTheta * r.Direction()[0] + cosTheta * r.Direction()[2];

    Ray rotatedR(origin, direction, r.Time());

    if (!obj->Hit(rotatedR, tMin, tMax, rec))
        return false;

    auto hitPoint = rec.hitPoint;
    auto normal = rec.normal;

    hitPoint[0] = cosTheta * rec.hitPoint[0] + sinTheta * rec.hitPoint[2];
    hitPoint[2] = -sinTheta * rec.hitPoint[0] + cosTheta * rec.hitPoint[2];

    normal[0] = cosTheta * rec.normal[0] + sinTheta * rec.normal[2];
    normal[2] = -sinTheta * rec.normal[0] + cosTheta * rec.normal[2];

    rec.hitPoint = hitPoint;
    rec.FaceNormal(rotatedR, normal);

    return true;
}

bool RotateY::BoundingBox(double timeStart, double timeEnd, AxisAlignedBoundingBox& outputBox) const
{
    outputBox = box;
    return hasBox;
}
