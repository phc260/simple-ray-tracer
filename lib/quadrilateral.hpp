#ifndef QUADRILATERAL_H
#define QUADRILATERAL_H
// using naive method, seperating it into 2 triangles
#include <vector>
#include "triangle.hpp"

class quadrilateral : public hitable {
public:
	quadrilateral(vec3 p0, vec3 p1, vec3 p2, vec3 p3, material *m);

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& hrec) const;
	virtual bool bounding_box(double t0, double t1, aabb& box) const;
	virtual double pdf_value(const vec3& o, const vec3& v) const;
	virtual vec3 random(const vec3& o) const;

private:
	std::vector<triangle*> triangles;
	material *mat_ptr;
};

quadrilateral::quadrilateral(vec3 p0, vec3 p1, vec3 p2, vec3 p3, material *m) {
	
	triangles.push_back(new triangle(p0, p1, p2, m));
	triangles.push_back(new triangle(p0, p2, p3, m));
	mat_ptr = m;
}

bool quadrilateral::hit(const ray& r, double t_min, double t_max, hit_record& hrec) const {
	for (int i = 0; i < triangles.size(); ++i) {
		if (triangles[i]->hit(r, t_min, t_max, hrec)) {
			return true;
		}
	}
	return false;
}

bool quadrilateral::bounding_box(double t0, double t1, aabb& box) const {
	return true;
}

double quadrilateral::pdf_value(const vec3& o, const vec3& v) const {
	double val = 0.0;
	for (int i = 0; i < triangles.size(); ++i) {
		val += triangles[i]->pdf_value(o, v);
	}
	return val;
}

vec3 quadrilateral::random(const vec3& o) const {
	return triangles[rand() % triangles.size()]->random(o);
}
#endif