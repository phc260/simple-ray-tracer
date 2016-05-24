#ifndef VEC3_H
#define VEC3_H

#define _USE_MATH_DEFINES
#define IOTA 0.0001

#include <math.h>
#include <stdlib.h>
#include <iostream>

double drand48() {
	return double(rand()) / RAND_MAX;
}

class vec3 {
public:
	// constructors
	vec3() { e[0] = e[1] = e[2] = 0.0; }
	vec3(double e0) { e[0] = e[1] = e[2] = e0; }
	vec3(double e0, double e1, double e2) { e[0] = e0; e[1] = e1; e[2] = e2; }

	inline double x() const { return e[0]; }
	inline double y() const { return e[1]; }
	inline double z() const { return e[2]; }
	inline double r() const { return e[0]; }
	inline double g() const { return e[1]; }
	inline double b() const { return e[2]; }

	inline const vec3& operator+() const { return *this; }
	inline vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
	inline double operator[](int i) const { return e[i]; }
	inline double& operator[](int i) { return e[i]; }

	inline vec3& operator+= (const vec3 &v2);
	inline vec3& operator-= (const vec3 &v2);
	inline vec3& operator*= (const vec3 &v2);
	inline vec3& operator/= (const vec3 &v2);
	inline vec3& operator*= (const double t);
	inline vec3& operator/= (const double t);

	inline double length() const { return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]); }
	inline double squared_length() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }

	inline void normalize() {
		double k = 1.0 / length();
		e[0] *= k; e[1] *= k; e[2] *= k;
	}


	double e[3];
};

/*
member functions
*/
inline vec3 operator+(const vec3& lhs, const vec3& rhs) { return vec3(lhs[0] + rhs[0], lhs[1] + rhs[1], lhs[2] + rhs[2]); }
inline vec3 operator-(const vec3& lhs, const vec3& rhs) { return vec3(lhs[0] - rhs[0], lhs[1] - rhs[1], lhs[2] - rhs[2]); }
inline vec3 operator*(const vec3& lhs, const vec3& rhs) { return vec3(lhs[0] * rhs[0], lhs[1] * rhs[1], lhs[2] * rhs[2]); }
inline vec3 operator/(const vec3& lhs, const vec3& rhs) { return vec3(lhs[0] / rhs[0], lhs[1] / rhs[1], lhs[2] / rhs[2]); }
inline vec3 operator*(const vec3& lhs, double t) { return vec3(lhs[0] * t, lhs[1] * t, lhs[2] * t); }
inline vec3 operator/(const vec3& lhs, double t) { t = 1.0 / t;  return vec3(lhs[0]*t, lhs[1]*t, lhs[2]*t); }

inline vec3& vec3::operator+=(const vec3& v) {
	e[0] += v.e[0];
	e[1] += v.e[1];
	e[2] += v.e[2];
	return *this;
}

inline vec3& vec3::operator-=(const vec3& v) {
	e[0] -= v.e[0];
	e[1] -= v.e[1];
	e[2] -= v.e[2];
	return *this;
}

inline vec3& vec3::operator*=(const vec3& v) {
	e[0] *= v.e[0];
	e[1] *= v.e[1];
	e[2] *= v.e[2];
	return *this;
}

inline vec3& vec3::operator/=(const vec3& v) {
	e[0] /= v.e[0];
	e[1] /= v.e[1];
	e[2] /= v.e[2];
	return *this;
}

inline vec3& vec3::operator*=(const double t) {
	e[0] *= t;
	e[1] *= t;
	e[2] *= t;
	return *this;
}

inline vec3& vec3::operator/=(const double t) {
	double s = 1.0 / t;
	e[0] *= s;
	e[1] *= s;
	e[2] *= s;
	return *this;
}

inline std::istream& operator>>(std::istream& is, vec3 &v) {
	is >> v.e[0] >> v.e[1] >> v.e[2];
	return is;
}

inline std::ostream& operator<<(std::ostream& os, const vec3& v) {
	os << "(" << v.x() << "," << v.y() << "," << v.z() << ")";
	return os;
}

/*
global functions
*/
inline vec3 normalize(const vec3 v) { return v / v.length(); }
inline double dot(const vec3& lhs, const vec3& rhs) { return lhs[0] * rhs[0] + lhs[1] * rhs[1] + lhs[2] * rhs[2]; }
inline vec3 mix(const vec3& lhs, const vec3&rhs, double t) { return lhs*(1.0 - t) + rhs*t; }
inline vec3 cross(const vec3& lhs, const vec3& rhs) {
	return vec3(lhs[1] * rhs[2] - lhs[2] * rhs[1],
		lhs[2] * rhs[0] - lhs[0] * rhs[2],
		lhs[0] * rhs[1] - lhs[1] * rhs[0]);
}
/*
inline vec3 random_unit_vec3() {
	vec3 p;
	double theta = 2.0 * M_PI * drand48();
	double phi = acos(-1.0 + 2.0 * drand48());
	p[0] = sin(phi) * cos(theta);
	p[1] = sin(phi) * sin(theta);
	p[2] = cos(phi);
	return p;
}
*/
inline vec3 random_unit_vec3() {
	vec3 p;
	double theta = 2.0 * M_PI * drand48();
	double phi = acos(-1.0 + 2.0 * drand48());
	p[0] = sin(phi) * cos(theta);
	p[1] = sin(phi) * sin(theta);
	p[2] = cos(phi);
	return p;
}

inline vec3 random_on_hemisphere() {
	vec3 p;
	double theta = 2.0 * M_PI * drand48();
	double phi = acos(-1.0 + 2.0 * drand48());
	p[0] = sin(phi) * cos(theta);
	p[1] = sin(phi) * sin(theta);
	p[2] = fabs(cos(phi));
	return p;
}

inline vec3 random_in_unit_sphere() {
	vec3 p;
	double r = drand48();
	double theta = 2.0 * M_PI * drand48();
	double phi = acos(-1.0 + 2.0 * drand48());
	p[0] = r * sin(phi) * cos(theta);
	p[1] = r * sin(phi) * sin(theta);
	p[2] = r * cos(phi);
	return p;
}

inline vec3 random_cosine_direction() {
	double x, y, z;
	double r1 = drand48();
	double r2 = drand48();
	double phi = 2.0*M_PI*r1;
	z = sqrt(1.0 - r2);
	x = cos(phi)*2.0*sqrt(r2);
	y = sin(phi)*2.0*sqrt(r2);

	return vec3(x, y, z);
}

inline vec3 random_to_sphere(double radius, double distance_squared) {
	double r1 = drand48();
	double r2 = drand48();
	double z = 1.0 + r2*(sqrt(1.0 - radius*radius / distance_squared) - 1.0);
	double phi = 2.0*M_PI*r1;
	double x = cos(phi)*sqrt(1.0 - z*z);
	double y = sin(phi)*sqrt(1.0 - z*z);
	return vec3(x, y, z);
}

inline vec3 de_nan(const vec3& c) {
	vec3 p = c;
	if (isnan(p[0])) p[0] = 0.0;
	if (isnan(p[1])) p[1] = 0.0;
	if (isnan(p[2])) p[2] = 0.0;
	return p;
}

inline vec3 de_nan(const vec3& c, const vec3& prev) {
	vec3 p = c;
	if (isnan(p[0])) p[0] = prev[0];
	if (isnan(p[1])) p[1] = prev[1];
	if (isnan(p[2])) p[2] = prev[2];
	return p;
}

inline vec3 reflect(const vec3& v, const vec3& n) {
	return v - 2.0*dot(v, n)*n;
}

inline vec3 refract(const vec3& v, const vec3& n, double ior) {
	double k = 1.0 - ior * ior * (1.0 - dot(v, n)*dot(v, n));
	return k > 0 ? ior * v - (ior*dot(v, n) + sqrt(k))*n : vec3(0.0);
}

inline vec3 clamp(const vec3& v, double m, double M) {
	vec3 p;
	p[0] = fmax(fmin(v[0], M), m);
	p[1] = fmax(fmin(v[1], M), m);
	p[2] = fmax(fmin(v[2], M), m);
	return p;
}

inline vec3 gamma_correction(vec3& c) {
	return vec3(pow(c[0], 0.4545), pow(c[1], 0.4545), pow(c[2], 0.4545));
}
#endif
