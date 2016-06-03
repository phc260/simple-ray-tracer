#ifndef BOX_H
#define BOX_H

#include <vector>
#include "aabb.hpp"
#include "hitable_list.hpp"
#include "quadrilateral.hpp"

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
	
	vec3 v0(p0.x(), p1.y(), p0.z());
	vec3 v1(p0.x(), p1.y(), p1.z());
	vec3 v2(p1.x(), p1.y(), p1.z());
	vec3 v3(p1.x(), p1.y(), p0.z());
	vec3 v4(p0.x(), p0.y(), p0.z());
	vec3 v5(p0.x(), p0.y(), p1.z());
	vec3 v6(p1.x(), p0.y(), p1.z());
	vec3 v7(p1.x(), p0.y(), p0.z());

	list.push(new quadrilateral(v0, v1, v2, v3, m));
	list.push(new quadrilateral(v7, v6, v5, v4, m));
	list.push(new quadrilateral(v1, v0, v4, v5, m));
	list.push(new quadrilateral(v3, v2, v6, v7, m));
	list.push(new quadrilateral(v4, v0, v3, v7, m));
	list.push(new quadrilateral(v2, v1, v5, v6, m));
}
#endif
