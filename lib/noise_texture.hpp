#ifndef NOISE_TEXTURE_H
#define NOISE_TEXTURE_H

#include <math.h>
#include "vec3.hpp"
#include "texture.hpp"
#include "perlin.hpp"

double turb(const vec3& p, int depth, const perlin& noise) {
	double accum = 0.0;
	vec3 tmp = p;
	double w = 1.0;
	for (int i = 0; i < 7; ++i) {
		accum += w * noise.noise(tmp);
		w *= 0.5;
		tmp *= 2.0;
	}
	return fabs(accum);
}

class noise_texture : public texture {
public:
	noise_texture() { scale = 1.0; }
	noise_texture(double sc) { scale = sc; }

	virtual vec3 value(double u, double v, const vec3& p) const {
		double k = sin(scale*p.x() + 10 * turb(p, 7, noise));
		return vec3(0.5*(1 +  k));
	}

private:
	perlin noise;
	double scale;
};

#endif
