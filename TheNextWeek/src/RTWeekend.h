#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <random>

// Constants
double const infinity = std::numeric_limits<double>::infinity();
double const pi = 3.1415926535897932385; // Try to replace pi later (tau or turns)


// Utility Functions
inline double DegreesToRadians(double degrees) { return degrees * pi / 180.0; }
inline double RandomDouble()
{
    static std::minstd_rand eng{std::random_device{}()};
    return (double)eng() / (std::minstd_rand::max() + 1);
}
inline double RandomDouble(double min, double max) { return min + (max - min) * RandomDouble(); }
inline int RandomInt(int min, int max) { return static_cast<int>(RandomDouble(min, max + 1)); }
inline double Clamp(double x, double min, double max)
{
    if (x < min)
        return min;
    if (x > max)
        return max;
    return x;
}

// Common Headers
