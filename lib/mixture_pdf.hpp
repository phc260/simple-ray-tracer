#ifndef MIXTURE_PDF_H
#define MIXTURE_PDF_H

#include "pdf.hpp"

class mixture_pdf : public pdf {
public:
	mixture_pdf(pdf* p0, pdf* p1, double m) { p[0] = p0; p[1] = p1; mix = m; }

	virtual double value(const vec3& direction) const;
	virtual vec3 generate();

private:
	int flag = 0;
	pdf* p[2];
	double mix;
};


double mixture_pdf::value(const vec3& direction) const {
	//return 0.5*p[0]->value(direction) + 0.5*p[1]->value(direction);
	return mix * p[0]->value(direction) + (1.0-mix) * p[1]->value(direction);
}

vec3 mixture_pdf::generate() {
	
	return drand48() < mix ? p[0]->generate() : p[1]->generate();
}
#endif
