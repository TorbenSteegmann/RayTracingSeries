#pragma once

#include <cmath>
#include <iostream>

#include "rtWeekend.h"

class Vec3
{
public:
	Vec3() : e{ 0,0,0 } {};
	Vec3(double x, double y, double z) : e{ x,y,z } {};

	double x() const { return e[0]; };
	double y() const { return e[1]; };
	double z() const { return e[2]; };

	Vec3 operator-() const { return Vec3{ -e[0], -e[1], -e[2] }; };
	double operator[](int i) const { return e[i]; };
	double& operator[](int i) { return e[i]; };

	Vec3& operator+=(const Vec3& v)
	{
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
	
		return *this; 
	}

	Vec3& operator*=(const double f)
	{
		e[0] *= f;
		e[1] *= f;
		e[2] *= f;

		return *this;
	}

	Vec3& operator/=(const double f)
	{
		return *this *= 1/f;
	}

	double Length() const
	{
		return std::sqrt(LengthSquared());
	}

	double LengthSquared() const
	{
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	static Vec3 Random()
	{
		return Vec3(RandomDouble(), RandomDouble(), RandomDouble());
	}

	static Vec3 Random(double min, double max)
	{
		return Vec3(RandomDouble(min, max), RandomDouble(min, max), RandomDouble(min, max));
	}

	bool IsNearZero() const {
		const auto s = 1e-8;
		return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s);
	}

	


private:
	double e[3];
};

using Point3 = Vec3;
using Color = Vec3;

inline std::ostream& operator<<(std::ostream& out, const Vec3& v)
{
	return out << v[0] << ' ' << v[1] << ' ' << v[2];
}

inline Vec3 operator+(const Vec3& v1, const Vec3& v2)
{
	return Vec3(v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2]);
}

inline Vec3 operator-(const Vec3& v1, const Vec3& v2)
{
	return Vec3(v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2]);
}

inline Vec3 operator*(const Vec3& v1, const Vec3& v2)
{
	return Vec3(v1[0] * v2[0], v1[1] * v2[1], v1[2] * v2[2]);
}

inline Vec3 operator*(const double t, const Vec3& v1)
{
	return Vec3(v1[0] * t, v1[1] * t, v1[2] * t);
}

inline Vec3 operator*(const Vec3& v1, const double t)
{
	return t * v1;
}

inline Vec3 operator/(const Vec3& v1, const double t)
{
	return (1 / t) * v1;
}

inline double DotProduct(const Vec3& v1, const Vec3& v2)
{
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

inline Vec3 CrossProduct(const Vec3& v1, const Vec3& v2)
{
	return Vec3(v1[1] * v2[2] - v1[2] * v2[1],
				v1[2] * v2[0] - v1[0] * v2[2],
				v1[0] * v2[1] - v1[1] * v2[0]);
}

inline Vec3 UnitVector(const Vec3& v)
{
	return v / v.Length();
}

inline Vec3 RandomInUnitSphere() //TODO:: This seems stupid, try to find a cheaper way later
{
	while (true)
	{
		auto p = Vec3::Random(-1, 1);
		if (p.LengthSquared() >= 1) continue;
		return p;
	}
}

inline Vec3 RandomUnitVector() { return UnitVector(RandomInUnitSphere()); }

inline Vec3 RandomInHemisphere(const Vec3& normal) 
{
	Vec3 inUnitSphere = RandomInUnitSphere();
	inUnitSphere = DotProduct(inUnitSphere, normal) > 0 ? inUnitSphere : -inUnitSphere;
	return inUnitSphere;
}

inline Vec3 Reflect(const Vec3& v, const Vec3& n)
{
	return v - 2 * DotProduct(v, n) * n;
}

inline Vec3 Refract(const Vec3& uv, const Vec3& n, double etaiOverEtat)
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
		if (p.LengthSquared() >= 1) continue;
		return p;
	}
}