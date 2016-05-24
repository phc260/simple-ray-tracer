#ifndef CHECKER_TEXTURE_H
#define CHECKER_TEXTURE_H

#include "texture.hpp"
#include "vec3.hpp"

class checker_texture : public texture {
public:
	checker_texture() {}
	checker_texture(texture *t0, texture *t1) { even = t0; odd = t1; }

	virtual vec3 value(double u, double v, const vec3&p) const {
		double sines = sin(10.0 * p.x()) * sin(10.0 * p.y()) * sin(1.0 * p.z());
		return sines < 0.0 ? odd->value(u, v, p) : even->value(u, v, p);
	}

private:
	texture *odd;
	texture *even;
};

#endif
