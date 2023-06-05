//==============================================================================================
// Originally written in 2016 by Peter Shirley <ptrshrl@gmail.com>
// Modified in 2023 by Alan P.H. Chiu <phc260@nyu.edu>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related and
// neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy (see file COPYING.txt) of the CC0 Public Domain Dedication
// along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
//==============================================================================================

#include "lib/rtweekend.h"

#include "lib/aarect.h"
#include "lib/box.h"
#include "lib/camera.h"
#include "lib/color.h"
#include "lib/hittable_list.h"
#include "lib/material.h"
#include "lib/sphere.h"
#include "lib/utility.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include <iostream>
#include <signal.h>
#include <tuple>


color ray_color(
    const ray& r,
    const color& background,
    const hittable& world,
    shared_ptr<hittable> lights,
    int depth
) {
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return color(0,0,0);

    // If the ray hits nothing, return the background color.
    if (!world.hit(r, 0.001, infinity, rec))
        return background;

    scatter_record srec;
    color emitted = rec.mat_ptr->emitted(r, rec, rec.u, rec.v, rec.p);

    if (!rec.mat_ptr->scatter(r, rec, srec))
        return emitted;

    if (srec.is_specular) {
        return srec.attenuation
             * ray_color(srec.specular_ray, background, world, lights, depth-1);
    }

    auto light_ptr = make_shared<hittable_pdf>(lights, rec.p);
    mixture_pdf p(light_ptr, srec.pdf_ptr);
    ray scattered = ray(rec.p, p.generate(), r.time());
    auto pdf_val = p.value(scattered.direction());

    return emitted
         + srec.attenuation * rec.mat_ptr->scattering_pdf(r, rec, scattered)
                            * ray_color(scattered, background, world, lights, depth-1)
                            / pdf_val;
}


template <typename... T>
color wrapper(const ray& r, const hittable& world, int depth, const std::tuple<T...>& args) {
    auto func = std::get<0>(args);
    auto lights = std::get<1>(args);
    color background = std::get<2>(args);
    return func(r, background, world, lights, depth);
}


hittable_list cornell_box() {
    hittable_list objects;

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<flip_face>(make_shared<xz_rect>(213, 343, 227, 332, 554, light)));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    shared_ptr<material> aluminum = make_shared<metal>(color(0.8, 0.85, 0.88), 0.0);
    shared_ptr<hittable> box1 = make_shared<box>(point3(0,0,0), point3(165,330,165), aluminum);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265,0,295));
    objects.add(box1);

    auto glass = make_shared<dielectric>(1.5);
    objects.add(make_shared<sphere>(point3(190,90,190), 90 , glass));

    return objects;
}


int main(int argc, char** argv) {
    signal(SIGINT, signal_callback_handler);

    argparse::ArgumentParser argparser =
        generate_argparser("cornell_box", argc, argv);
    const double aspect_ratio = 1.0;
    int image_width = argparser.get<int>("--image-width");
    int image_height = image_width;
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
    auto lights = make_shared<hittable_list>();
    lights->add(make_shared<xz_rect>(213, 343, 227, 332, 554, shared_ptr<material>()));
    lights->add(make_shared<sphere>(point3(190, 90, 190), 90, shared_ptr<material>()));

    auto world = cornell_box();

    color background(0,0,0);

    // Camera
    point3 lookfrom(278, 278, -800);
    point3 lookat(278, 278, 0);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.0;
    auto vfov = 40.0;
    auto time0 = 0.0;
    auto time1 = 1.0;

    camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, time0, time1);

    // Render
    auto args = std::make_tuple(ray_color, lights, background);
    uint8_t *image = render(image_width, image_height, samples_per_pixel, max_depth, cam, world, wrapper, args);

    stbi_write_jpg(output_jpg.c_str(), image_width, image_height, 3, image, 100);
    delete[] image;
}
