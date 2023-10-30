#pragma once

#include "Ray.h"
#include "Vec3.h"

class AxisAlignedBoundingBox
{
public:
    AxisAlignedBoundingBox() {}

    AxisAlignedBoundingBox(Point3 const& min, Point3 const& max)
    {
        minimum = min;
        maximum = max;
    }

    Point3 Min() const { return minimum; }
    Point3 Max() const { return maximum; }

    bool Hit(Ray const& r, double tMin, double tMax) const
    {
        for (int a = 0; a < 3; ++a)
        {
            auto inverseDirection = 1.0f / r.Direction()[a];
            auto t0 = (minimum[a] - r.Origin()[a]) * inverseDirection;
            auto t1 = (maximum[a] - r.Origin()[a]) * inverseDirection;

            if (inverseDirection < 0.0f)
                std::swap(t0, t1);

            tMin = t0 > tMin ? t0 : tMin;
            tMax = t1 < tMax ? t1 : tMax;

            if (tMax <= tMin)
                return false;
        }
        return true;
    }


private:
    Point3 minimum;
    Point3 maximum;
};


inline AxisAlignedBoundingBox SurroundingBox(AxisAlignedBoundingBox boxOne, AxisAlignedBoundingBox boxTwo)
{
    Point3 lowPoint = {fmin(boxOne.Min().x(), boxTwo.Min().x()), fmin(boxOne.Min().y(), boxTwo.Min().y()), fmin(boxOne.Min().z(), boxTwo.Min().z())};
    Point3 highPoint = {fmax(boxOne.Max().x(), boxTwo.Max().x()), fmax(boxOne.Max().y(), boxTwo.Max().y()), fmax(boxOne.Max().z(), boxTwo.Max().z())};

    return {lowPoint, highPoint};
}
