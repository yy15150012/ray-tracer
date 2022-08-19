#pragma once

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
public:
	//sphere(): center(), {};
    sphere(point3 cen, double r, shared_ptr<material> m)
        : center(cen), radius(r), mat_ptr(m) {};


	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
        vec3 oc = r.origin() - center;
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

        vec3 outward_normal = (rec.p - center) / radius;

        rec.set_face_normal(r, outward_normal);
        rec.mat_ptr = mat_ptr;

        return true;
	};


    sphere(vec3 cen, double r) : center(cen), radius(r){};

public:
	vec3 center;
    double radius;
    shared_ptr<material> mat_ptr;
};