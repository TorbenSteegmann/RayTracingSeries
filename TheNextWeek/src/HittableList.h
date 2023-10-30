#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "Hittable.h"

class HittableList : public Hittable
{
public:
    HittableList() : backgroundColor({0, 0, 0}) {}

    explicit HittableList(std::shared_ptr<Hittable> const& object) { Add(object); };

    [[nodiscard]] std::vector<std::shared_ptr<Hittable>> Objects() const { return objects; };
    void Clear() { objects.clear(); }
    void Add(std::shared_ptr<Hittable> const& object) { objects.emplace_back(object); }

    bool Hit(Ray const& r, double tMin, double tMax, HitRecord& rec) const override;

    bool BoundingBox(double timeStart, double timeEnd, AxisAlignedBoundingBox& outputBox) const override;

    [[nodiscard]] Color BackgroundColor() const { return backgroundColor; }
    void BackgroundColor(Color c) { backgroundColor = c; }

private:
    std::vector<std::shared_ptr<Hittable>> objects;
    Color backgroundColor = {0.70, 0.80, 1.0};
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

inline bool HittableList::BoundingBox(double timeStart, double timeEnd, AxisAlignedBoundingBox& outputBox) const
{
    if (objects.empty())
        return false;

    AxisAlignedBoundingBox tempBox;
    bool firstBox = true;

    for (auto const& object : objects)
    {
        if (!object->BoundingBox(timeStart, timeEnd, outputBox))
            return false;

        outputBox = firstBox ? tempBox : SurroundingBox(tempBox, outputBox);
        firstBox = false;
    }

    return true;
}
