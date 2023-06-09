#pragma once
#include "material.h"// Index of Refraction
#include "vec3.h"
#include "ray.h"
#include "hittable.h"
class dielectric : public material {
public:
	dielectric(double index_of_refraction) : ior(index_of_refraction) {}
	virtual bool scatter(
		const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
	) const override {
		attenuation = color(1.0, 1.0, 1.0);
		double refraction_ratio = rec.front_face ? (1.0 / ior) : ior;

		vec3 unit_direction = unit_vector(r_in.direction());
		double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
		double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

		bool cannot_refract = refraction_ratio * sin_theta > 1.0;
		vec3 direction;
		if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double()){
			direction = reflect(unit_direction, rec.normal);
		}
		else {
			direction = refract(unit_direction, rec.normal, refraction_ratio);
		}

		scattered = ray(rec.p, direction);
		return true;
	}
	/*Schlick 近似估算
		现实世界中的玻璃, 发生折射的概率会随着入射角而改变——
		从一个很狭窄的角度去看玻璃窗, 它会变成一面镜子。
		这个式子又丑又长, 好在我们有个数学上近似的等式, 
		它是由Christophe Schlick提出的:*/
	static double reflectance(double cosine, double ref_idx) {
		auto r0 = (1 - ref_idx) / (1 + ref_idx);
		r0 = r0 * r0;
		//return false;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}

public:
	double ior; // 折射率
};
