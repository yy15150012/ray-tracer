#pragma once
#include "vec3.h"

class ray {
public:
	ray() {};
	ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}

	point3 origin() const { return orig; }
	vec3 direction() const { return dir; }

	point3 at(double t) const {
		return orig + t * dir;
	}

private:
	point3 orig;
	vec3 dir;
};

//vec3 ray_color(const ray& r) {
//	vec3 unit_direction = unit_vector(r.direction());
//	auto t = 0.5 * (unit_direction.y() + 1.0);
//	return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
//}
