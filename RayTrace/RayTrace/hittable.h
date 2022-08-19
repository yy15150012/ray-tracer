#pragma once

#include "ray.h"
#include "rtweekend.h"
class material;


// ��ײ���¼����
struct hit_record
{
    vec3 p;  // ��ײ��λ��
    vec3 normal; // ��ײ����淨��
    double t;   // ��ײ�������ϵ�ʱ��t����

    shared_ptr<material> mat_ptr;

    bool front_face;
    
    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
public:
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};
