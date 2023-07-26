#pragma once

#include "Dielectric.h"
#include "HittableList.h"
#include "Lambertian.h"
#include "Material.h"
#include "Metal.h"
#include "Sphere.h"
#include "Vec3.h"

[[maybe_unused]] HittableList RandomScene()
{
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
                    auto albedo = Color::Random() * Color::Random(); // albedo = rueckstrahlvermoegen
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

[[maybe_unused]] HittableList BasicScene()
{
    HittableList world;
    auto materialGround = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    auto materialCenter = std::make_shared<Lambertian>(Color(0.2, 0.3, 0.8));
    auto materialLeft = std::make_shared<Dielectric>(1.5);
    auto materialRight = std::make_shared<Metal>(Color(0.8, 0.6, 0.2), 1.0);


    world.Add(std::make_shared<Sphere>(Point3(0, -100.5, -1), 100, materialGround));
    world.Add(std::make_shared<Sphere>(Point3(0, 0, -1), 0.5, materialCenter));
    world.Add(std::make_shared<Sphere>(Point3(-1.0, 0, -1), 0.5, materialLeft));
    world.Add(std::make_shared<Sphere>(Point3(-1.0, 0, -1), -0.45, materialLeft));
    world.Add(std::make_shared<Sphere>(Point3(1.0, 0, -1), 0.5, materialRight));

    return world;
}
