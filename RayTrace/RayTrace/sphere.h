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
        // ��������޽���ֱ�ӷ���false
        if (discriminant < 0) return false;
        auto sqrtd = sqrt(discriminant);

        // �����ڿɽ��ܵ�t��Χ�ڣ��������ײ��
        auto root = (-half_b - sqrtd) / a;
        if (root < t_min || t_max < root) {
            root = (-half_b + sqrtd) / a;
            if (root < t_min || t_max < root)
                return false;
        }

        // ��ײʱ��
        rec.t = root;
        // ��ײ��
        rec.p = r.at(rec.t);
        // ��ײ����

        vec3 outward_normal = (rec.p - center) / radius;

        rec.set_face_normal(r, outward_normal);
        rec.mat_ptr = mat_ptr;

        return true;
	}

    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;


    sphere(vec3 cen, double r) : center(cen), radius(r){};

public:
	vec3 center;
    double radius;
    shared_ptr<material> mat_ptr;
};


bool sphere::bounding_box(double time0, double time1, aabb& output_box) const {
    output_box = aabb(center - vec3(radius, radius, radius), center + vec3(radius, radius, radius));
    return true;
}