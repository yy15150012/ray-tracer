#pragma once
#include "rtweekend.h"
#include "vec3.h"
// �������

class texture {
public:
	virtual color value(double u, double v, const point3& p) const = 0;
};
//������ɫ�����������ɫ���ǹ̶���ĳ����ɫ

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