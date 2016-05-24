#ifndef DIFFUSE_LIGHT_H
#define DIFFUSE_LIGHT_H


#include "material.hpp"
#include "texture.hpp"

class diffuse_light : public material {
public: 
	diffuse_light(texture *a) { emit = a; }

	virtual bool scatter(const ray& r_in, const hit_record& hrec, scatter_record& srec) const { return false; }
	virtual vec3 emitted(const ray& r, const hit_record& rec) const;

private:
	texture *emit;
};

vec3 diffuse_light::emitted(const ray& r, const hit_record& rec) const {
	if (dot(r.direction(), rec.normal) < 0.0)
		return emit->value(rec.u, rec.v, rec.point);
	else
		return vec3(0.0);
}
#endif
