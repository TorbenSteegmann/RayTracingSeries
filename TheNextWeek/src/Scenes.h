#pragma once

#include "AaRect.h"
#include "Box.h"
#include "Bvh.h"
#include "Camera.h"
#include "ConstantDensityVolume.h"
#include "Dielectric.h"
#include "HittableList.h"
#include "Lambertian.h"
#include "Material.h"
#include "Metal.h"
#include "Sphere.h"
#include "Textures.h"
#include "Vec3.h"

[[maybe_unused]] HittableList RandomScene()
{
    HittableList world;


    auto checkerTexture = std::make_shared<CheckerTexture>(Color(0.03, 0.27, 0.09), Color(0.9, 0.9, 0.9));
    auto groundMaterial = std::make_shared<Lambertian>(checkerTexture);
    world.Add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, groundMaterial));

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
                    // world.Add(std::make_shared<Sphere>(center, center + Vec3(0, RandomDouble(0, 0.5), 0), 0.2, sphereMaterial)); //moving spheres
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

    world = HittableList(std::make_shared<BvhNode>(world, 0, 1));
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

    world = HittableList(std::make_shared<BvhNode>(world, 0, 1));
    return world;
}


[[maybe_unused]] HittableList CheckerScene()
{
    HittableList world;

    auto checkerTexture = std::make_shared<CheckerTexture>(Color(0.03, 0.27, 0.09), Color(0.9, 0.9, 0.9));
    auto groundMaterial = std::make_shared<Lambertian>(checkerTexture);

    world.Add(std::make_shared<Sphere>(Point3(0, -10, 0), 10, groundMaterial));
    world.Add(std::make_shared<Sphere>(Point3(0, 10, 0), 10, groundMaterial));

    world = HittableList(std::make_shared<BvhNode>(world, 0, 1));
    return world;
}

[[maybe_unused]] HittableList PerlinSpheresScene()
{
    HittableList world;

    auto perlinTexture = std::make_shared<NoiseTexture>(4);

    world.Add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, std::make_shared<Lambertian>(perlinTexture)));
    world.Add(std::make_shared<Sphere>(Point3(0, 2, 0), 2, std::make_shared<Lambertian>(perlinTexture)));

    world = HittableList(std::make_shared<BvhNode>(world, 0, 1));
    return world;
}


[[maybe_unused]] HittableList EarthScene()
{
    HittableList world;
    world.BackgroundColor({0, 0, 0});
    auto earthTexture = std::make_shared<ImageTexture>("earthmap.jpg");
    auto earthSurface = std::make_shared<Lambertian>(earthTexture);
    auto globe = std::make_shared<Sphere>(Point3(0, 0, 0), 2, earthSurface);
    world.Add(globe);

    world = HittableList(std::make_shared<BvhNode>(world, 0, 1));
    return world;
}


[[maybe_unused]] HittableList SimpleLightScene()
{
    HittableList world;


    auto perlinTexture = std::make_shared<NoiseTexture>(4);
    world.Add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, std::make_shared<Lambertian>(perlinTexture)));
    world.Add(std::make_shared<Sphere>(Point3(0, 2, 0), 2, std::make_shared<Lambertian>(perlinTexture)));

    auto diffLightTexture = std::make_shared<DiffuseLight>(Color(4, 4, 4));
    auto diffLightGreenTexture = std::make_shared<DiffuseLight>(Color(0, 8, 0));
    world.Add(std::make_shared<XyRect>(3, 5, 1, 3, -2, diffLightTexture));
    world.Add(std::make_shared<Sphere>(Point3(0, 5.5, 0), 1, diffLightGreenTexture));

    world = HittableList(std::make_shared<BvhNode>(world, 0, 1));
    world.BackgroundColor(Color(0, 0, 0));
    return world;
}

[[maybe_unused]] HittableList CornellBoxScene()
{
    HittableList world;

    auto red = std::make_shared<Lambertian>(Color(0.65, 0.05, 0.05));
    auto white = std::make_shared<Lambertian>(Color(0.73, 0.73, 0.73));
    auto green = std::make_shared<Lambertian>(Color(0.12, 0.45, 0.15));
    auto light = std::make_shared<DiffuseLight>(Color(15, 15, 15));

    // Walls, Floor, Ceiling, Light
    world.Add(std::make_shared<YzRect>(0, 555, 0, 555, 555, red));
    world.Add(std::make_shared<YzRect>(0, 555, 0, 555, 0, green));
    world.Add(std::make_shared<XzRect>(213, 343, 227, 332, 554, light));
    world.Add(std::make_shared<XzRect>(0, 555, 0, 555, 0, white));
    world.Add(std::make_shared<XzRect>(0, 555, 0, 555, 555, white));
    world.Add(std::make_shared<XyRect>(0, 555, 0, 555, 555, white));

    // Boxes
    std::shared_ptr<Hittable> boxOne = std::make_shared<Box>(Point3(0, 0, 0), Point3(165, 330, 165), white);
    std::shared_ptr<Hittable> boxTwo = std::make_shared<Box>(Point3(0, 0, 0), Point3(165, 165, 165), white);

    boxOne = std::make_shared<RotateY>(boxOne, 15);
    boxOne = std::make_shared<Translate>(boxOne, Vec3(265, 0, 295));

    boxTwo = std::make_shared<RotateY>(boxTwo, -18);
    boxTwo = std::make_shared<Translate>(boxTwo, Vec3(130, 0, 65));

    world.Add(boxOne);
    world.Add(boxTwo);

    world = HittableList(std::make_shared<BvhNode>(world, 0, 0));
    world.BackgroundColor(Color(0, 0, 0));
    return world;
}

[[maybe_unused]] HittableList CornellBoxSmokeScene()
{
    HittableList world;

    auto red = std::make_shared<Lambertian>(Color(0.65, 0.05, 0.05));
    auto white = std::make_shared<Lambertian>(Color(0.73, 0.73, 0.73));
    auto green = std::make_shared<Lambertian>(Color(0.12, 0.45, 0.15));
    auto light = std::make_shared<DiffuseLight>(Color(15, 15, 15));

    // Walls, Floor, Ceiling, Light
    world.Add(std::make_shared<YzRect>(0, 555, 0, 555, 555, red));
    world.Add(std::make_shared<YzRect>(0, 555, 0, 555, 0, green));
    world.Add(std::make_shared<XzRect>(213, 343, 227, 332, 554, light));
    world.Add(std::make_shared<XzRect>(0, 555, 0, 555, 0, white));
    world.Add(std::make_shared<XzRect>(0, 555, 0, 555, 555, white));
    world.Add(std::make_shared<XyRect>(0, 555, 0, 555, 555, white));

    // Boxes
    std::shared_ptr<Hittable> boxOne = std::make_shared<Box>(Point3(0, 0, 0), Point3(165, 330, 165), white);
    std::shared_ptr<Hittable> boxTwo = std::make_shared<Box>(Point3(0, 0, 0), Point3(165, 165, 165), white);

    boxOne = std::make_shared<RotateY>(boxOne, 15);
    boxOne = std::make_shared<Translate>(boxOne, Vec3(265, 0, 295));

    boxTwo = std::make_shared<RotateY>(boxTwo, -18);
    boxTwo = std::make_shared<Translate>(boxTwo, Vec3(130, 0, 65));

    world.Add(std::make_shared<ConstantDensityVolume>(boxOne, 0.01, Color(0, 0, 0)));
    world.Add(std::make_shared<ConstantDensityVolume>(boxTwo, 0.01, Color(1, 1, 1)));

    world = HittableList(std::make_shared<BvhNode>(world, 0, 0));
    world.BackgroundColor(Color(0, 0, 0));
    return world;
}

[[maybe_unused]] HittableList NextWeekFinalScene(Camera& cam)
{
    HittableList boxesOne;
    auto ground = std::make_shared<Lambertian>(Color(0.48, 0.83, 0.53));

    int const BOXES_PER_SIDE = 20;
    for (int i = 0; i < BOXES_PER_SIDE; ++i)
    {
        for (int j = 0; j < BOXES_PER_SIDE; ++j)
        {
            auto w = 100.0;
            auto x0 = -1000.0 + i * w;
            auto z0 = -1000.0 + j * w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto z1 = z0 + w;
            auto y1 = RandomDouble(1, 101);

            boxesOne.Add(std::make_shared<Box>(Point3(x0, y0, z0), Point3(x1, y1, z1), ground));
        }
    }

    HittableList world;

    world.Add(std::make_shared<BvhNode>(boxesOne, 0, 1));

    auto light = std::make_shared<DiffuseLight>(Color(7, 7, 7));
    world.Add(std::make_shared<XzRect>(123, 423, 147, 412, 554, light));

    auto centerOne = Point3(400, 400, 200);
    auto centerTwo = centerOne + Vec3(30, 0, 0);
    auto movingSphereMat = std::make_shared<Lambertian>(Color(0.7, 0.3, 0.1));
    world.Add(std::make_shared<Sphere>(centerOne, centerTwo, 50, movingSphereMat));

    world.Add(std::make_shared<Sphere>(Point3(260, 150, 45), 50, std::make_shared<Dielectric>(1.5)));
    world.Add(std::make_shared<Sphere>(Point3(0, 150, 145), 50, std::make_shared<Metal>(Color(0.8, 0.8, 0.9), 1.0)));

    auto boundary = std::make_shared<Sphere>(Point3(360, 150, 145), 70, std::make_shared<Dielectric>(1.5));
    world.Add(boundary);
    world.Add(std::make_shared<ConstantDensityVolume>(boundary, 0.2, Color(0.2, 0.4, 0.9)));
    boundary = std::make_shared<Sphere>(Point3(0, 0, 0), 5000, std::make_shared<Dielectric>(1.5));
    world.Add(std::make_shared<ConstantDensityVolume>(boundary, 0.0001, Color(1, 1, 1)));

    auto emat = std::make_shared<Lambertian>(std::make_shared<ImageTexture>("earthmap.jpg"));
    world.Add(std::make_shared<Sphere>(Point3(400, 200, 400), 100, emat));
    auto perlinTexture = std::make_shared<NoiseTexture>(0.1);
    world.Add(std::make_shared<Sphere>(Point3(220, 280, 300), 80, std::make_shared<Lambertian>(perlinTexture)));


    HittableList boxesTwo;
    auto white = std::make_shared<Lambertian>(Color(0.73, 0.73, 0.73));
    int ns = 1000;
    for (int j = 0; j < ns; ++j)
    {
        boxesTwo.Add(std::make_shared<Sphere>(Point3::Random(0, 165), 10, white));
    }

    world.Add(std::make_shared<Translate>(std::make_shared<RotateY>(std::make_shared<BvhNode>(boxesTwo, 0.0, 1.0), 15), Vec3(-100, 270, 395)));

    world = HittableList(std::make_shared<BvhNode>(world, 0, 1));
    world.BackgroundColor(Color(0, 0, 0));

    // Camera
    Point3 lookFrom(478, 278, -600);
    Point3 lookAt(278, 278, 0);
    Vec3 vUp(0, 1, 0);
    auto focusDist = 40.0;
    auto aperture = 0.0;
    auto fov = 40;

    cam = Camera(lookFrom, lookAt, vUp, fov, 1.0, aperture, focusDist);

    return world;
}
