#pragma once

class Interval
{
public:
    bool contains(double x) { return min <= x && x <= max; }

private:
    double min;
    double max;
};
