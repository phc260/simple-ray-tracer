// TODO euler angle rotation
#ifndef ROTATE_H
#define ROTATE_H

#include <float.h>
#include "aabb.hpp"
#include "hitable.hpp"

vec3 euler_rotate(const vec3 &v, const vec3 &radians) {
	vec3 q;

	vec3 c, s;
	int p = 0, h = 1, r = 2;
	for (int i = 0; i < 3; ++i) {
		c[i] = cos(radians[i]);
		s[i] = sin(radians[i]);
	}
	q[0] = (c[r] * c[h] - s[r] * s[p] * s[h]) * v[0] + (-s[r] * c[p]) * v[1] + (c[r] * s[h] + s[r] * s[p] * c[h]) *v[2];
	q[1] = (s[r] * c[h] + c[r] * s[p] * s[h]) *v[0] + (c[r] * c[p]) *v[1] + (s[r] * s[h] - c[r] * s[p] * c[h]) *v[2];
	q[2] = (-c[p] * s[h]) *v[0] + s[p] * v[1] + (c[p] * c[h]) *v[2];
	return q;
}

vec3 euler_rotate(const vec3 &v, const vec3 m[3]) {
	vec3 q;

	q[0] = dot(v, m[0]);
	q[1] = dot(v, m[1]);
	q[2] = dot(v, m[2]);
	return q;
}

class rotate: public hitable {
public:
	rotate(hitable *h, const vec3 &angle);
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const;
	virtual bool boundng_box(double t0, double t1, aabb& box) const;

private:
	hitable *ptr;
	vec3 radians;
	bool hasbox;
	aabb bbox;
	vec3 mat[3];
	vec3 neg_mat[3];
};

rotate::rotate(hitable *h, const vec3 &angle) {
	radians = (M_PI / 180.0) * angle;
	ptr = h;
	
	hasbox = ptr->bounding_box(0.0, 1.0, bbox);
	vec3 min(DBL_MAX);
	vec3 max(-DBL_MAX);

	vec3 point;
	for (int i = 0; i < 2; ++i)
		for (int j = 0; j < 2; ++j)
			for (int k = 0; k < 2; ++k) {
				double x = i*bbox.max().x() + (1 - i)*bbox.min().x();
				double y = j*bbox.max().y() + (1 - j)*bbox.min().y();
				double z = k*bbox.max().z() + (1 - k)*bbox.min().z();
				
				vec3 p(x, y, z);
				point = euler_rotate(p, radians);
				for (int c = 0; c < 3; ++c) {
					max[c] = ffmax(point[c], max[c]);
					min[c] = ffmin(point[c], min[c]);
				}
				
			}
	bbox = aabb(min, max);
}

bool rotate::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	vec3 origin = r.origin();
	vec3 direction = r.direction();

	origin = euler_rotate(origin, -radians);
	direction = euler_rotate(direction, -radians);

	ray rotated_r(origin, direction, r.time());
	if (ptr->hit(rotated_r, t_min, t_max, rec)) {
		rec.point = euler_rotate(rec.point, radians);
		rec.normal = euler_rotate(rec.normal, radians);
		return true;;
	}

	return false;
}

bool rotate::boundng_box(double t0, double t1, aabb& box) const {
	box = bbox;
	return hasbox;
}
#endif
