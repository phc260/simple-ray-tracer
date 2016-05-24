// TODO euler angle rotation
#ifndef ROTATE_H
#define ROTATE_H

#include <float.h>
#include "aabb.hpp"
#include "hitable.hpp"


vec3 rotate_around(const vec3 &point, const vec3& radians) {
	vec3 p;
	vec3 cos_theta;
	vec3 sin_theta;
	for (int i = 0; i < 3; ++i) {
		sin_theta[i] = sin(radians[i]);
		cos_theta[i] = cos(radians[i]);
	}
	p[0] = cos_theta[1] * cos_theta[2] * point.x()
		+ (cos_theta[2] * sin_theta[0] * sin_theta[1] - cos_theta[0] * sin_theta[2]) * point.y()
		+ (cos_theta[0] * cos_theta[2] * sin_theta[1] + sin_theta[0] * sin_theta[2]) * point.z();
	p[1] = cos_theta[1] * sin_theta[2] * point.x()
		+ (cos_theta[0] * cos_theta[2] + sin_theta[0] * sin_theta[1] * sin_theta[2]) * point.y()
		+ (-cos_theta[2] * sin_theta[0] + cos_theta[0] * sin_theta[1] * sin_theta[2]) * point.z();
	p[2] = -sin_theta[1] * point.x() + cos_theta[1] * sin_theta[0] * point.y() + cos_theta[0] * cos_theta[1] * point.z();
	return p;
}

vec3 rotate_y(const vec3 &point, const vec3& radians) {
	vec3 p;
	vec3 cos_theta;
	vec3 sin_theta;
	for (int i = 0; i < 3; ++i) {
		sin_theta[i] = sin(radians[i]);
		cos_theta[i] = cos(radians[i]);
	}
	p[0] = cos_theta[1] * point.x() + sin_theta[1] * point.z();
	p[1] = point.y();
	p[2] = -sin_theta[1] * point.x() + cos_theta[1] * point.z();
	return p;
}

class rotate: public hitable {
public:
	rotate(hitable *h, const vec3 &angle);
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const;
	virtual bool boundng_box(double t0, double t1, aabb& box) const;

	hitable *ptr;
	vec3 radians;
	bool hasbox;
	aabb bbox;
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
				point = rotate_y(p, radians);
				/*
				point[0] = cos(radians[1]) * p.x() + sin(radians[1]) * p.z();
				point[1] = y;
				point[2] = -sin(radians[1]) * p.x() + cos(radians[1]) * p.z();
				*/
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

	origin = rotate_y(origin, -radians);
	direction = rotate_y(direction, -radians);
	/*
	origin[0] = cos(radians[1]) * r.origin().x() - sin(radians[1]) * r.origin().z();
	origin[1] = r.origin().y();
	origin[2] = sin(radians[1]) * r.origin().x() + cos(radians[1]) * r.origin().z();

	direction[0] = cos(radians[1]) * r.direction().x() - sin(radians[1]) * r.direction().z();
	direction[1] = r.direction().y();
	direction[2] = sin(radians[1]) * r.direction().x() + cos(radians[1]) * r.direction().z();
	*/
	ray rotated_r(origin, direction, r.time());
	if (ptr->hit(rotated_r, t_min, t_max, rec)) {
		rec.point = rotate_y(rec.point, radians);
		rec.normal = rotate_y(rec.normal, radians);
		return true;;
	}

	return false;
}

bool rotate::boundng_box(double t0, double t1, aabb& box) const {
	box = bbox;
	return hasbox;
}
#endif
