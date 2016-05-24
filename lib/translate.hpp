#ifndef TRANSLATE_H
#define TRANSLATE_H

#include "aabb.hpp"
#include "hitable.hpp"
#include "ray.hpp"
#include "vec3.hpp"

class translate : public hitable {
public:
	translate(hitable* h, const vec3& d) { ptr = h; offset = d; }
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const;
	virtual bool bounding_box(double t0, double t1, aabb& box) const;

private:
	hitable *ptr;
	vec3 offset;
};

bool translate::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	ray moved_ray(r.origin()-offset, r.direction(), r.time());
	if (ptr->hit(moved_ray, t_min, t_max, rec)) {
		rec.point += offset;
		return true;
	}
	return false;
}

bool translate::bounding_box(double t0, double t1, aabb& box) const {
	if (ptr->bounding_box(t0, t1, box)) {
		box = aabb(box.min() + offset, box.max() + offset);
		return true;
	}
	return false;
}

#endif
