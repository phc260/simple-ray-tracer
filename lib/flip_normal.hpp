#ifndef FLIP_NORMAL_H
#define FLIP_NORMAL_H

#include "hitable.hpp"

class flip_normal : public hitable {
public:
	flip_normal(hitable *h) { ptr = h; }

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
		if(ptr->hit(r, t_min, t_max, rec)) {
			rec.normal = -rec.normal;
			return true;
		}
		return false;
	}

private:
	hitable *ptr;
};

#endif
