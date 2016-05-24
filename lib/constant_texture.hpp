#ifndef CONSTANT_TEXTURE_H
#define CONSTANT_TEXTURE_H

#include "texture.hpp"
#include "vec3.hpp"

class constant_texture : public texture {
public:
	constant_texture() {}
	constant_texture(vec3 c) { colour = c; }
	
	virtual vec3 value(double u, double v, const vec3&p) const {
		return colour;
	}

private:
	vec3 colour;
};

#endif
