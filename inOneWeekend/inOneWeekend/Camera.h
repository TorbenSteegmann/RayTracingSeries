#pragma once

#include "rtWeekend.h"
#include "Ray.h"
#include "Vec3.h"

class Camera
{
public:
	Camera
	(
		const Point3& lookFrom, const Point3& lookAt, const Vec3& vUp, //v=vertical
		double vfov, double aspectRatio, double aperture, double focusDist
	) 
	{
		auto theta = DegreesToRadians(vfov);
		auto h = std::tan(theta / 2);
		auto viewportHeight = 2.0 * h;
		auto viewportWidth = aspectRatio * viewportHeight;

		w = UnitVector(lookFrom - lookAt);
		u = UnitVector(CrossProduct(vUp, w));
		v = CrossProduct(w, u);

		origin = lookFrom;
		horizontal = focusDist * viewportWidth * u;
		vertical = focusDist * viewportHeight * v;
		lowerLeftCorner = origin - horizontal / 2 - vertical / 2 - focusDist * w;

		lensRadius = aperture / 2;
	}

	Ray GetRay(double s, double t) const
	{
		Vec3 rd = lensRadius * RandomInUnitDisk();
		Vec3 offset = u * rd.x() + v * rd.y();
		return 
			Ray
			(
				origin + offset, 
				lowerLeftCorner + s * horizontal + t * vertical - origin - offset
			);
	}

private:
	Point3 origin;
	Point3 lowerLeftCorner;
	Vec3 horizontal;
	Vec3 vertical;
	Vec3 u, v, w;
	double lensRadius;
};

