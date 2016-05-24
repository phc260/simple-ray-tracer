#ifndef BOX_H
#define BOX_H

#include <vector>
#include "aabb.hpp"
#include "flip_normal.hpp"
#include "hitable_list.hpp"
#include "xy_rect.hpp"
#include "xz_rect.hpp"
#include "yz_rect.hpp"

class box : public hitable {
public:
	box() {}
	box(const vec3& p0, const vec3& p1, material *m);

	virtual bool hit(const ray& r, double t0, double t1, hit_record& rec) const {
		return list.hit(r, t0, t1, rec);
	}
	virtual bool bounding_box(double t0, double t1, aabb& box) const {
		box = aabb(pmin, pmax);
		return true;
	}

private:
	vec3 pmin;
	vec3 pmax;
	hitable_list list;
};


box::box(const vec3& p0, const vec3& p1, material *m) {
	pmin = p0; pmax = p1;
	
	list.push(new xy_rect(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), m));
	list.push(new flip_normal(new xy_rect(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), m)));
	list.push(new xz_rect(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), m));
	list.push(new flip_normal(new xz_rect(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), m)));
	list.push(new yz_rect(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), m));
	list.push(new flip_normal(new yz_rect(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), m)));
	
}
#endif
