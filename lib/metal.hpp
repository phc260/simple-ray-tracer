#ifndef METAL_H
#define METAL_H

#include "hitable.hpp"
#include "material.hpp"
#include "ray.hpp"
#include "vec3.hpp"

class metal : public material {
public:
	metal() { albedo = vec3(1.0); }
	metal(vec3& a) { albedo = a; fuzz = 0.0; }
	metal(vec3& a, double f) { albedo = a; fuzz = f; }
	virtual bool scatter(const ray& r_in, const hit_record& hrec, scatter_record& srec) const;

private:
	vec3 albedo;
	double fuzz;
};

bool metal::scatter(const ray& r_in, const hit_record& hrec, scatter_record& srec) const {
	vec3 reflected = reflect(r_in.direction(), hrec.normal);
	srec.is_specular = true;
	srec.specular_ray = ray(hrec.point, reflected + fuzz*random_unit_vec3());
	srec.attenuation = albedo;
	srec.pdf_ptr = nullptr;
	return true;
}
#endif
