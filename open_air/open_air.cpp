#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <cstdlib>

#include "../lib/camera.hpp"
#include "../lib/vec3.hpp"
#include "../lib/ray.hpp"

#include "../lib/ellipsoid.hpp"
#include "../lib/moving_ellipsoid.hpp"

#include "../lib/hitable.hpp"
#include "../lib/hitable_list.hpp"
#include "../lib/triangle.hpp"

#include "../lib/lambertian.hpp"
#include "../lib/metal.hpp"
#include "../lib/dielectric.hpp"


#define STB_IMAGE_IMPLEMENTATION
#include "../lib/stb_image.h"

#include "../lib/texture.hpp"
#include "../lib/constant_texture.hpp"
#include "../lib/checker_texture.hpp"
#include "../lib/image_texture.hpp"
#include "../lib/noise_texture.hpp"

#define MAX_DEPTH 5
//#define MOVING_SPHERE
//#define RANDOM_ALG

vec3 colour(const ray& r, hitable& world, int depth) {
	hit_record hrec;
	if (world.hit(r, IOTA, DBL_MAX, hrec)) {
		scatter_record srec;
		vec3 emitted = hrec.mat_ptr->emitted(r, hrec);

		if (depth < MAX_DEPTH && hrec.mat_ptr->scatter(r, hrec, srec)) {
			if (srec.is_specular) {
				return srec.attenuation * colour(srec.specular_ray, world, depth + 1);
			}
			//ray scattered(hrec.point, random_cosine_direction(), r.time());
			ray scattered(hrec.point, srec.pdf_ptr->generate(), r.time());
			delete srec.pdf_ptr;
			return srec.attenuation * colour(scattered, world, depth + 1);
		}
		return emitted;
	}
	return mix(vec3(1.0), vec3(0.5, 0.7, 1.0), 0.5*(r.direction().y() + 1.0));
}

std::vector<hitable*> test_triangle() {
	std::vector<hitable*> list;
	list.push_back(new triangle(vec3(0.0), vec3(1.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0), new lambertian(new constant_texture(vec3(0.1, 0.2, 0.5)))));
	list.push_back(new ellipsoid(vec3(0.0, -1000.0, 0.0), vec3(1000.0), new lambertian(new constant_texture(vec3(0.8, 0.8, 0.0)))));
	return list;
}

std::vector<hitable*> test_lambertian_metal_dielectric() {
	std::vector<hitable*> list;
	list.push_back(new ellipsoid(vec3(0.0, 1.0, 0.0), vec3(1.0), new lambertian(new constant_texture(vec3(0.1, 0.2, 0.5)))));
	list.push_back(new ellipsoid(vec3(0.0, -1000.0, 0.0), vec3(1000.0), new lambertian(new constant_texture(vec3(0.8, 0.8, 0.0)))));
	list.push_back(new ellipsoid(vec3(4.1, 1.0, -1.0), vec3(1.0), new metal(vec3(0.8, 0.6, 0.2), 0.3)));
	list.push_back(new ellipsoid(vec3(-4.1, 1.0, 1.0), vec3(1.0), new dielectric(1.5)));
	return list;
}


std::vector<hitable*> rand_world(int k) {
	std::vector<hitable*> list;
	// ground
	list.push_back(new ellipsoid(vec3(0.0, -1000.0, 0.0), vec3(1000.0), new lambertian(new constant_texture(vec3(0.5)))));

	for (int i = -k; i < k; ++i) {
		for (int j = -k; j < k; ++j) {
			double choose_mat = drand48();
			vec3 centre(i + 0.9*drand48(), 0.2, j + 0.9*drand48());
			if (choose_mat < 0.8) { //diffuse
				vec3 l(drand48()*drand48(), drand48()*drand48(), drand48()*drand48());
#ifndef MOVING_SPHERE
				list.push_back(new ellipsoid(centre, vec3(0.2), new lambertian(new constant_texture(l))));
#else
				list.push_back(new moving_ellipsoid(centre, centre + vec3(0.0, 0.5*drand48(), 0.0), 0.0, 1.0, vec3(0.2), new lambertian(new constant_texture(l))));
#endif // !MOVING_SPHERE


			}
			else if (choose_mat < 0.9) { // metal
				vec3 m(0.5*(1.0*drand48()), 0.5*(1.0*drand48()), 0.5*(1.0*drand48()));
				list.push_back(new ellipsoid(centre, vec3(0.2), new metal(m, 0.5*drand48())));
			}
			else { // glass
				list.push_back(new ellipsoid(centre, vec3(0.2), new dielectric(1.5)));
			}
		}
	}

	list.push_back(new ellipsoid(vec3(0.0, 1.0, 0.0), vec3(1.0), new dielectric(1.5)));
	list.push_back(new ellipsoid(vec3(-4.0, 1.0, 0.0), vec3(1.0), new lambertian(new constant_texture(vec3(0.1, 0.2, 0.5)))));
	list.push_back(new ellipsoid(vec3(4.0, 1.0, 0.0), vec3(1.0), new metal(vec3(0.7, 0.6, 0.5), 0.0)));
	return list;
}

std::vector<hitable*> two_sphere() {
	std::vector<hitable*> list;
	texture *checker = new checker_texture(new constant_texture(vec3(0.2, 0.3, 0.3)), new constant_texture(vec3(0.8, 0.8, 0.2)));
	list.push_back(new ellipsoid(vec3(0.0, -100.0, 0.0), vec3(100.0), new lambertian(checker)));
	list.push_back(new ellipsoid(vec3(0.0, 2.0, 0.0), vec3(2.0), new lambertian(checker)));
	return list;
}

std::vector<hitable*> test_perlin_texture_mapping() {
	std::vector<hitable*> list;
	int nx, ny, nn;
	unsigned char* tex_data = stbi_load("1.jpg", &nx, &ny, &nn, 0);
	material *mat1 = new lambertian(new image_texture(tex_data, nx, ny));
	texture *pertext = new noise_texture(4.0);
	material *mat2 = new lambertian(pertext);
	list.push_back(new ellipsoid(vec3(0.0, -100, 0.0), vec3(100), mat2));
	list.push_back(new ellipsoid(vec3(0.0, 1.0, 0.0), vec3(1.0), mat1));
	return list;
}

int main() {
	int nx = 800;
	int ny = 600;

	// horizontal sampling and vertical sampling
	int sampling_size = 32;
	double s = double(sampling_size);
	std::ofstream out("test_perlin_texture_mapping.ppm");
	out << "P3\n" << nx << " " << ny << "\n255\n";

	hitable_list world(test_perlin_texture_mapping());

	vec3 lookfrom = vec3(13.0, 2.0, 3.0);
	vec3 lookat = vec3(0.0, 1.0, 0.0);
	camera cam(lookfrom, lookat, vec3(0.0, 1.0, 0.0), 20.0, double(nx) / double(ny), 0.0, 10.0, 0.0, 1.0);

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
					col += clamp(colour(r, world, 0), 0.0, 1.0);
				}
			}

			col /= s*s;
			col = clamp(col, 0.0, 1.0);
			int ir = int(255.99*sqrt(col[0]));
			int ig = int(255.99*sqrt(col[1]));
			int ib = int(255.99*sqrt(col[2]));
			out << ir << " " << ig << " " << ib << "\n";
		}
	}
	out.close();
	std::cout << "Press any key to continue.\n";
	//std::cin.get();
	return 0;
}
