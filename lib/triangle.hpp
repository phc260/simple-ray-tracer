// TODO unfinished
#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hitable.hpp"

class triangle : public hitable {
public :
	triangle(vec3 v0, vec3 v1, vec3 v2, material *m);

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& hrec) const;
	virtual bool bounding_box(double t0, double t1, aabb& box) const;
	virtual double pdf_value(const vec3& o, const vec3& v) const;
	virtual vec3 random(const vec3& o) const;

	vec3 get_normal() const { return normal; }

private:
	vec3 v[3];
	vec3 e[2];
	vec3 normal;
	double area;
	material *mat_ptr;
};

triangle::triangle(vec3 v0, vec3 v1, vec3 v2, material *m) {
	v[0] = v0; v[1] = v1; v[2] = v2;
	e[0] = v[1] - v[0];
	e[1] = v[2] - v[0];
	normal = normalize(cross(e[0], e[1]));
	area = 0.5 * cross(e[0], e[1]).length();
	mat_ptr = m;
}

// Möller–Trumbore intersection algorithm
bool triangle::hit(const ray& r, double t_min, double t_max, hit_record& hrec) const {
	vec3 pvec, qvec, tvec;
	double det, inv_det, t, u, v;

	pvec = cross(r.direction(), e[1]);

	det = dot(e[0], pvec);

	if (fabs(det) < IOTA) return false;

	inv_det = 1.0 / det;

	tvec = r.origin() - this->v[0];

	u = dot(tvec, pvec) * inv_det;

	if (u < 0.0 || u> 1.0) return false;

	qvec = cross(tvec, this->e[0]);

	v = dot(r.direction(), qvec) * inv_det;

	if (v < 0.0 || u + v > 1.0) return false;

	t = dot(this->e[1], qvec) * inv_det;

	if (t < t_min || t > t_max) return false;
	hrec.t = t;
	hrec.point = r.origin() + r.direction() * t;
	hrec.normal = normal;
	hrec.mat_ptr = mat_ptr;

	return true;
}

bool triangle::bounding_box(double t0, double t1, aabb& box) const {
	vec3 a = e[0], b = e[0];

	for (int i = 1; i < 2; ++i) {
		for (int j = 0; j < 2; ++j) {
			if (e[i][j] < a[j]) {
				a[j] = e[i][j];
			}
			if (e[i][j] > b[j]) {
				b[j] = e[i][j];
			}
		}
	}
	
	box = aabb(a, b + IOTA);

	return true;
}

double triangle::pdf_value(const vec3& o, const vec3& v) const {
	hit_record hrec;
	if (this->hit(ray(o, v), IOTA, DBL_MAX, hrec)) {
		double distance_squared = hrec.t * hrec.t;
		double cosine = fabs(dot(v, hrec.normal));
		double cos_area = cosine * area;
		return distance_squared / cos_area;
	}
	return 0.0;
}

vec3 triangle::random(const vec3& o) const {
	float b0 = drand48();
	float b1 = (1.0f - b0) * drand48();
	float b2 = 1 - b0 - b1;

	vec3 p = v[0] * b0 + v[1] * b1 + v[2] * b2;
	return p - o;
}
#endif
