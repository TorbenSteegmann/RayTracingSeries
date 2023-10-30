#pragma once

#include "Vec3.h"
#include "rtWeekend.h"

class Perlin
{
public:
    Perlin()
    {
        randomVec = new Vec3[POINT_COUNT];

        for (int i = 0; i < POINT_COUNT; ++i)
        {
            randomVec[i] = UnitVector(Vec3::Random(-1, 1));
        }

        permX = PerlinGeneratePerm();
        permY = PerlinGeneratePerm();
        permZ = PerlinGeneratePerm();
    }

    ~Perlin()
    {
        delete[] randomVec;
        delete[] permX;
        delete[] permY;
        delete[] permZ;
    }

    double Noise(Point3 const& p) const
    {
        auto u = p.x() - std::floor(p.x());
        auto v = p.y() - std::floor(p.y());
        auto w = p.z() - std::floor(p.z());

        auto i = static_cast<int>(std::floor(p.x()));
        auto j = static_cast<int>(std::floor(p.y()));
        auto k = static_cast<int>(std::floor(p.z()));

        Vec3 c[2][2][2];

        for (int di = 0; di < 2; ++di)
        {
            for (int dj = 0; dj < 2; ++dj)
            {
                for (int dk = 0; dk < 2; ++dk)
                {
                    c[di][dj][dk] = randomVec[permX[(i + di) & 255] ^ permY[(j + dj) & 255] ^ permZ[(k + dk) & 255]]; // ^ = XOR
                }
            }
        }

        return PerlinInterpolation(c, u, v, w);
    }

    double Turbulence(Point3 const& p, int depth = 7) const
    {
        auto accum = 0.0;
        auto tempP = p;
        auto weight = 1.0;

        for (int i = 0; i < depth; ++i)
        {
            accum += weight * Noise(tempP);
            weight *= 0.5;
            tempP *= 2;
        }

        return std::fabs(accum);
    }

private:
    static int const POINT_COUNT = 256;
    Vec3* randomVec;
    int* permX;
    int* permY;
    int* permZ;

    static int* PerlinGeneratePerm()
    {
        auto p = new int[POINT_COUNT];
        for (int i = 0; i < POINT_COUNT; ++i)
        {
            p[i] = i;
        }

        Permute(p, POINT_COUNT);

        return p;
    }

    static void Permute(int* p, int n)
    {
        for (int i = n - 1; i > 0; --i)
        {
            int target = RandomInt(0, i);
            std::swap(p[i], p[target]);
        }
    }

    static double PerlinInterpolation(Vec3 c[2][2][2], double u, double v, double w)
    {
        auto uu = u * u * (3 - 2 * u);
        auto vv = v * v * (3 - 2 * v);
        auto ww = w * w * (3 - 2 * w);

        auto accum = 0.0f;
        for (int i = 0; i < 2; ++i)
        {
            for (int j = 0; j < 2; ++j)
            {
                for (int k = 0; k < 2; ++k)
                {
                    Vec3 weightV(u - i, v - j, w - k);
                    accum += (i * uu + (1 - i) * (1 - uu)) * (j * vv + (1 - j) * (1 - vv)) * (k * ww + (1 - k) * (1 - ww)) * DotProduct(c[i][j][k], weightV);
                }
            }
        }
        return accum;
    }
};
