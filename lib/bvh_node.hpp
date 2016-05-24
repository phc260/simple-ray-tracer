#ifndef BVH_NODE_H
#define BVH_NODE_H

#include <math.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include "aabb.hpp"
#include "hitable.hpp"

int box_x_compare(const void *a, const void *b) {
	aabb box_left, box_right;
	hitable *ah = *(hitable**)a;
	hitable *bh = *(hitable**)b;
	if (!ah->bounding_box(0.0, 0.0, box_left) || !bh->bounding_box(0.0, 0.0, box_right))
		std::cerr << "no bounding box in bvh_node constructor\n";
	return box_left.min.x() < box_right.min.x() ? -1 : 1;
}

int box_y_compare(const void *a, const void *b) {
	aabb box_left, box_right;
	hitable *ah = *(hitable**)a;
	hitable *bh = *(hitable**)b;
	if (!ah->bounding_box(0.0, 0.0, box_left) || !bh->bounding_box(0.0, 0.0, box_right))
		std::cerr << "no bounding box in bvh_node constructor\n";
	return box_left.min.y() < box_right.min.y() ? -1 : 1;
}

int box_z_compare(const void *a, const void *b) {
	aabb box_left, box_right;
	hitable *ah = *(hitable**)a;
	hitable *bh = *(hitable**)b;
	if (!ah->bounding_box(0.0, 0.0, box_left) || !bh->bounding_box(0.0, 0.0, box_right))
		std::cerr << "no bounding box in bvh_node constructor\n";
	return box_left.min.z() < box_right.min.z() ? -1 : 1;
}

class bvh_node : public hitable {
public:
	bvh_node() {}
	bvh_node(std::vector<hitable*> &l, int b, int e, double time0, double time1);
	
	virtual bool hit(const ray &r, double tmin, double tmax, hit_record& rec) const;
	virtual bool bounding_box(double t0, double t1, aabb& box) const;

private:
	hitable *left;
	hitable *right;
	aabb box;
};

bvh_node::bvh_node(std::vector<hitable*> &l, int b, int e, double time0, double time1) {
	int n = l.size();
	int axis = rand() % 3;
	if (axis == 0)
		std::sort(l.begin(), l.end(), box_x_compare);
	else if (axis == 1)
		std::sort(l.begin(), l.end(), box_y_compare);
	else
		std::sort(l.begin(), l.end(), box_z_compare);
	if (n == 1) {
		left = right = l[0];
	}
	else if (n == 2) {
		left = l[0]; right = l[1];
	}
	else {
		int i = (b + e) / 2;
		left = new bvh_node(l, b, i, time0, time1);
		right = new bvh_node(l, i, e, time0, time1);
	}

	aabb box_left, box_right;
	if (!left->bounding_box(time0, time1, box_left) || right->bounding_box(time0, time1, box_right))
		std::cerr << "no bounding box in bvh_node constructor\n";
	box = surrounding_box(box_left, box_right);
}

bool bvh_node::hit(const ray &r, double tmin, double tmax, hit_record& rec) const {
	if (box.hit(r, tmin, tmax)) {
		hit_record left_rec, right_rec;
		bool hit_left = left->hit(r, tmin, tmax, left_rec);
		bool hit_right = right->hit(r, tmin, tmax, right_rec);

		if (hit_left && hit_right) {
			rec = left_rec.t < right_rec.t ? left_rec : right_rec;
			return true;
		}
		if (hit_left) {
			rec = left_rec;
			return true;
		}
		if (hit_right) {
			rec = right_rec;
			return true;
		}
	}
	return false;
}

bool bvh_node::bounding_box(double t0, double t1, aabb& b) const {
	b = box;
	return true;
}

#endif
