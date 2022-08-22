cmake_minimum_required(VERSION 3.17)
project(ray_tracer)

set(CMAKE_CXX_STANDARD 14)

include_directories(RayTrace/RayTrace)

add_executable(ray_tracer
        RayTrace/RayTrace/aabb.h
        RayTrace/RayTrace/bvh.h
        RayTrace/RayTrace/camera.h
        RayTrace/RayTrace/color.h
        RayTrace/RayTrace/dielectric.h
        RayTrace/RayTrace/hittable.h
        RayTrace/RayTrace/hittable_list.h
        RayTrace/RayTrace/lambertian.h
        RayTrace/RayTrace/material.h
        RayTrace/RayTrace/metal.h
        RayTrace/RayTrace/moving_sphere.h
        RayTrace/RayTrace/ray.h
        RayTrace/RayTrace/RayTrace.cpp
        RayTrace/RayTrace/rtweekend.h
        RayTrace/RayTrace/sphere.h
        RayTrace/RayTrace/texture.h
        RayTrace/RayTrace/vec3.h RayTrace/RayTrace/Timer.h RayTrace/RayTrace/perlin.h)
