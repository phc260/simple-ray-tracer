#ifndef MOVING_ELLIPSOID_H
#define MOVING_ELLIPSOID_H

#include "aabb.hpp"
#include "hitable.hpp"
#include "vec3.hpp"
#include "ray.hpp"

class moving_ellipsoid : public hitable {
public:
	moving_ellipsoid() {}
	moving_ellipsoid(vec3 cen0, vec3 cen1, double t0, double t1, vec3 r, material *m) {
		centre0 = cen0; centre1 = cen1; time0 = t0; time1 = t1; radius = r; mat_ptr = m;
	}

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const;
	virtual bool boundng_box(double t0, double t1, aabb& box) const;

	vec3 centre(double time) const;

private:
	vec3 centre0, centre1;
	double time0, time1;
	vec3 radius;
	material *mat_ptr;
};

vec3 moving_ellipsoid::centre(double time) const {
	return centre0 + ((time - time0) / (time1 - time0))*(centre1 - centre0);
}

bool moving_ellipsoid::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	vec3 moving_centre = centre(r.time());
	vec3 oc = (r.origin() - moving_centre) / radius;
	vec3 rd = r.direction() / radius;
	double a = dot(rd, rd);
	double b = dot(oc, rd);
	double c = dot(oc, oc) - 1.0;
	double discriminant = b*b - a*c;
	if (discriminant >= 0.0) {
		double t = (-b - sqrt(discriminant)) / a;
		if (t_min < t && t < t_max) {
			rec.t = t;
			rec.point = r.point_at_parameter(t);
			rec.normal = (rec.point - moving_centre) / radius;
			rec.mat_ptr = mat_ptr;
			return true;
		}
		t = (-b + sqrt(discriminant)) / a;
		if (t_min < t && t < t_max) {
			rec.t = t;
			rec.point = r.point_at_parameter(t);
			rec.normal = (rec.point - moving_centre) / radius;
			rec.mat_ptr = mat_ptr;
			return true;
		}
	}
	return false;
}

bool moving_ellipsoid::boundng_box(double t0, double t1, aabb& box) const {
	aabb box0(centre0 - radius, centre0 + radius);
	aabb box1(centre1 - radius, centre1 + radius);
	box = surrounding_box(box0, box1);
	return true;
}

#endif