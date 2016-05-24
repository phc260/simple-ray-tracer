#ifndef PDF_H
#define PDF_H

#include "vec3.hpp"

class pdf {
public:
	virtual double value(const vec3& direction) const = 0;
	virtual vec3 generate() { return vec3(1.0, 0.0, 0.0); };
};

#endif
