#ifndef AABB_H
#define AABB_H

#include "ray.hpp"
#include "vec3.hpp"

inline double ffmin(double a, double b) { return a < b ? a : b; }
inline double ffmax(double a, double b) { return a > b ? a : b; }

class aabb {
public:
	aabb() {}
	aabb(const vec3& a, const vec3& b) { _min = a; _max = b; }

	vec3 min() const { return _min; }
	vec3 max() const { return _max; }
	inline bool aabb::hit(const ray& r, double tmin, double tmax) const;

private:
	vec3 _min, _max;
};

inline bool aabb::hit(const ray& r, double tmin, double tmax) const {
	vec3 o = r.origin();
	vec3 d = r.direction();
	for (int i = 0; i < 3; ++i) {
		double invD = 1.0 / d[i];
		double t0 = (min()[i] - o[i])*invD;
		double t1 = (max()[i] - o[i])*invD;

		if (invD < 0.0) std::swap(t0, t1);

		tmin = t0 > tmin ? t0 : tmin;
		tmax = t1 < tmax ? t1 : tmax;

		if (tmin >= tmax) return false;
	}
	return true;
}

aabb surrounding_box(aabb& box0, aabb& box1) {
	vec3 small(fmin(box0.min().x(), box1.min().x()),
		fmin(box0.min().y(), box1.min().y()),
		fmin(box0.min().z(), box1.min().z()));
	vec3 big(fmin(box0.max().x(), box1.max().x()),
		fmin(box0.max().y(), box1.max().y()),
		fmin(box0.max().z(), box1.max().z()));
	return aabb(small, big);
}

#endif
