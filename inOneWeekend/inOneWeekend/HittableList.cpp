#include "HittableList.h"


bool HittableList::Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const
{
	HitRecord tempRec;
	bool hitAnything = false;
	auto currentClosest = tMax;

	for (const auto& object : objects)
	{
		if (object->Hit(r, tMin, currentClosest, tempRec))
		{
			hitAnything = true;
			currentClosest = tempRec.t;
			rec = tempRec;
		}
	}

	return hitAnything;
}