#ifndef XY_RECT_H
#define XY_RECT_H

#include "hitable.hpp"
#include "material.hpp"

class xy_rect : public hitable {
public:
	xy_rect() {}
	xy_rect(double x0, double x1, double y0, double y1, double k, material *m) {
		this->x0 = x0; this->x1 = x1;
		this->y0 = y0; this->y1 = y1;
		this->k = k; this->mat_ptr = m;
	}
	
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const;
	virtual bool bounding_box(double t0, double t1, aabb& box) const;

private:
	double x0, x1, y0, y1, k;
	material *mat_ptr;
};


bool xy_rect::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	double t = (k - r.origin().z()) / r.direction().z();

	if (t<t_min || t>t_max) return false;

	double x = r.origin().x() + t*r.direction().x();
	double y = r.origin().y() + t*r.direction().y();

	if (x<x0 || x>x1 || y<y0 || y>y1) return false;

	rec.u = (x - x0) / (x1 - x0);
	rec.v = (y - y0) / (y1 - y0);
	rec.t = t;
	rec.mat_ptr = mat_ptr;
	rec.point = r.point_at_parameter(t);
	rec.normal = vec3(0.0, 0.0, 1.0);
	return true;
}

bool xy_rect::bounding_box(double t0, double t1, aabb& box) const {
	box = aabb(vec3(x0, y0, k - IOTA), vec3(x0, y0, k + IOTA));
	return true;
}
#endif
