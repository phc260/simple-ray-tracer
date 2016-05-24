// TODO unfinished

#ifndef CONSTANT_MEDIUM_H
#define CONSTANT_MEDIUM_H

#include <float.h>
#include "isotropic.hpp"
#include "hitable.hpp"
#include "texture.hpp"

class constant_medium : public hitable {
public:
	constant_medium(hitable *h, double d, texture *t) {
		boundary = h;
		density = d;
		phase_function = new isotropic(t);
	}

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const;
	virtual bool bounding_box(double t0, double t1, aabb& box) const;

	hitable *boundary;
	double density;
	material *phase_function;
};

bool constant_medium::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	bool db = (drand48() < IOTA);
	db = false;
	hit_record rec1, rec2;

	if (boundary->hit(r, -DBL_MAX, DBL_MAX, rec1)) {
		if (boundary->hit(r, -DBL_MAX, DBL_MAX, rec2)) {
			if (db) std::cerr << "t0 = " << rec1.t << ", t1 = " << rec2.t << "\n";
			if (t_min >= t_max) return false;
			rec1.t = ffmax(rec1.t, ffmax(t_min, 0.0));
			rec2.t = ffmax(rec2.t, t_max);

			// r.direction() == 1
			double distant_inside_boundary = rec2.t - rec1.t;

			double hit_distance = (-1.0 / density) * log(drand48());

			if (hit_distance < distant_inside_boundary) {
				if (db) {

				}
				rec.t = rec1.t + hit_distance;
				rec.point = r.point_at_parameter(rec.t);
				rec.normal = vec3(1.0, 0.0, 0.0); // arbitrary
				rec.mat_ptr = phase_function;
				return true;
			}
			
		}
	}
	return false;
}

bool constant_medium::bounding_box(double t0, double t1, aabb& box) const {
	return boundary->bounding_box(t0, t1, box);
}

#endif
