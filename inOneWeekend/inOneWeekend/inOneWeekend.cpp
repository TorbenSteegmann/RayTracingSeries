#include <iostream>
#include <string>

#include "rtWeekend.h"
#include "Ray.h"
#include "Vec3.h"
#include "Camera.h"
#include "Color.h"
#include "HittableList.h"
#include "Sphere.h"

inline void DisplayProgess(int IMAGE_HEIGHT, int scanline) 
{
    std::string progressString = "Scanlines remaining: " + std::to_string(scanline);
    std::string progressBar = "|";
    int progressInPercent = (int) (((double(IMAGE_HEIGHT - scanline)) / IMAGE_HEIGHT) * 100);
    for (int i = 2; i <= progressInPercent; i += 2)
    {
        progressBar += (char)178;
    }
    for (int i = progressInPercent % 2 == 1 ? progressInPercent - 1 : progressInPercent; i < 100; i += 2)
    {
        progressBar += ' ';
    }
    progressBar.append("|(" + std::to_string(progressInPercent) + "%, " + progressString + ')');
    std::cerr << "\r" << progressBar << ' ' << std::flush;
}


inline Color RayColor(const Ray& r, const Hittable& world, int depth, int diffusionMethod) //Why is this not in Color class?
{
    if (depth <= 0) return Color(1, 1, 1);

    HitRecord rec;
    if (world.Hit(r, 0.001, infinity, rec))
    {
        Point3 target = rec.normal + (diffusionMethod == 0 ? RandomUnitVector() : RandomInHemisphere(rec.normal));
        return 0.5 * RayColor(Ray(rec.hitPoint, target), world, depth-1, diffusionMethod);
    }
    Vec3 unitDirection = UnitVector(r.direction());
    auto t = 0.5 * (unitDirection.y() + 1.0);
    return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

int main() 
{
    //Testing
    
    //Image 

    const auto ASPECT_RATIO = 16.0 / 9.0;
    const int IMAGE_WIDTH = 400;
    const int IMAGE_HEIGHT = static_cast<int>(IMAGE_WIDTH/ASPECT_RATIO);
    const int SAMPLES_PER_PIXEL = 100;
    const int MAX_DEPTH = 50;
    const enum DiffusionMethod{UNIT_SPHERE, HEMISPHERE};


    //World

    HittableList world;
    world.Add(std::make_shared<Sphere>(Point3(0, 0, -1), 0.5));
    world.Add(std::make_shared<Sphere>(Point3(0, -100.5, -1), 100));


    //Camera

    Camera cam;


    //Render

    std::cout << "P3\n" << IMAGE_WIDTH << ' ' << IMAGE_HEIGHT << "\n255\n";

    for (int j = IMAGE_HEIGHT - 1; j >= 0; --j) 
    {
        DisplayProgess(IMAGE_HEIGHT, j);
        for (int i = 0; i < IMAGE_WIDTH; ++i) 
        {
            Color pixelColor(0, 0, 0);
            for (int s = 0; s < SAMPLES_PER_PIXEL; ++s)
            {
                auto u = (i + RandomDouble()) / (IMAGE_WIDTH - 1);
                auto v = (j + RandomDouble()) / (IMAGE_HEIGHT - 1);
                Ray r = cam.getRay(u, v);
                pixelColor += RayColor(r, world, MAX_DEPTH, HEMISPHERE);
            }
            WriteColor(std::cout, pixelColor, SAMPLES_PER_PIXEL);
        }
    }
    std::cerr << "\nDone.\n";
}