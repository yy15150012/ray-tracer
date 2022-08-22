#pragma once

#include "rtweekend.h"
#include "vec3.h"
#include "perlin.h"
// 纹理基类

class texture {
public:
    virtual color value(double u, double v, const point3 &p) const = 0;
};
//常量颜色纹理，纹理的颜色就是固定的某种颜色

class solid_color : public texture {
public:
    color color_value;
public:
    solid_color() {}

    solid_color(color c) : color_value(c) {}

    solid_color(double r, double g, double b) : solid_color(color(r, g, b)) {}

    virtual color value(double u, double v, const vec3 &p) const override {
        return color_value;
    }
};


class checker_texture : public texture {
public:
    checker_texture() {}

    checker_texture(shared_ptr<texture> _even, shared_ptr<texture> _odd) : even(_even), odd(_odd) {}

    checker_texture(color c1, color c2) : even(make_shared<solid_color>(c1)), odd(make_shared<solid_color>(c2)){}

    virtual color value(double u, double v, const point3& p) const override{
        auto sines = std::sin(5 * p.x()) * std::sin(5 * p.y()) * std::sin(5 * p.z());
        if (sines < 0) {
            return odd->value(u, v, p);
        } else {
            return even->value(u, v, p);
        }
    }

public:
    shared_ptr<texture> even, odd;
};


class noise_texture : public texture {
public:
    noise_texture(){}
    virtual color value(double u, double v, const point3 &p) const override {
        return color(1.0, 1.0, 1.0) * noise.noise(p);
    }

public:
    perlin noise;
};