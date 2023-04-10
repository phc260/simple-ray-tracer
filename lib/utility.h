#ifndef UTILITY_H
#define UTILITY_H
//==============================================================================================
// Originally written in 2023 by Alan P.H. Chiu <phc260@nyu.edu>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related and
// neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy (see file COPYING.txt) of the CC0 Public Domain Dedication
// along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
//==============================================================================================

#include <argparse/argparse.hpp>
#include <indicators/progress_bar.hpp>
#include <indicators/cursor_control.hpp>
#include "../thread-pool/BS_thread_pool.hpp"

#include <string>
#include <tuple>
#include <vector>


argparse::ArgumentParser generate_argparser(std::string name, int argc, char** argv) {
    argparse::ArgumentParser program(name);

    program.add_argument("-H", "--image-height")
        .default_value(540)
        .scan<'i', int>();

    program.add_argument("-s", "--samples-per-pixel")
        .default_value(100)
        .scan<'i', int>();

    program.add_argument("-d", "--max-depth")
        .default_value(50)
        .scan<'i', int>();

    program.add_argument("-o" , "--output-jpg")
        .default_value(std::string(name + ".jpg"));

    try {
        program.parse_args(argc, argv);
    }
    catch (const std::runtime_error& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        std::exit(1);
    }
    return program;
}


void signal_callback_handler(int signum) {
    indicators::show_console_cursor(true);
}


template <typename... T>
uint8_t* render(
        int image_width,
        int image_height,
        int samples_per_pixel,
        int max_depth,
        camera& cam,
        hittable_list& world,
        color (*func)(const ray& r, const hittable&, int, const std::tuple<T...>&),
        std::tuple<T...>& args) {
    using namespace indicators;
    ProgressBar pbar{
        option::BarWidth{50},
        option::Start{"["},
        option::Fill{"="},
        option::Lead{">"},
        option::Remainder{" "},
        option::End{"]"},
        option::ShowPercentage(true),
        option::ForegroundColor{Color::cyan},
        option::FontStyles{std::vector<FontStyle>{FontStyle::bold}}
    }; 
    show_console_cursor(false);
 
    BS::thread_pool pool;
    uint8_t* image = new uint8_t[image_width*image_height*3];
    uint64_t index = 0;
    for (int j = image_height-1; j >= 0; --j) {
        std::vector<std::future<color>> futures;

        for (int i = 0; i < image_width; ++i) {
            std::future<color> my_future = pool.submit(
                [cam, world, i, j, image_width, image_height,
                 samples_per_pixel, max_depth, func, args] {
                    color pixel_color(0,0,0);
                    for (int s = 0; s < samples_per_pixel; ++s) {
                        auto u = (i + random_double()) / (image_width-1);
                        auto v = (j + random_double()) / (image_height-1);
                        ray r = cam.get_ray(u, v);
                        pixel_color += func(r, world, max_depth, args);
                    }
                    return pixel_color;
                });
            futures.push_back(std::move(my_future));
        }
        for (auto& _future : futures) {
            color pix = _future.get();
            auto r = pix.x(); 
            auto g = pix.y();
            auto b = pix.z();

            if (r != r) r = 0.0;
            if (g != g) g = 0.0;
            if (b != b) b = 0.0;

            auto scale = 1.0 / samples_per_pixel;
            r = sqrt(scale * r);
            g = sqrt(scale * g);
            b = sqrt(scale * b);

            image[index++] = static_cast<uint8_t>(255 * clamp(r, 0.0, 1.0));
            image[index++] = static_cast<uint8_t>(255 * clamp(g, 0.0, 1.0));
            image[index++] = static_cast<uint8_t>(255 * clamp(b, 0.0, 1.0));
        }

        auto progress = 100*(image_height-j)/image_height;
        pbar.set_progress(progress);
    }

    show_console_cursor(true);
    return image;
}

#endif
