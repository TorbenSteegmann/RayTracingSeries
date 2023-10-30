#pragma once

#include "Vec3.h"

class Ray
{
public:
    Ray() = default;
    Ray(Point3 const& origin, Vec3 const& direction, double time = 0.0) : orig(origin), dir(direction), tm(time) {}

    [[nodiscard]] Point3 Origin() const { return orig; }
    [[nodiscard]] Vec3 Direction() const { return dir; }
    [[nodiscard]] double Time() const { return tm; }
    [[nodiscard]] Point3 At(double t) const { return orig + t * dir; }

private:
    Point3 orig;
    Vec3 dir;
    double tm;
};
