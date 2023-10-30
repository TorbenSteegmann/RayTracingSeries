cmake_minimum_required(VERSION 3.23)
project(TheNextWeek)

set(CMAKE_CXX_STANDARD 20)

include_directories(TheNextWeek)

add_executable(TheNextWeek

        src/TheNextWeek.cpp

        src/AxisAlignedBoundingBox.h

        src/AaRect.h

        src/Bvh.h

        src/Box.h

        src/Camera.h

        src/Color.h

        src/ConstantDensityVolume.h

        src/Dielectric.h

        src/Headers.h

        src/Hittable.h

        src/HittableList.h

        src/Lambertian.h

        src/Material.h

        src/Metal.h

        src/Perlin.h

        src/Ray.h

        src/RTWeekend.h

        src/RtwStbImage.h

        src/Scenes.h

        src/Sphere.h

        src/Textures.h

        src/Vec3.h

        )
