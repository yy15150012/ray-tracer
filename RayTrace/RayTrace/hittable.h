#pragma once

#include "ray.h"
#include "rtweekend.h"
class material;


// 碰撞点记录数据
struct hit_record
{
    vec3 p;  // 碰撞点位置
    vec3 normal; // 碰撞点表面法线
    double t;   // 碰撞在射线上的时间t参数

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
