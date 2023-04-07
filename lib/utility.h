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

#include <string>
#include <vector>


argparse::ArgumentParser generate_argparser(std::string name, int argc, char** argv) {
    argparse::ArgumentParser program(name);

    program.add_argument("-r", "--image-resolution")
        .nargs(2)
        .default_value(std::vector<int>(960, 540))
        .scan<'i', int>();

    program.add_argument("-s", "--samples-per-pixel")
        .default_value(100)
        .scan<'i', int>();

    program.add_argument("-d", "--max-depth")
        .default_value(50)
        .scan<'i', int>();

    program.add_argument("-o" , "--output-jpg-file")
        .default_value(std::string("output.jpg"));

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

#define GET_ARGS(name, argc, argv) \
    argparse::ArgumentParser argparser = generate_argparser(name, argc, argv); \
    std::vector<int> resolution = argparser.get<std::vector<int>>("--image-resolution"); \
    int image_width = resolution[0]; \
    int image_height = resolution[1]; \
    double aspect_ratio = static_cast<double>(image_width) / image_height; \
    int samples_per_pixel = argparser.get<int>("--samples-per-pixel"); \
    int max_depth = argparser.get<int>("--max-depth"); \
    std::string output_filename = argparser.get<std::string>("--output-jpg-file"); \
    std::cout \
        << "image-width = " << image_width << "\n" \
        << "image-height = " << image_height << "\n" \
        << "sample-per-pixel = " << samples_per_pixel << "\n" \
        << "max-depth = " << max_depth << "\n";


#define PROGRESSBAR() \
    using namespace indicators; \
    ProgressBar pbar{ \
        option::BarWidth{50}, \
        option::Start{"["}, \
        option::Fill{"="}, \
        option::Lead{">"}, \
        option::Remainder{" "}, \
        option::End{"]"}, \
        option::ShowPercentage(true), \
        option::ForegroundColor{Color::cyan}, \
        option::FontStyles{std::vector<FontStyle>{FontStyle::bold}} \
    }; 
    
#endif
