// TODO unfinished
#ifndef ISOTROPIC_H
#define ISOTROPIC_H

#include "material.hpp"
#include "texture.hpp"
#include "cosine_pdf.hpp"
#include "func_pdf.hpp"

class isotropic : public material {
public:
	isotropic(texture *a) : albedo(a) {}
	virtual bool scatter(const ray& r_in, const hit_record& hrec, scatter_record& srec) const {
		//scattered = ray(hrec.point, random_in_unit_sphere());
		srec.is_specular = false;
		srec.attenuation = albedo->value(hrec.u, hrec.v, hrec.point);
		//srec.pdf_ptr = new func_pdf(random_unit_vec3);
		srec.pdf_ptr = new cosine_pdf(hrec.normal);
		return true;
	}

	texture *albedo;
};

#endif
