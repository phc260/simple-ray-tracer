#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H

#include "cosine_pdf.hpp"
#include "hitable.hpp"
#include "material.hpp"
#include "onb.hpp"
#include "ray.hpp"
#include "texture.hpp"
#include "vec3.hpp"

class lambertian : public material {
public:
	lambertian(texture *a) { albedo = a; }

	virtual double scattering_pdf(const ray& r_in, const hit_record& rec, const ray& scattered) const;
	virtual bool scatter(const ray& r_in, const hit_record& hrec, scatter_record& srec) const;
	
private:
	texture *albedo;
};

double lambertian::scattering_pdf(const ray& r_in, const hit_record& rec, const ray& scattered) const {
	double cosine = dot(rec.normal, scattered.direction());
	cosine = ffmax(cosine, 0.0);
	return cosine / M_PI;
}

bool lambertian::scatter(const ray& r_in, const hit_record& hrec, scatter_record& srec) const {
	srec.is_specular = false;
	srec.attenuation = albedo->value(hrec.u, hrec.v, hrec.point);
	srec.pdf_ptr = new cosine_pdf(hrec.normal);
	return true;
}
#endif

