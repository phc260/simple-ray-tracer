#ifndef TEXTURE_H
#define TEXTURE_H

#include "vec3.hpp"

class texture {
public:
	virtual vec3 value(double u, double v, const vec3&p) const = 0;
};

#endif
