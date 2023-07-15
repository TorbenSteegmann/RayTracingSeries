#include <iostream>
#include <string>

#include "rtWeekend.h"
#include "Ray.h"
#include "Vec3.h"
#include "Camera.h"
#include "Color.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Material.h"
#include "Lambertian.h"
#include "Metal.h"
#include "Dielectric.h"

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

HittableList RandomScene() {
    HittableList world;

    auto ground_material = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world.Add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++) 
    {
        for (int b = -11; b < 11; b++) 
        {
            auto chooseMat = RandomDouble();
            Point3 center(a + 0.9 * RandomDouble(), 0.2, b + 0.9 * RandomDouble());

            if ((center - Point3(4, 0.2, 0)).Length() > 0.9) 
            {
                std::shared_ptr<Material> sphereMaterial;

                if (chooseMat < 0.8) 
                {
                    // diffuse
                    auto albedo = Color::Random() * Color::Random(); //albedo = rückstrahlvermögen
                    sphereMaterial = std::make_shared<Lambertian>(albedo);
                    world.Add(std::make_shared<Sphere>(center, 0.2, sphereMaterial));
                }
                else if (chooseMat < 0.95) 
                {
                    // metal
                    auto albedo = Color::Random(0.5, 1);
                    auto fuzz = RandomDouble(0, 0.5);
                    sphereMaterial = std::make_shared<Metal>(albedo, fuzz);
                    world.Add(std::make_shared<Sphere>(center, 0.2, sphereMaterial));
                }
                else 
                {
                    // glass
                    sphereMaterial = std::make_shared<Dielectric>(1.5);
                    world.Add(std::make_shared<Sphere>(center, 0.2, sphereMaterial));
                }
            }
        }
    }

    auto material1 = std::make_shared<Dielectric>(1.5);
    world.Add(std::make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    world.Add(std::make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    world.Add(std::make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

    return world;
}


inline Color RayColor(const Ray& r, const Hittable& world, int depth) //Why is this not in Color class?
{
    if (depth <= 0) return Color(0, 0, 0);

    HitRecord rec;
    if (world.Hit(r, 0.001, infinity, rec))
    {
        Ray scattered;
        Color attenuation;
        if (rec.matPtr -> Scatter(r, rec, attenuation, scattered))
        {
            return attenuation * RayColor(scattered, world, depth - 1);
        }
        return Color(0, 0, 0);
    }
    Vec3 unitDirection = UnitVector(r.direction());
    auto t = 0.5 * (unitDirection.y() + 1.0);
    return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

int main() 
{
    //Testing
    
    //Image 

    const auto ASPECT_RATIO = 3.0 / 2.0;
    const int IMAGE_WIDTH = 1200;
    const int IMAGE_HEIGHT = static_cast<int>(IMAGE_WIDTH/ASPECT_RATIO);
    const int SAMPLES_PER_PIXEL = 500;
    const int MAX_DEPTH = 50;
    const enum DiffusionMethod{UNIT_SPHERE, HEMISPHERE};


    //World

    HittableList world = RandomScene();

    //auto materialGround = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    //auto materialCenter = std::make_shared<Lambertian>(Color(0.2, 0.3, 0.8));
    //auto materialLeft = std::make_shared<Dielectric>(1.5);
    //auto materialRight = std::make_shared<Metal>(Color(0.8, 0.6, 0.2), 1.0);
    //

    //world.Add(std::make_shared<Sphere>(Point3(0, -100.5, -1), 100, materialGround));
    //world.Add(std::make_shared<Sphere>(Point3(0, 0, -1), 0.5, materialCenter));
    //world.Add(std::make_shared<Sphere>(Point3(-1.0, 0, -1), 0.5, materialLeft));
    //world.Add(std::make_shared<Sphere>(Point3(-1.0, 0, -1), -0.45, materialLeft));
    //world.Add(std::make_shared<Sphere>(Point3(1.0, 0, -1), 0.5, materialRight));


    //Camera
    Point3 lookFrom(13, 2, 3);
    Point3 lookAt(0, 0, 0);
    Vec3 vUp(0, 1, 0);
    auto distToFocus = 10;
    double aperture = 0.1;
    double fov = 20.0;
    Camera cam(lookFrom, lookAt, vUp, fov, ASPECT_RATIO, aperture, distToFocus);


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
                Ray r = cam.GetRay(u, v);
                pixelColor += RayColor(r, world, MAX_DEPTH);
            }
            WriteColor(std::cout, pixelColor, SAMPLES_PER_PIXEL);
        }
    }
    std::cerr << "\nDone.\n";
}