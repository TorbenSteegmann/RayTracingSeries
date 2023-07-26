cmake_minimum_required(VERSION 3.23)
project(inOneWeekend)

set(CMAKE_CXX_STANDARD 20)

include_directories(inOneWeekend)

add_executable(inOneWeekend

        src/InOneWeekend.cpp

        src/Camera.h

        src/Color.h

        src/Dielectric.h

        src/Headers.h

        src/Hittable.h

        src/HittableList.h

        src/Lambertian.h

        src/Material.h

        src/Metal.h

        src/Ray.h

        src/RTWeekend.h

        src/Scenes.h

        src/Sphere.h

        src/Vec3.h)
