#pragma once

#include <memory>
#include <vector>

#include "Hittable.h"

class HittableList : public Hittable
{
public:
	HittableList() {};
	HittableList(std::shared_ptr<Hittable> object) { Add(object); };

	void clear() { objects.clear(); }
	void Add(std::shared_ptr<Hittable> object) { objects.emplace_back(object); }
	
	virtual bool Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const override;



private:
	std::vector<std::shared_ptr<Hittable>> objects;
};

