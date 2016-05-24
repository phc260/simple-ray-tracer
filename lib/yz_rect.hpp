#ifndef YZ_RECT_H
#define YZ_RECT_H

#include "hitable.hpp"
#include "material.hpp"

class yz_rect : public hitable {
public:
	yz_rect() {}
	yz_rect(double y0, double y1, double z0, double z1, double k, material *m) {
		this->y0 = y0; this->y1 = y1;
		this->z0 = z0; this->z1 = z1;
		this->k = k; this->mat_ptr = m;
	}

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const;
	virtual bool bounding_box(double t0, double t1, aabb& box) const;

private:
	double y0, y1, z0, z1, k;
	material *mat_ptr;
};


bool yz_rect::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	double t = (k - r.origin().x()) / r.direction().x();

	if (t<t_min || t>t_max) return false;

	double y = r.origin().y() + t*r.direction().y();
	double z = r.origin().z() + t*r.direction().z();

	if (y<y0 || y>y1 || z<z0 || z>z1) return false;

	rec.u = (y - y0) / (y1 - y0);
	rec.v = (z - z0) / (z1 - z0);
	rec.t = t;
	rec.mat_ptr = mat_ptr;
	rec.point = r.point_at_parameter(t);
	rec.normal = vec3(1.0, 0.0, 0.0);
	return true;
}

bool yz_rect::bounding_box(double t0, double t1, aabb& box) const {
	box = aabb(vec3(y0, z0, k - IOTA), vec3(y0, z0, k + IOTA));
	return true;
}
#endif
