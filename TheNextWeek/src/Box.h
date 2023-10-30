#pragma once

#include "AaRect.h"
#include "Hittable.h"
#include "HittableList.h"
#include "Vec3.h"

class Box : public Hittable
{
public:
    Box() {}

    Box(Point3 const& p0, Point3 const& p1, std::shared_ptr<Material> mat);

    virtual bool Hit(Ray const& r, double tMin, double tMax, HitRecord& rec) const override { return sides.Hit(r, tMin, tMax, rec); }

    virtual bool BoundingBox(double timeStart, double timeEnd, AxisAlignedBoundingBox& outputBox) const override
    {
        outputBox = {boxMin, boxMax};
        return true;
    }

private:
    Point3 boxMin;
    Point3 boxMax;
    HittableList sides;
};

Box::Box(Point3 const& p0, Point3 const& p1, std::shared_ptr<Material> mat)
{
    boxMin = p0;
    boxMax = p1;

    sides.Add(std::make_shared<XyRect>(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), mat));
    sides.Add(std::make_shared<XyRect>(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), mat));

    sides.Add(std::make_shared<XzRect>(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), mat));
    sides.Add(std::make_shared<XzRect>(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), mat));

    sides.Add(std::make_shared<YzRect>(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), mat));
    sides.Add(std::make_shared<YzRect>(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), mat));
}
