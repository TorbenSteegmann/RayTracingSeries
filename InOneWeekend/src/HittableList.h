#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "Hittable.h"

class HittableList : public Hittable
{
public:
    HittableList() = default;
    ;
    explicit HittableList(std::shared_ptr<Hittable> const& object) { Add(object); };

    void clear() { objects.clear(); }
    void Add(std::shared_ptr<Hittable> const& object) { objects.emplace_back(object); }

    bool Hit(Ray const& r, double tMin, double tMax, HitRecord& rec) const override;


private:
    std::vector<std::shared_ptr<Hittable>> objects;
};

inline bool HittableList::Hit(Ray const& r, double tMin, double tMax, HitRecord& rec) const
{
    HitRecord tempRec;
    bool hitAnything = false;
    auto currentClosest = tMax;

    for (auto const& object : objects)
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
