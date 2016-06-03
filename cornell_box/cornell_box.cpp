
#include "stdafx.h"


#include "../lib/camera.hpp"
#include "../lib/vec3.hpp"
#include "../lib/ray.hpp"

#include "../lib/box.hpp"
#include "../lib/ellipsoid.hpp"
#include "../lib/moving_ellipsoid.hpp"
#include "../lib/triangle.hpp"
#include "../lib/quadrilateral.hpp"

//#include "../lib/xy_rect.hpp"
#include "../lib/xz_rect.hpp"
//#include "../lib/yz_rect.hpp"

#include "../lib/hitable.hpp"
#include "../lib/hitable_list.hpp"
#include "../lib/constant_medium.hpp"

#include "../lib/diffuse_light.hpp"
#include "../lib/lambertian.hpp"
#include "../lib/metal.hpp"
#include "../lib/dielectric.hpp"
//#include "../lib/constant_medium.hpp"

#include "../lib/perlin.hpp"
#include "../lib/texture.hpp"
#include "../lib/constant_texture.hpp"
#include "../lib/checker_texture.hpp"
#include "../lib/noise_texture.hpp"
#include "../lib/image_texture.hpp"



#define STB_IMAGE_IMPLEMENTATION
#include "../lib/stb_image.h"

#include "../lib/translate.hpp"
#include "../lib/rotate.hpp"

#include "../lib/pdf.hpp"
#include "../lib/cosine_pdf.hpp"
#include "../lib/hitable_pdf.hpp"
#include "../lib/mixture_pdf.hpp"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <float.h>


#include <windows.h>


#define MAX_DEPTH 10

vec3 colour(const ray& r, hitable& world, hitable& light_shape, int depth = 0) {
	hit_record hrec;
	if (world.hit(r, IOTA, DBL_MAX, hrec)) {
		scatter_record srec;
		vec3 emitted = hrec.mat_ptr->emitted(r, hrec);
		if (depth < MAX_DEPTH && hrec.mat_ptr->scatter(r, hrec, srec)) {
			if (srec.is_specular) {
				return srec.attenuation * colour(srec.specular_ray, world, light_shape, depth + 1);
			}
			else {
				hitable_pdf plight(&light_shape, hrec.point);
				mixture_pdf p(&plight, srec.pdf_ptr, 0.5);
				ray scattered = ray(hrec.point, p.generate(), r.time());
				double pdf_val = p.value(scattered.direction());
				delete srec.pdf_ptr;
				if (pdf_val > IOTA) {
					return emitted + srec.attenuation*hrec.mat_ptr->scattering_pdf(r, hrec, scattered) * colour(scattered, world, light_shape, depth + 1) / pdf_val;
				}
			}
		}
		return emitted;
	}
	return vec3(0.0);
}

std::vector<hitable*> cornell_box() {
	std::vector<hitable*> list;
	material *red = new lambertian(new constant_texture(vec3(0.65, 0.05, 0.05)));
	material *white = new lambertian(new constant_texture(vec3(0.73)));
	material *green = new lambertian(new constant_texture(vec3(0.12, 0.45, 0.15)));
	material *light = new diffuse_light(new constant_texture(vec3(15)));
	vec3 p0(0.0, 555.0, 0.0);
	vec3 p1(0.0, 555.0, 555.0);
	vec3 p2(555.0, 555.0, 555.0);
	vec3 p3(555.0, 555.0, 0.0);
	vec3 p4(0.0, 0.0, 0.0);
	vec3 p5(0.0, 0.0, 555.0);
	vec3 p6(555.0, 0.0, 555.0);
	vec3 p7(555.0, 0.0, 0.0);

	vec3 l0(213.0, 554.0, 227.0);
	vec3 l1(213.0, 554.0, 332.0);
	vec3 l2(343.0, 554.0, 332.0);
	vec3 l3(343.0, 554.0, 227.0);

	list.push_back(new quadrilateral(l3, l2, l1, l0, light));

	list.push_back(new quadrilateral(p2, p3, p7, p6, green));
	list.push_back(new quadrilateral(p0, p1, p5, p4, red));
	list.push_back(new quadrilateral(p3, p2, p1, p0, white));
	list.push_back(new quadrilateral(p4, p5, p6, p7, white));
	list.push_back(new quadrilateral(p1, p2, p6, p5, white));

	return list;
}

std::vector<hitable*> cornell_box_lambertian() {
	std::vector<hitable*> list;
	material *red = new lambertian(new constant_texture(vec3(0.65, 0.05, 0.05)));
	material *white = new lambertian(new constant_texture(vec3(0.73)));
	material *green = new lambertian(new constant_texture(vec3(0.12, 0.45, 0.15)));
	material *light = new diffuse_light(new constant_texture(vec3(15)));
	vec3 p0(0.0, 555.0, 0.0);
	vec3 p1(0.0, 555.0, 555.0);
	vec3 p2(555.0, 555.0, 555.0);
	vec3 p3(555.0, 555.0, 0.0);
	vec3 p4(0.0, 0.0, 0.0);
	vec3 p5(0.0, 0.0, 555.0);
	vec3 p6(555.0, 0.0, 555.0);
	vec3 p7(555.0, 0.0, 0.0);

	vec3 l0(213.0, 554.0, 227.0);
	vec3 l1(213.0, 554.0, 332.0);
	vec3 l2(343.0, 554.0, 332.0);
	vec3 l3(343.0, 554.0, 227.0);

	list.push_back(new quadrilateral(l3, l2, l1, l0, light));

	list.push_back(new quadrilateral(p2, p3, p7, p6, green));
	list.push_back(new quadrilateral(p0, p1, p5, p4, red));
	list.push_back(new quadrilateral(p3, p2, p1, p0, white));
	list.push_back(new quadrilateral(p4, p5, p6, p7, white));
	list.push_back(new quadrilateral(p1, p2, p6, p5, white));

	list.push_back(new translate(new rotate(new box(vec3(0, 0, 0), vec3(165, 165, 165), white), vec3(0, -18, 0)), vec3(130, 0, 65)));
	list.push_back(new translate(new rotate(new box(vec3(0, 0, 0), vec3(165, 330, 165), white), vec3(0, 15, 0)), vec3(265, 0, 295)));

	return list;
}

std::vector<hitable*> cornell_box_metal() {
	std::vector<hitable*> list;
	material *red = new lambertian(new constant_texture(vec3(0.65, 0.05, 0.05)));
	material *white = new lambertian(new constant_texture(vec3(0.73)));
	material *green = new lambertian(new constant_texture(vec3(0.12, 0.45, 0.15)));
	material *light = new diffuse_light(new constant_texture(vec3(15)));
	material * aluminum = new metal(vec3(0.8, 0.85, 0.88), 0.0);

	vec3 p0(0.0, 555.0, 0.0);
	vec3 p1(0.0, 555.0, 555.0);
	vec3 p2(555.0, 555.0, 555.0);
	vec3 p3(555.0, 555.0, 0.0);
	vec3 p4(0.0, 0.0, 0.0);
	vec3 p5(0.0, 0.0, 555.0);
	vec3 p6(555.0, 0.0, 555.0);
	vec3 p7(555.0, 0.0, 0.0);

	vec3 l0(213.0, 554.0, 227.0);
	vec3 l1(213.0, 554.0, 332.0);
	vec3 l2(343.0, 554.0, 332.0);
	vec3 l3(343.0, 554.0, 227.0);

	list.push_back(new quadrilateral(l3, l2, l1, l0, light));

	list.push_back(new quadrilateral(p2, p3, p7, p6, green));
	list.push_back(new quadrilateral(p0, p1, p5, p4, red));
	list.push_back(new quadrilateral(p3, p2, p1, p0, white));
	list.push_back(new quadrilateral(p4, p5, p6, p7, white));
	list.push_back(new quadrilateral(p1, p2, p6, p5, white));

	list.push_back(new translate(new rotate(new box(vec3(0, 0, 0), vec3(165, 165, 165), white), vec3(0, -18, 0)), vec3(130, 0, 65)));
	list.push_back(new translate(new rotate(new box(vec3(0, 0, 0), vec3(165, 330, 165), aluminum), vec3(0, 15, 0)), vec3(265, 0, 295)));

	return list;
}
/*
std::vector<hitable*> cornell_box_smoke() {
std::vector<hitable*> list;
material *red = new lambertian(new constant_texture(vec3(0.65, 0.05, 0.05)));
material *white = new lambertian(new constant_texture(vec3(0.73)));
material *green = new lambertian(new constant_texture(vec3(0.12, 0.45, 0.15)));
material *light = new diffuse_light(new constant_texture(vec3(15)));
material * aluminum = new metal(vec3(0.8, 0.85, 0.88), 0.0);

//material *white_smoke = new constant_medium();

list.push_back(new flip_normal(new yz_rect(0, 555, 0, 555, 555, green)));
list.push_back(new yz_rect(0, 555, 0, 555, 0, red));
list.push_back(new flip_normal(new xz_rect(213, 343, 227, 332, 554, light)));
list.push_back(new flip_normal(new xz_rect(0, 555, 0, 555, 555, white)));
list.push_back(new xz_rect(0, 555, 0, 555, 0, white));
list.push_back(new flip_normal(new xy_rect(0, 555, 0, 555, 555, white)));

hitable* b1 = new translate(new rotate(new box(vec3(0, 0, 0), vec3(165, 165, 165), white), vec3(0, -18, 0)), vec3(130, 0, 65));
hitable *b2 = new translate(new rotate(new box(vec3(0, 0, 0), vec3(165, 330, 165), aluminum), vec3(0, 15, 0)), vec3(265, 0, 295));

list.push_back(new constant_medium(b1, 0.01, new constant_texture(1.0)));
list.push_back(new constant_medium(b2, 0.01, new constant_texture(0.0)));

return list;
}
*/
std::vector<hitable*> cornell_box_glass() {
	std::vector<hitable*> list;
	material *red = new lambertian(new constant_texture(vec3(0.65, 0.05, 0.05)));
	material *white = new lambertian(new constant_texture(vec3(0.73)));
	material *green = new lambertian(new constant_texture(vec3(0.12, 0.45, 0.15)));
	material *light = new diffuse_light(new constant_texture(vec3(25)));

	vec3 p0(0.0, 555.0, 0.0);
	vec3 p1(0.0, 555.0, 555.0);
	vec3 p2(555.0, 555.0, 555.0);
	vec3 p3(555.0, 555.0, 0.0);
	vec3 p4(0.0, 0.0, 0.0);
	vec3 p5(0.0, 0.0, 555.0);
	vec3 p6(555.0, 0.0, 555.0);
	vec3 p7(555.0, 0.0, 0.0);

	vec3 l0(213.0, 554.0, 227.0);
	vec3 l1(213.0, 554.0, 332.0);
	vec3 l2(343.0, 554.0, 332.0);
	vec3 l3(343.0, 554.0, 227.0);

	list.push_back(new quadrilateral(l3, l2, l1, l0, light));

	list.push_back(new quadrilateral(p2, p3, p7, p6, green));
	list.push_back(new quadrilateral(p0, p1, p5, p4, red));
	list.push_back(new quadrilateral(p3, p2, p1, p0, white));
	list.push_back(new quadrilateral(p4, p5, p6, p7, white));
	list.push_back(new quadrilateral(p1, p2, p6, p5, white));

	list.push_back(new translate(new rotate(new box(vec3(0, 0, 0), vec3(165, 330, 165), white), vec3(0, 15, 0)), vec3(265, 0, 295)));
	list.push_back(new ellipsoid(vec3(190, 90, 190), vec3(90), new dielectric(2.25)));
	return list;
}

void draw_progress_bar(int len, double percent) {
	std::cout << "\r";
	std::string progress;
	for (int i = 0; i < len; ++i) {
		if (i < static_cast<int>(len * percent)) {
			progress += "=";
		}
		else {
			progress += " ";
		}
	}
	std::cout << "[" << progress << "] " << (static_cast<int>(100.0 * percent)) << "%";
	std::flush(std::cout);
}

int main() {
	int nx = 600;
	int ny = 600;

	// horizontal sampling and vertical sampling
	int sampling_size = 32;
	double s = double(sampling_size);
	std::ofstream out("cornell_box_glass.ppm");
	out << "P3\n" << nx << " " << ny << "\n255\n";

	hitable_list world(cornell_box_glass());

	vec3 lookfrom(278.0, 278.0, -800.0);
	vec3 lookat(278.0, 278.0, 0.0);

	camera cam(lookfrom, lookat, vec3(0.0, 1.0, 0.0), 40.0, double(nx) / double(ny), 0.0, 10, 0.0, 1.0);

	vec3 l0(213.0, 554.0, 227.0);
	vec3 l1(213.0, 554.0, 332.0);
	vec3 l2(343.0, 554.0, 332.0);
	vec3 l3(343.0, 554.0, 227.0);

	hitable_list lights;
	//lights.push(new quadrilateral(l3, l2, l1, l0, nullptr));
	lights.push(new xz_rect(213, 343, 227, 332, 554, nullptr));

	for (int j = ny - 1; j >= 0; --j) {
		for (int i = 0; i < nx; ++i) {

			vec3 col(0.0);

			for (int si = 0; si < sampling_size; ++si) {
				for (int sj = 0; sj < sampling_size; ++sj) {
#ifdef RANDOM_ALG
					double u = (double(i) + double(drand48())) / double(nx);
					double v = (double(j) + double(drand48())) / double(ny);
#else
					double u = (double(i) + double(si) / s) / double(nx);
					double v = (double(j) + double(sj) / s) / double(ny);
#endif
					ray r = cam.get_ray(u, v);

					vec3 c = colour(r, world, lights);
					col += clamp(c, 0.0, 15.0);
				}
			}

			col /= s*s;
			col = gamma_correction(col);
			col = clamp(col, 0.0, 1.0);

			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);

			out << ir << " " << ig << " " << ib << "\n";
		}
		draw_progress_bar(40, double(ny - j) / double(ny));
	}
	out.close();

	return 0;
}

