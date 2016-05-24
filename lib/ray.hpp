#ifndef RAY_H
#define RAY_H

#include "vec3.hpp"

class ray {
public:
	ray() {};
	ray(const vec3& ro, const vec3& rd, double ti=0.0) { this->ro = ro; this->rd = normalize(rd); this->_time = ti; }

	vec3 origin() const { return ro; }
	vec3 direction() const { return rd; }
	double time() const { return _time; }
	vec3 point_at_parameter(double t) const { return ro + rd*t; }

private:
	vec3 ro;
	vec3 rd;
	double _time;
};

#endif
