#pragma once

#include <cmath>
#include <iostream>

#include "rtWeekend.h"

class Vec3
{
public:
    Vec3() : e{0, 0, 0} {};
    Vec3(double x, double y, double z) : e{x, y, z} {};

    [[nodiscard]] double x() const { return e[0]; };
    [[nodiscard]] double y() const { return e[1]; };
    [[nodiscard]] double z() const { return e[2]; };

    Vec3 operator-() const { return Vec3{-e[0], -e[1], -e[2]}; };
    double operator[](int i) const { return e[i]; };
    double& operator[](int i) { return e[i]; };

    Vec3& operator+=(Vec3 const& v)
    {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];

        return *this;
    }

    Vec3& operator*=(double const f)
    {
        e[0] *= f;
        e[1] *= f;
        e[2] *= f;

        return *this;
    }

    Vec3& operator/=(double const f) { return *this *= 1 / f; }

    [[nodiscard]] double Length() const { return std::sqrt(LengthSquared()); }

    [[nodiscard]] double LengthSquared() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }

    static Vec3 Random() { return {RandomDouble(), RandomDouble(), RandomDouble()}; }

    static Vec3 Random(double min, double max) { return {RandomDouble(min, max), RandomDouble(min, max), RandomDouble(min, max)}; }

    [[nodiscard]] bool IsNearZero() const
    {
        auto const s = 1e-8;
        return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s);
    }


private:
    double e[3];
};

using Point3 = Vec3;
using Color = Vec3;

inline std::ostream& operator<<(std::ostream& out, Vec3 const& v) { return out << v[0] << ' ' << v[1] << ' ' << v[2]; }

inline Vec3 operator+(Vec3 const& v1, Vec3 const& v2) { return {v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2]}; }

inline Vec3 operator-(Vec3 const& v1, Vec3 const& v2) { return {v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2]}; }

inline Vec3 operator*(Vec3 const& v1, Vec3 const& v2) { return {v1[0] * v2[0], v1[1] * v2[1], v1[2] * v2[2]}; }

inline Vec3 operator*(double const t, Vec3 const& v1) { return {v1[0] * t, v1[1] * t, v1[2] * t}; }

inline Vec3 operator*(Vec3 const& v1, double const t) { return t * v1; }

inline Vec3 operator/(Vec3 const& v1, double const t) { return (1 / t) * v1; }

inline double DotProduct(Vec3 const& v1, Vec3 const& v2) { return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2]; }

inline Vec3 CrossProduct(Vec3 const& v1, Vec3 const& v2)
{
    return {v1[1] * v2[2] - v1[2] * v2[1], v1[2] * v2[0] - v1[0] * v2[2], v1[0] * v2[1] - v1[1] * v2[0]};
}

inline Vec3 UnitVector(Vec3 const& v) { return v / v.Length(); }

inline Vec3 RandomInUnitSphere() // TODO:: This seems stupid, try to find a cheaper way later
{
    while (true)
    {
        auto p = Vec3::Random(-1, 1);
        if (p.LengthSquared() >= 1)
            continue;
        return p;
    }
}

inline Vec3 RandomUnitVector() { return UnitVector(RandomInUnitSphere()); }

inline Vec3 RandomInHemisphere(Vec3 const& normal)
{
    Vec3 inUnitSphere = RandomInUnitSphere();
    inUnitSphere = DotProduct(inUnitSphere, normal) > 0 ? inUnitSphere : -inUnitSphere;
    return inUnitSphere;
}

inline Vec3 Reflect(Vec3 const& v, Vec3 const& n) { return v - 2 * DotProduct(v, n) * n; }

inline Vec3 Refract(Vec3 const& uv, Vec3 const& n, double etaiOverEtat)
{
    auto cosTheta = std::fmin(DotProduct(-uv, n), 1.0);
    Vec3 rOutPerp = etaiOverEtat * (uv + cosTheta * n);
    Vec3 rOutParallel = -sqrt(std::fabs(1.0 - rOutPerp.LengthSquared())) * n;
    return rOutPerp + rOutParallel;
}

inline Vec3 RandomInUnitDisk()
{
    while (true)
    {
        auto p = Vec3(RandomDouble(-1, 1), RandomDouble(-1, 1), 0);
        if (p.LengthSquared() >= 1)
            continue;
        return p;
    }
}
