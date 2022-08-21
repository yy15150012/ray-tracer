#pragma once

#include "rtweekend.h"
#include "hittable.h"

class moving_sphere : public hittable {
public:
	moving_sphere(){}
	moving_sphere(
		point3 cen0,
		point3 cen1,
		double _t0,
		double _t1,
		double r,
		shared_ptr<material> m)
		:
		center0(cen0),
		center1(cen1),
		time0(_t0),
		time1(_t1),
		radius(r),
		mat_ptr(m) {}

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override {
		vec3 oc = r.origin() - center(r.time());
		auto a = r.direction().length_squared();
		auto half_b = dot(oc, r.direction());
		auto c = oc.length_squared() - radius * radius;

		auto discriminant = half_b * half_b - a * c;
		// 如果函数无解则直接返回false
		if (discriminant < 0) return false;
		auto sqrtd = sqrt(discriminant);

		// 查找在可接受的t范围内，最近的碰撞点
		auto root = (-half_b - sqrtd) / a;
		if (root < t_min || t_max < root) {
			root = (-half_b + sqrtd) / a;
			if (root < t_min || t_max < root)
				return false;
		}

		// 碰撞时间
		rec.t = root;
		// 碰撞点
		rec.p = r.at(rec.t);
		// 碰撞法线

		vec3 outward_normal = (rec.p - center(r.time())) / radius;

		rec.set_face_normal(r, outward_normal);
		rec.mat_ptr = mat_ptr;

		return true;
	};

	point3 center(double time) const {
		return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
	}

	virtual bool bounding_box(
		double _time0, double _time1, aabb& output_box) const override;



public:
	point3 center0, center1;
	double time0, time1;
	double radius;
	shared_ptr<material> mat_ptr;

};

bool moving_sphere::bounding_box(
	double _time0, double _time1, aabb& output_box) const {
	aabb box0(center(_time0) - vec3(radius, radius, radius), center(_time0) + vec3(radius, radius, radius));
	aabb box1(center(_time1) - vec3(radius, radius, radius), center(_time1) + vec3(radius, radius, radius));
	output_box = surrounding_box(box0, box1);
	return true;
};