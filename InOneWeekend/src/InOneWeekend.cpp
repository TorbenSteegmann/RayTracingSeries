// This is an implementation of the techniques presented in the first book of the "Ray Tracing in one Weekend" - Series
// which is available on GitHub under the following link: https://github.com/RayTracing/raytracing.github.io/ .
// Although I plan a lot of changes, to be able to follow the following books closely I have decided to postpone most
// of them until after the completion of "Ray Tracing the Rest of your Life". For now, I have deviated only in the
// implementation of Multi-Threading to cut down on the long render times. Future plans include the implementation of:
// 1. Denoising with Intel Open Image Denoise
// 2. Live Rendering and a proper UI 

#include <chrono>
#include <future>
#include <iostream>
#include <string>

#include "Camera.h"
#include "Color.h"
#include "HittableList.h"
#include "Material.h"
#include "Ray.h"
#include "Scenes.h"
#include "Vec3.h"
#include "rtWeekend.h"


void DisplayProgess(uint32_t PACKAGE_TOTAL, uint32_t package)
{
    uint32_t remainingPackages = PACKAGE_TOTAL - package;
    std::string progressString = "Packages remaining: " + std::to_string(remainingPackages);
    std::string progressBar = "|";
    auto progressInPercent = (uint8_t)((float(package) / float(PACKAGE_TOTAL)) * 100);
    for (uint8_t i = 2; i <= progressInPercent; i += 2)
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


Color RayColor(Ray const& r, Hittable const& world, int depth) // Why is this not in Color class?
{
    if (depth <= 0)
        return {0, 0, 0};

    HitRecord rec;
    if (world.Hit(r, 0.001, infinity, rec))
    {
        Ray scattered;
        Color attenuation;
        if (rec.matPtr->Scatter(r, rec, attenuation, scattered))
        {
            return attenuation * RayColor(scattered, world, depth - 1);
        }
        return {0, 0, 0};
    }
    Vec3 unitDirection = UnitVector(r.direction());
    auto t = 0.5 * (unitDirection.y() + 1.0);
    return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

Camera CameraSetup(double aspectRatio)
{
    Point3 lookFrom(13, 2, 3);
    Point3 lookAt(0, 0, 0);
    Vec3 vUp(0, 1, 0);
    auto focusDist = 10.0;
    auto aperture = 0.1;
    auto fov = 20;

    Camera cam(lookFrom, lookAt, vUp, fov, aspectRatio, aperture, focusDist);

    return cam;
}

void Render(uint32_t packageNumber,
            uint32_t packageWidth,
            uint32_t packageHeight,
            uint32_t imageHeight,
            uint32_t imageWidth,
            uint32_t samplesPerPixel,
            uint16_t maxDepth,
            Camera cam,
            HittableList const& world,
            std::vector<uint8_t>& colors)
{
    auto startHeight = (imageHeight - 1) - (((packageNumber * packageWidth) / imageWidth) * packageHeight);
    auto startWidth = (packageNumber * packageWidth) % imageWidth;
    auto endHeight = int(startHeight) - int(packageHeight);

    for (auto j = int(startHeight); j > endHeight; --j)
    {
        for (auto i = startWidth; i < (startWidth + packageWidth); ++i)
        {
            Color pixelColor(0, 0, 0);
            for (int s = 0; s < samplesPerPixel; ++s)
            {
                auto u = (i + RandomDouble()) / (imageWidth - 1);
                auto v = (j + RandomDouble()) / (imageHeight - 1);
                Ray r = cam.GetRay(u, v);
                pixelColor += RayColor(r, world, maxDepth);
            }

            auto pixelNumber = ((((imageHeight - 1 - j)) * imageWidth) + i);
            WriteColor(colors, pixelColor, samplesPerPixel, pixelNumber);
        }
    }
}

int main()
{
    // Testing
    using namespace std::chrono_literals;
    auto startTime = std::chrono::high_resolution_clock::now();

    // Image

    constexpr auto ASPECT_RATIO = 3.0f / 2.0f;
    int const IMAGE_WIDTH = 1200;
    constexpr auto IMAGE_HEIGHT = static_cast<int>(IMAGE_WIDTH / ASPECT_RATIO);
    constexpr auto TOTAL_PIXELS = IMAGE_HEIGHT * IMAGE_WIDTH;

    int const SAMPLES_PER_PIXEL = 64;
    int const MAX_DEPTH = 12;

    int const PACKAGE_WIDTH = IMAGE_WIDTH * 0.01;
    int const PACKAGE_HEIGHT = IMAGE_HEIGHT * 0.01;

    const uint32_t PACKAGE_TOTAL = 100 * 100;


    // World

    auto world = RandomScene();


    // Camera

    auto cam = CameraSetup(ASPECT_RATIO);


    // Render

    std::cout << "P3\n" << IMAGE_WIDTH << ' ' << IMAGE_HEIGHT << "\n255\n";

    std::vector<uint8_t> colors(TOTAL_PIXELS * 3);
    int const MAX_THREADS = (int)std::thread::hardware_concurrency();
    std::vector<std::future<void>> workers{};
    for (uint32_t i = 0; i < PACKAGE_TOTAL; ++i)
    {
        DisplayProgess(PACKAGE_TOTAL, i + 1);

        if (workers.size() < MAX_THREADS)
        {
            workers.emplace_back(std::async(std::launch::async, Render, i, PACKAGE_WIDTH, PACKAGE_HEIGHT, IMAGE_HEIGHT, IMAGE_WIDTH,
                                            SAMPLES_PER_PIXEL, MAX_DEPTH, cam, world, std::ref(colors)));
            continue;
        }

        int j = 0;
        while (true)
        {
            if (workers[j].wait_for(0ms) == std::future_status::ready)
            {
                workers[j] = std::async(std::launch::async, Render, i, PACKAGE_WIDTH, PACKAGE_HEIGHT, IMAGE_HEIGHT, IMAGE_WIDTH, SAMPLES_PER_PIXEL,
                                        MAX_DEPTH, cam, world, std::ref(colors));
                break;
            }
            j = (j + 1) % MAX_THREADS;
        }
    }

    // wait for remaining threads to finish
    int j = 0;
    while (j < MAX_THREADS)
    {
        if (!(workers[j].wait_for(0ms) == std::future_status::ready))
        {
            continue;
        }
        ++j;
    }

    for (uint32_t i = 0; i < colors.size(); i = i + 3)
    {
        std::cout << (int)colors[i] << ' ' << (int)colors[i + 1] << ' ' << (int)colors[i + 2] << '\n';
    }

    auto endTime = std::chrono::high_resolution_clock::now();

    std::cerr << "\nDone (" << std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count() << "s).\n";
}
