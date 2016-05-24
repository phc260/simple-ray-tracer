#ifndef MATERIAL_H
#define MATERIAL_H

#include "hitable.hpp"
#include "pdf.hpp"
#include "ray.hpp"
#include "vec3.hpp"

struct scatter_record {
	ray specular_ray;
	bool is_specular;
	vec3 attenuation;
	pdf *pdf_ptr;
};

class material {
public:
	virtual bool scatter(const ray& r_in, const hit_record& hrec, scatter_record& srec) const = 0 { return false; }
	virtual double scattering_pdf(const ray& r_in, const hit_record& hrec, const ray& scattered) const { return 0.0; }
	virtual vec3 emitted(const ray& r, const hit_record& hrec) const { return vec3(0.0); }
};

#endif
