#ifndef XZ_RECT_H
#define XZ_RECT_H

#include "hitable.hpp"
#include "material.hpp"

class xz_rect : public hitable {
public:
	xz_rect() {}
	xz_rect(double x0, double x1, double z0, double z1, double k, material *m) {
		this->x0 = x0; this->x1 = x1;
		this->z0 = z0; this->z1 = z1;
		area = (x1 - x0)*(z1 - z0);
		this->k = k; this->mat_ptr = m;
	}

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const;
	virtual bool bounding_box(double t0, double t1, aabb& box) const;
	virtual double pdf_value(const vec3& o, const vec3& v) const;
	virtual vec3 random(const vec3& o) const;

private:
	double x0, x1, z0, z1, k;
	double area;
	material *mat_ptr;
};


bool xz_rect::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	double t = (k - r.origin().y()) / r.direction().y();

	if (t<t_min || t>t_max) return false;

	double x = r.origin().x() + t*r.direction().x();
	double z = r.origin().z() + t*r.direction().z();

	if (x<x0 || x>x1 || z<z0 || z>z1) return false;

	rec.u = (x - x0) / (x1 - x0);
	rec.v = (z - z0) / (z1 - z0);
	rec.t = t;
	rec.mat_ptr = mat_ptr;
	rec.point = r.point_at_parameter(t);
	rec.normal = vec3(0.0, 1.0, 0.0);
	return true;
}

bool xz_rect::bounding_box(double t0, double t1, aabb& box) const {
	box = aabb(vec3(x0, z0, k - IOTA), vec3(x0, z0, k + IOTA));
	return true;
}

double xz_rect::pdf_value(const vec3& o, const vec3& v) const {
	hit_record rec;
	
	if (this->hit(ray(o, v), IOTA, DBL_MAX, rec)) {
		double distance_squared = rec.t * rec.t;
		double cosine = fabs(dot(v, rec.normal));
		double cos_area = cosine * area;
		return distance_squared / cos_area;
	}
	return 0.0;
}

vec3 xz_rect::random(const vec3& o) const {
	vec3 p(x0 + drand48()*(x1 - x0), k, z0 + drand48()*(z1 - z0));
	return p - o;
}
#endif
