#ifndef HITABLE_H
#define HITABLE_H

//#include "material.hpp"
#include "aabb.hpp"
#include "ray.hpp"
#include "vec3.hpp"

class material;

struct hit_record {
	double t = 0.0;
	vec3 point;
	vec3 normal;
	double u = 0.0, v = 0.0;
	material *mat_ptr = nullptr;
};

class hitable {
public:
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0 { return false; }
	virtual bool bounding_box(double t0, double t1, aabb& box) const { return false; }
	virtual double pdf_value(const vec3& o, const vec3& v) const { return 0.0; }
	virtual vec3 random(const vec3& o) const { return vec3(1.0, 0.0, 0.0); }
};

#endif
