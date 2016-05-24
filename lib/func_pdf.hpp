#ifndef FUNC_PDF_H
#define FUNC_PDF_H

#include "pdf.hpp"

class func_pdf : public pdf {
public:
	func_pdf(vec3(*ptr)()) { func_ptr = ptr; }
	virtual double value(const vec3& direction) const;
	virtual vec3 generate() const;

private:
	vec3(*func_ptr)();
};

double func_pdf::value(const vec3& direction) const {
	return 0.5 / M_PI;
}

vec3 func_pdf::generate() const {
	return func_ptr();
}
#endif
