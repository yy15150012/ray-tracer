#pragma once
#include "material.h"
#include "vec3.h"
#include "texture.h"


class lambertian : public material {
public:
    lambertian(const color& a) : albedo(make_shared<solid_color>(a)) {}
    lambertian(shared_ptr<texture> a) : albedo(a) {}

    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const override {
        auto scatter_direction = rec.normal + vec3::random_unit_vector();

        // Catch degenerate scatter direction
        if (scatter_direction.near_zero())
            scatter_direction = rec.normal;

        scattered = ray(rec.p, scatter_direction, r_in.time());
        //attenuation = albedo;
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    }

public:
	// 散射衰减值 反射率 漫反射系数;
    //color albedo;
    shared_ptr<texture> albedo;
};