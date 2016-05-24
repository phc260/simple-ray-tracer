#ifndef HITABLE_PDF_H
#define HITABLE_PDF_H

#include "pdf.hpp"
class hitable_pdf : public pdf {
public:
	hitable_pdf(hitable *p, const vec3& o) { ptr = p; origin = o; }

	virtual double value(const vec3& direction) const { return ptr->pdf_value(origin, direction); }
	virtual vec3 generate() { return ptr->random(origin); }

private:
	hitable *ptr;
	vec3 origin;
};
#endif
