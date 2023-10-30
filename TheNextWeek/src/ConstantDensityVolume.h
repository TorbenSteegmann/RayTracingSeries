#pragma once

#include "Hittable.h"
#include "Material.h"
#include "Textures.h"

class ConstantDensityVolume : public Hittable
{
public:
    ConstantDensityVolume(std::shared_ptr<Hittable> b, double density, std::shared_ptr<Texture> tex)
      : boundary(b), negInvDensity(-1 / density), phaseFunction(std::make_shared<Isotropic>(tex)){};
    ConstantDensityVolume(std::shared_ptr<Hittable> b, double density, Color c)
      : boundary(b), negInvDensity(-1 / density), phaseFunction(std::make_shared<Isotropic>(c)){};

    virtual bool Hit(Ray const& r, double tMin, double tMax, HitRecord& rec) const override;
    virtual bool BoundingBox(double timeStart, double timeEnd, AxisAlignedBoundingBox& outputBox) const override
    {
        return boundary->BoundingBox(timeStart, timeEnd, outputBox);
    }

private:
    std::shared_ptr<Hittable> boundary;
    std::shared_ptr<Material> phaseFunction;
    double negInvDensity;
};

bool ConstantDensityVolume::Hit(Ray const& r, double tMin, double tMax, HitRecord& rec) const
{
    HitRecord recOne;
    HitRecord recTwo;

    if (!boundary->Hit(r, -infinity, infinity, recOne))
        return false;

    if (!boundary->Hit(r, recOne.t + 0.0001, infinity, recTwo))
        return false;

    if (recOne.t < tMin)
        recOne.t = tMin;
    if (recTwo.t > tMax)
        recTwo.t = tMax;

    if (recOne.t >= recTwo.t)
        return false;

    if (recOne.t < 0)
        recOne.t = 0;

    auto const RAY_LENGTH = r.Direction().Length();
    auto const DISTANCE_INSIDE_BOUNDARY = (recTwo.t - recOne.t) * RAY_LENGTH;
    auto const HIT_DISTANCE = negInvDensity * std::log(RandomDouble());
    if (HIT_DISTANCE > DISTANCE_INSIDE_BOUNDARY)
        return false;
    rec.t = recOne.t + HIT_DISTANCE / RAY_LENGTH;
    rec.hitPoint = r.At(rec.t);

    rec.normal = Vec3(1, 0, 0); // arbitrary
    rec.frontFace = true;       // arbitrary
    rec.matPtr = phaseFunction;

    return true;
}
