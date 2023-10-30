#pragma once

#include "Hittable.h"
class XyRect : public Hittable
{
public:
    XyRect() {}
    XyRect(double xMin, double xMax, double yMin, double yMax, double z, std::shared_ptr<Material> m)
      : x0(xMin), x1(xMax), y0(yMin), y1(yMax), z(z), mat(m)
    {
    }

    virtual bool Hit(Ray const& r, double tMin, double tMax, HitRecord& rec) const override;

    virtual bool BoundingBox(double timeStart, double timeEnd, AxisAlignedBoundingBox& outputBox) const override;

private:
    std::shared_ptr<Material> mat;
    double x0, x1;
    double y0, y1;
    double z;
};


class XzRect : public Hittable
{
public:
    XzRect() {}

    XzRect(double xMin, double xMax, double zMin, double zMax, double y, std::shared_ptr<Material> m)
      : x0(xMin), x1(xMax), z0(zMin), z1(zMax), y(y), mat(m)
    {
    }

    virtual bool Hit(Ray const& r, double tMin, double tMax, HitRecord& rec) const override;

    virtual bool BoundingBox(double timeStart, double timeEnd, AxisAlignedBoundingBox& outputBox) const override;

private:
    std::shared_ptr<Material> mat;
    double x0, x1;
    double z0, z1;
    double y;
};


class YzRect : public Hittable
{
public:
    YzRect() {}

    YzRect(double yMin, double yMax, double zMin, double zMax, double x, std::shared_ptr<Material> m)
      : y0(yMin), y1(yMax), z0(zMin), z1(zMax), x(x), mat(m)
    {
    }

    virtual bool Hit(Ray const& r, double tMin, double tMax, HitRecord& rec) const override;

    virtual bool BoundingBox(double timeStart, double timeEnd, AxisAlignedBoundingBox& outputBox) const override;

private:
    std::shared_ptr<Material> mat;
    double y0, y1;
    double z0, z1;
    double x;
};


bool XyRect::Hit(Ray const& r, double tMin, double tMax, HitRecord& rec) const
{
    auto t = (z - r.Origin().z()) / (r.Direction().z());
    if (t < tMin || t > tMax)
        return false;

    auto x = r.Origin().x() + t * r.Direction().x();
    auto y = r.Origin().y() + t * r.Direction().y();

    if (x < x0 || x > x1 || y < y0 || y > y1)
        return false;

    rec.u = (x - x0) / (x1 - x0);
    rec.v = (y - y0) / (y1 - y0);
    rec.t = t;
    Vec3 outwardNormal = {0, 0, 1};
    rec.FaceNormal(r, outwardNormal);
    rec.matPtr = mat;
    rec.hitPoint = r.At(t);

    return true;
}


bool XyRect::BoundingBox(double timeStart, double timeEnd, AxisAlignedBoundingBox& outputBox) const
{
    outputBox = AxisAlignedBoundingBox({x0, y0, z - 0.0001}, {x1, y1, z + 0.0001});
    return true;
}

bool XzRect::Hit(Ray const& r, double tMin, double tMax, HitRecord& rec) const
{
    auto t = (y - r.Origin().y()) / (r.Direction().y());
    if (t < tMin || t > tMax)
        return false;

    auto x = r.Origin().x() + t * r.Direction().x();
    auto z = r.Origin().z() + t * r.Direction().z();

    if (x < x0 || x > x1 || z < z0 || z > z1)
        return false;

    rec.u = (x - x0) / (x1 - x0);
    rec.v = (z - z0) / (z1 - z0);
    rec.t = t;
    Vec3 outwardNormal = {0, 1, 0};
    rec.FaceNormal(r, outwardNormal);
    rec.matPtr = mat;
    rec.hitPoint = r.At(t);

    return true;
}


bool XzRect::BoundingBox(double timeStart, double timeEnd, AxisAlignedBoundingBox& outputBox) const
{
    outputBox = AxisAlignedBoundingBox({x0, y - 0.0001, z0}, {x1, y + 0.0001, z1});
    return true;
}

bool YzRect::Hit(Ray const& r, double tMin, double tMax, HitRecord& rec) const
{
    auto t = (x - r.Origin().x()) / (r.Direction().x());
    if (t < tMin || t > tMax)
        return false;

    auto y = r.Origin().y() + t * r.Direction().y();
    auto z = r.Origin().z() + t * r.Direction().z();

    if (y < y0 || y > y1 || z < z0 || z > z1)
        return false;

    rec.u = (y - y0) / (y1 - y0);
    rec.v = (z - z0) / (z1 - z0);
    rec.t = t;
    Vec3 outwardNormal = {1, 0, 0};
    rec.FaceNormal(r, outwardNormal);
    rec.matPtr = mat;
    rec.hitPoint = r.At(t);

    return true;
}


bool YzRect::BoundingBox(double timeStart, double timeEnd, AxisAlignedBoundingBox& outputBox) const
{
    outputBox = AxisAlignedBoundingBox({x - 0.0001, y0, z0}, {x + 0.0001, y1, z1});
    return true;
}
