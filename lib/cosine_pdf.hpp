#ifndef COSINE_PDF_H
#define COSINE_PDF_H

#include "onb.hpp"
#include "pdf.hpp"
#include "vec3.hpp"

class cosine_pdf : public pdf {
public:
	cosine_pdf(const vec3& w);

	virtual double value(const vec3& direction) const;
	virtual vec3 generate();

private:
	onb uvw;
};

cosine_pdf::cosine_pdf(const vec3& w) {
	uvw.build_from_w(w);
}

double cosine_pdf::value(const vec3& direction) const {
	double cosine = dot(direction, uvw.w());
	return cosine > 0.0 ? cosine / M_PI : 0.0;
}

vec3 cosine_pdf::generate() {
	return uvw.local(random_cosine_direction());
}
#endif