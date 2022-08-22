#pragma once
#include "rtweekend.h"
#include "vec3.h"
// 纹理基类

class texture {
public:
	virtual color value(double u, double v, const point3& p) const = 0;
};
//常量颜色纹理，纹理的颜色就是固定的某种颜色

class solid_color : public texture {
public:
	color color_value;
public:
	solid_color(){}
	solid_color(color c) : color_value(c) {}
	solid_color(double r, double g, double b) : solid_color(color(r, g, b)) {}

	virtual color value(double u, double v, const vec3& p) const override
	{
		return color_value;
	}
};