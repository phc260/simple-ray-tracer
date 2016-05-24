#ifndef HITABLELIST_H
#define HITABLELIST_H

#include <vector>
#include "hitable.hpp"

class hitable_list : public hitable {
public:
	hitable_list() {}
	hitable_list(std::vector<hitable*>& l) { list = l; }

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const;
	virtual double pdf_value(const vec3& o, const vec3& v) const;
	virtual vec3 random(const vec3& o) const;

	void push(hitable *h) { list.push_back(h); }
	int size() const { return list.size(); }

	std::vector<hitable*> list;
};

bool hitable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	hit_record new_rec;
	bool hit_anything = false;
	double closet_so_far = t_max;
	for (int i = 0; i < size(); ++i) {
		if (list[i]->hit(r, t_min, closet_so_far, new_rec)) {
			hit_anything = true;
			closet_so_far = new_rec.t;
			rec = new_rec;
		}
	}
	return hit_anything;
}

double hitable_list::pdf_value(const vec3& o, const vec3& v) const {
	double weight = 1.0 / size();
	double sum = 0.0;
	for (int i = 0; i < size(); ++i) {
		sum += weight*list[i]->pdf_value(o, v);
	}
	return sum;
}

vec3 hitable_list::random(const vec3& o) const {
	return list[rand() % size()]->random(o);
}

#endif
