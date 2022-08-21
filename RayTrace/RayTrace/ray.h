#pragma once
#include "vec3.h"

class ray {
public:
	ray() {};
	ray(const point3& origin, const vec3& direction, double time = 0.0) : orig(origin), dir(direction) ,tm(time) {}

	point3 origin() const { return orig; }
	vec3 direction() const { return dir; }
	double time() const { return tm; }

	point3 at(double t) const {
		return orig + t * dir;
	}

public:
	point3 orig;
	vec3 dir;
	double tm;
};

//vec3 ray_color(const ray& r) {
//	vec3 unit_direction = unit_vector(r.direction());
//	auto t = 0.5 * (unit_direction.y() + 1.0);
//	return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
//}
