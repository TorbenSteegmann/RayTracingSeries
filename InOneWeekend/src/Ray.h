#pragma once

#include "Vec3.h"

class Ray
{
public:
    Ray() = default;
    Ray(Point3 const& origin, Vec3 const& direction, double time = 0.0) : orig(origin), dir(direction) {}

    [[nodiscard]] Point3 origin() const { return orig; }
    [[nodiscard]] Vec3 direction() const { return dir; }

    [[nodiscard]] Point3 at(double t) const { return orig + t * dir; }

private:
    Point3 orig;
    Vec3 dir;
};
