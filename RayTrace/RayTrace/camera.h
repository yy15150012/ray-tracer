#pragma once
#include "ray.h"


class camera {

public:
	camera(point3 lookfrom,
		point3 lookat,
		vec3 vup,
		double vfov,
		double aspect_ratio,
		double aperture,
		double focus_dist,
		double _time0 = 0.0,
		double _time1 = 0.0
	) {
		auto theta = degrees_to_radians(vfov);
		auto h = std::tan(theta / 2.0);
		//h是一半，而2 * h是全部
		auto viewport_height = 2 * h;
		auto viewport_width = aspect_ratio * viewport_height;


		//new z
		w = unit_vector(lookfrom - lookat);
		//new x
		u = unit_vector(cross(vup, w));
		//new y
		v = cross(w, u);



		origin = lookfrom;
		horizontal = focus_dist * viewport_width * u;
		vertical = focus_dist * viewport_height * v;
		lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist* w;

		lens_radius = aperture / 2.0;
		time0 = _time0;
		time1 = _time1;
	}

	ray get_ray(double s, double t) const {
		//return ray(origin, lower_left_corner + s * horizontal + t * vertical - origin);
		vec3 rd = lens_radius * random_in_unit_disk();
		vec3 offset = u * rd.x() + v * rd.y();

		return ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset, random_double(time0, time1));
		//快门打开关闭的随机时间[0,1)内，从相机发射一条光线
	}

	static vec3 random_in_unit_disk() {
		while (true) {
			auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
			if (p.length_squared() >= 1) continue;
			return p;
		}
	}

public:
	point3 origin;
	point3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
	vec3 u, v, w;
	double lens_radius;
	double time0, time1;
};
