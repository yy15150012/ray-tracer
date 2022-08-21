#pragma once
#include "material.h"
#include "vec3.h"
#include "ray.h"
#include "hittable.h"

class metal : public material {
public:
    metal(const color& a, double f) : albedo(a) , fuzz(f) {}

    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const override {
        // 向量反射
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + fuzz * vec3::random_in_unit_sphere(), r_in.time());
        attenuation = albedo;

        // 只要反射的方向和法线是同一个半球，则表示反射成功
        return (dot(scattered.direction(), rec.normal) > 0);
    }

public:
    //反射率
    color albedo;
    //模糊反射系数
    double fuzz;
};