#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H
#endif


#include "rtweekend.h"
#include "texture.h"
#include "vec3.h"

struct hit_record;

class material {
public:
    virtual color emitted(double u, double v, const point3 &p) const {
        return color(0, 0, 0);
    }

    virtual bool scatter(
            const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered
    ) const = 0;
};


class isotropic : public material {
public:
    isotropic(color c) : albedo(make_shared<solid_color>(c)) {}

    isotropic(shared_ptr<texture> a) : albedo(a) {}

    virtual bool scatter(
            const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered
    ) const override {
        scattered = ray(rec.p, vec3::random_in_unit_sphere(), r_in.time());
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    }

public:
    shared_ptr<texture> albedo;
};