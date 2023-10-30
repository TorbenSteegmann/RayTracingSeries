#pragma once

#include <algorithm>

#include "Hittable.h"
#include "HittableList.h"

class BvhNode : public Hittable
{
public:
    BvhNode();

    BvhNode(HittableList const& list, double timeStart, double timeEnd) : BvhNode(list.Objects(), 0, list.Objects().size(), timeStart, timeEnd){};

    BvhNode(std::vector<std::shared_ptr<Hittable>> const& srcObjects, size_t start, size_t end, double timeStart, double timeEnd);

    virtual bool Hit(Ray const& r, double tMin, double tMax, HitRecord& rec) const override;

    virtual bool BoundingBox(double timeStart, double timeEnd, AxisAlignedBoundingBox& outputBox) const override;


private:
    std::shared_ptr<Hittable> left;
    std::shared_ptr<Hittable> right;
    AxisAlignedBoundingBox box;
};

inline bool BoxCompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b, int axis)
{
    AxisAlignedBoundingBox boxA;
    AxisAlignedBoundingBox boxB;

    if (!a->BoundingBox(0, 0, boxA) || !b->BoundingBox(0, 0, boxB))
        std::cerr << "No bounding box in BvhNode constructor.\n";

    return boxA.Min()[axis] < boxB.Min()[axis];
}

inline bool BoxXCompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b) { return BoxCompare(a, b, 0); }
inline bool BoxYCompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b) { return BoxCompare(a, b, 1); }
inline bool BoxZCompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b) { return BoxCompare(a, b, 2); }

inline BvhNode::BvhNode(std::vector<std::shared_ptr<Hittable>> const& srcObjects, size_t start, size_t end, double timeStart, double timeEnd)
{
    auto objects = srcObjects;

    int axis = RandomInt(0, 2);
    auto comparator = (axis == 0) ? BoxXCompare : (axis == 1) ? BoxYCompare : BoxZCompare;

    size_t objectSpan = end - start;

    if (objectSpan == 1)
    {
        left = right = objects[start];
    }
    else if (objectSpan == 2)
    {
        if (comparator(objects[start], objects[start + 1]))
        {
            left = objects[start];
            right = objects[start + 1];
        }
        else
        {
            left = objects[start + 1];
            right = objects[start];
        }
    }
    else
    {
        std::sort(objects.begin() + start, objects.begin() + end, comparator);

        auto mid = start + objectSpan / 2;
        left = std::make_shared<BvhNode>(objects, start, mid, timeStart, timeEnd);
        right = std::make_shared<BvhNode>(objects, mid, end, timeStart, timeEnd);
    }

    AxisAlignedBoundingBox boxLeft;
    AxisAlignedBoundingBox boxRight;

    if (!left->BoundingBox(timeStart, timeEnd, boxLeft) || !right->BoundingBox(timeStart, timeEnd, boxRight))
        std::cerr << "No bounding box in BvhNode constructor.\n";

    box = SurroundingBox(boxLeft, boxRight);
}


inline bool BvhNode::BoundingBox(double timeStart, double timeEnd, AxisAlignedBoundingBox& outputBox) const
{
    outputBox = box;
    return true;
}

inline bool BvhNode::Hit(Ray const& r, double tMin, double tMax, HitRecord& rec) const
{
    if (!box.Hit(r, tMin, tMax))
        return false;

    bool hitLeft = left->Hit(r, tMin, tMax, rec);
    bool hitRight = right->Hit(r, tMin, hitLeft ? rec.t : tMax, rec);

    return hitLeft || hitRight;
}
