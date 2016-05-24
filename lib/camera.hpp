#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include "ray.hpp"
#include "vec3.hpp"

class camera {
public:
	camera(const vec3& lookfrom, const vec3& lookat, const vec3& vup, double vfov, double aspect, double aperture = 0.0, double focus_dist = 1.0, double t0 = 0.0, double t1 = 0.0);

	ray get_ray(double s, double t);
	

private:
	vec3 origin;
	vec3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
	vec3 u, v, w;
	double time0, time1;
	double lens_radius;
};

camera::camera(const vec3& lookfrom, const vec3& lookat, const vec3& vup, double vfov, double aspect, double aperture, double focus_dist, double t0, double t1) {
	time0 = t0; time1 = t1;
	double theta = vfov * M_PI / 180.0;
	double half_height = tan(theta / 2.0);
	double half_width = aspect * half_height;

	origin = lookfrom;

	w = normalize(lookfrom - lookat);
	u = normalize(cross(vup, w));
	v = normalize(cross(w, u));

	lower_left_corner = origin - (half_width*u + half_height*v + w) * focus_dist;
	horizontal = 2.0 * half_width * focus_dist * u;
	vertical = 2.0 * half_height * focus_dist* v;

	lens_radius = aperture * 0.5;
}


ray camera::get_ray(double s, double t) {
	double time = 0.0;

	if (time0 < time1 && time1 > 0.0) {
		time = time0 + drand48()*(time1 - time0);
	}

	if (lens_radius > IOTA) {
		double theta = 2.0 * M_PI * drand48();
		vec3 offset = cos(theta) * u + sin(theta) * v;
		return ray(origin + offset, lower_left_corner + horizontal*s + vertical*t - origin - offset, time);
	}

	return ray(origin, lower_left_corner + horizontal*s + vertical*t - origin, time);
}

#endif
