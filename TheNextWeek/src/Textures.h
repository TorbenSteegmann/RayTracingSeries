#pragma once

#include <cmath>
#include <iostream>

#include "Perlin.h"
#include "RtwStbImage.h"
#include "Vec3.h"

class Texture
{
public:
    virtual Color Value(double u, double v, Point3 const& p) const = 0;
};

class SolidColor : public Texture
{
public:
    SolidColor() {}
    SolidColor(Color c) : rgb(c) {}
    SolidColor(double red, double green, double blue) : rgb(red, green, blue) {}

    virtual Color Value(double u, double v, Point3 const& p) const override { return rgb; }

private:
    Color rgb;
};

class CheckerTexture : public Texture
{
public:
    CheckerTexture() {}
    CheckerTexture(std::shared_ptr<Texture> e, std::shared_ptr<Texture> o) : even(e), odd(o) {}
    CheckerTexture(Color cOne, Color cTwo) : even(std::make_shared<SolidColor>(cOne)), odd(std::make_shared<SolidColor>(cTwo)) {}

    virtual Color Value(double u, double v, Point3 const& p) const override
    {
        auto sines = std::sin(10 * p.x()) * std::sin(10 * p.y()) * std::sin(10 * p.z());
        if (sines < 0)
            return odd->Value(u, v, p);
        else
            return even->Value(u, v, p);
    }

private:
    std::shared_ptr<Texture> odd;
    std::shared_ptr<Texture> even;
};

class NoiseTexture : public Texture
{
public:
    NoiseTexture(double sc = 1) : scale(sc) {}

    virtual Color Value(double u, double v, Point3 const& p) const override
    {
        return Color(1, 1, 1) * 0.5 * (1 + std::sin(scale * p.z() + 10 * noise.Turbulence(scale * p)));
    }

private:
    Perlin noise;
    double scale;
};


class ImageTexture : public Texture
{
public:
    const static int BYTES_PER_PIXEL = 3;

    ImageTexture() : data(nullptr), width(0), height(0), bytesPerScanline(0) {}

    ImageTexture(char const* filename)
    {
        auto componentsPerPixel = BYTES_PER_PIXEL;
        data = stbi_load(filename, &width, &height, &componentsPerPixel, componentsPerPixel);

        if (!data)
        {
            std::cerr << "ERROR: Could not load texture image file '" << filename << "'\n";
            width = height = 0;
        }

        bytesPerScanline = BYTES_PER_PIXEL * width;
    }

    ~ImageTexture() { delete data; }

    virtual Color Value(double u, double v, Vec3 const& p) const override
    {
        if (data == nullptr)
            return Color(0, 1, 1); // Cyan as debugging aid

        u = Clamp(u, 0.0, 1.0);
        v = 1.0 - Clamp(v, 0.0, 1.0);

        auto i = static_cast<int>(u * width);
        auto j = static_cast<int>(v * height);

        if (i >= width)
            i = width - 1;
        if (j >= height)
            j = height - 1;

        auto const COLOR_SCALE = 1.0 / 255.0;
        auto pixel = data + i * BYTES_PER_PIXEL + j * bytesPerScanline;

        return {COLOR_SCALE * pixel[0], COLOR_SCALE * pixel[1], COLOR_SCALE * pixel[2]};
    }

private:
    unsigned char* data;
    int width;
    int height;
    int bytesPerScanline;
};
