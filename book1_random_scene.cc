//==============================================================================================
// Originally written in 2016 by Peter Shirley <ptrshrl@gmail.com>
// Modified by Alan P.H. Chiu <phc260@nyu.edu>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related and
// neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy (see file COPYING.txt) of the CC0 Public Domain Dedication
// along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
//==============================================================================================

#include "lib/rtweekend.h"
#include "lib/camera.h"
#include "lib/hittable_list.h"
#include "lib/material.h"
#include "lib/sphere.h"
#include "lib/utility.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include <iostream>
#include <signal.h>
#include <tuple>


color ray_color(const ray& r, const hittable& world, int depth) {
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return color(0, 0, 0);

    if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth-1);
        return color(0, 0, 0);
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}


template <typename... T>
color wrapper(const ray& r, const hittable& world, int depth, const std::tuple<T...>& args) {
    auto func = std::get<0>(args);
    return func(r, world, depth);
}


hittable_list random_scene() {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}


int main(int argc, char** argv) {
    signal(SIGINT, signal_callback_handler);

    RaytracerArgumentParser argparser("random_scene");
    argparser.try_parse_args(argc, argv);

    const double aspect_ratio = 16.0 / 9.0;
    int image_width = argparser.get<int>("--image-width");
    int image_height= static_cast<int>(image_width / aspect_ratio);
    int samples_per_pixel = argparser.get<int>("--samples-per-pixel");
    int max_depth = argparser.get<int>("--max-depth");
    std::string output_jpg = argparser.get<std::string>("--output-jpg");
    std::cout
        << "image-width = " << image_width << "\n"
        << "image-height = " << image_height << "\n"
        << "sample-per-pixel = " << samples_per_pixel << "\n"
        << "max-depth = " << max_depth << "\n"
        << "output-jpg = " << output_jpg << "\n"; 

    // World
    auto world = random_scene();

    // Camera
    point3 lookfrom(13, 2, 3);
    point3 lookat(0, 0, 0);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.01;
    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

    // Render
    auto args = std::make_tuple(ray_color);
    uint8_t *image = render(image_width, image_height, samples_per_pixel, max_depth, cam, world, wrapper, args);

    // Write image
    stbi_write_jpg(output_jpg.c_str(), image_width, image_height, 3, image, 100);
    delete[] image;
}
