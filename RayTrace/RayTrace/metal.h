#pragma once
#include "material.h"
#include "vec3.h"
#include "ray.h"
#include "hittable.h"

class metal : public material {
public:
    metal(const color& a, double f) : albedo(a) , fuzz(f) {}

    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const override {
        // ��������
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + fuzz * vec3::random_in_unit_sphere(), r_in.time());
        attenuation = albedo;

        // ֻҪ����ķ���ͷ�����ͬһ���������ʾ����ɹ�
        return (dot(scattered.direction(), rec.normal) > 0);
    }

public:
    //������
    color albedo;
    //ģ������ϵ��
    double fuzz;
};