#ifndef ELLIPSOID_H
#define ELLIPSOID_H

#include "aabb.hpp"
#include "hitable.hpp"
#include "onb.hpp"
#include "vec3.hpp"
#include "ray.hpp"

void get_sphere_uv(const vec3& p, double& u, double& v) {
	double phi = atan2(p.z(), p.x());
	double theta = asin(p.y());
	u = 1.0 - (phi + M_PI) / (2.0*M_PI);
	v = (theta + M_PI_2) / M_PI;
}

class ellipsoid : public hitable {
public:
	ellipsoid() {}
	ellipsoid(vec3 c, vec3 r, material* m);

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const;
	virtual bool bounding_box(double t0, double t1, aabb& box) const;
	virtual double pdf_value(const vec3& o, const vec3& v) const;
	virtual vec3 random(const vec3& o) const;

private:
	vec3 centre;
	vec3 radius;
	double radius_max;
	material *mat_ptr;
};

ellipsoid::ellipsoid(vec3 c, vec3 r, material* m) {
	centre = c; radius = r; mat_ptr = m;
	radius_max = fmax(fmax(radius[0], radius[1]), radius[2]);
}

bool ellipsoid::hit(const ray& r, double t_min, double t_max, hit_record& hrec) const {
	vec3 oc = (r.origin() - centre) / radius;
	vec3 rd = r.direction() / radius;
	double a = dot(rd, rd);
	double b = dot(oc, rd);
	double c = dot(oc, oc) - 1.0;
	double discriminant = b*b - a*c;
	
	if (discriminant >= 0.0) {
		double t = (-b - sqrt(discriminant))/a;
		if (t_min < t && t < t_max) {
			hrec.t = t;
			hrec.point = r.point_at_parameter(t);
			hrec.normal = (hrec.point - centre) / radius;
			hrec.mat_ptr = mat_ptr;
			//get uv
			get_sphere_uv((hrec.point - centre) / radius, hrec.u, hrec.v);
			return true;
		}
		
		/*
		t = (-b + sqrt(discriminant)) / a;
		if (t_min < t && t < t_max) {
			hrec.t = t;
			hrec.point = r.point_at_parameter(t);
			hrec.normal = (hrec.point - centre) / radius;
			hrec.mat_ptr = mat_ptr;
			//get uv
			get_sphere_uv((hrec.point - centre) / radius, hrec.u, hrec.v);
			return true;
		}
		*/
	}
	return false;
}

bool ellipsoid::bounding_box(double t0, double t1, aabb& box) const {
	box = aabb(centre - radius, centre + radius);
	return true;
}

double ellipsoid::pdf_value(const vec3& o, const vec3& v) const {
	hit_record hrec;
	if (this->hit(ray(o, v), IOTA, DBL_MAX, hrec)) {
		// TODO sphere to ellipsoid
		double cos_theta_max = sqrt(1.0 - radius_max * radius_max / (centre - o).squared_length());
		double solid_angle = 2.0 * M_PI*(1.0 - cos_theta_max);
		//return 1.0 / solid_angle / 2.0;
		return 2.0 / solid_angle;
	}
	return 0.0;
}

vec3 ellipsoid::random(const vec3& o) const {
	vec3 direction = centre - o;
	double distance_squared = direction.squared_length();
	onb uvw;
	uvw.build_from_w(direction);

	return uvw.local(random_to_sphere(radius_max, distance_squared));
}
#endif
