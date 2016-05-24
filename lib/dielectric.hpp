#ifndef DIELECTRIC_H
#define DIELECTRIC_H

#include "hitable.hpp"
#include "material.hpp"
#include "ray.hpp"
#include "vec3.hpp"

inline double schlick(double cosine, double ref_index) {
	double r0 = (1.0 - ref_index) / (1.0 + ref_index);
	r0 *= r0;
	return r0 + (1.0 - r0) * pow(1 - cosine, 5.0);
}

class dielectric : public material {
public:
	dielectric(double ri) { ref_index = ri; }

	virtual bool scatter(const ray& r_in, const hit_record& rec, scatter_record& srec) const;

private:
	double ref_index;
};

bool dielectric::scatter(const ray& r_in, const hit_record& hrec, scatter_record& srec) const {
		vec3 outward_normal;
		vec3 refracted;
		double ior;
		double reflect_prob;
		double cosine;
		srec.attenuation[0] = srec.attenuation[1] = srec.attenuation[2] = 1.0;

		if (dot(r_in.direction(), hrec.normal) > 0.0) {
			outward_normal = -hrec.normal;
			ior = ref_index;
			cosine = dot(r_in.direction(), hrec.normal);
			cosine = sqrt(1.0 - ref_index*ref_index*(1.0 - cosine*cosine));
		}
		else {
			outward_normal = hrec.normal;
			ior = 1.0 / ref_index;
			cosine = -dot(r_in.direction(), hrec.normal);
		}

		
		refracted = refract(r_in.direction(), outward_normal, ior);
		reflect_prob = refracted.squared_length() > 0.99 ? schlick(cosine, ref_index) : 1.0;

		srec.is_specular = true;
		srec.pdf_ptr = nullptr;

		if (drand48() < reflect_prob) { // reflect
			vec3 reflected = reflect(r_in.direction(), hrec.normal);
			srec.specular_ray = ray(hrec.point, reflected);
			
		}
		else { // refract
			srec.specular_ray = ray(hrec.point, refracted);
		}
		return true;
	}

#endif
