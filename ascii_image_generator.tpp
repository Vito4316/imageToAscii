//
// Created by vitoi on 30/10/2022.
//

#include "ascii_image_generator.hpp"
#include "image_reader.h"

static char map[] = " .:-=+*#%@";
static int gray_size = sizeof(map) -1;

uint8_t pixel_to_gray(const std::array<uint8_t, 3> &pixel) { return (pixel[0] + pixel[1] + pixel[0]) / 3; }

template <int height, int width>
ascii_image_generator<height, width>::ascii_image_generator(image_reader &_image) : image(_image) {
    min = pixel_to_gray(image.get_pixel(0,0));
    max = pixel_to_gray(image.get_pixel(0,0));
    for(int i = 0; i < image.get_height(); i++)
        for(int j = 0; j < image.get_width(); j++) {
            auto val = pixel_to_gray(image.get_pixel(j, i));
            min = val < min ? val : min;
            max = val > max ? val : max;
        }
    diff = max - min;
}

template<int height, int width>
std::array<uint8_t, 3> ascii_image_generator<height, width>::condensate_pixel(int x, int y) {
    int width_coeff = image.get_width() / width;
    int height_coeff = image.get_height() / height;

    uint32_t r, g, b;
    r = g = b = 0;


    //mapping pixels
    for(int i = 0; i < height_coeff; i++) {
        for(int j = 0; j < width_coeff; j++) {
            auto temp = image.get_pixel(x * width_coeff + j, y * height_coeff + i);
            r += temp[0];
            g += temp[1];
            b += temp[2];
        }
    }

    r /= height_coeff * width_coeff;
    g /= height_coeff * width_coeff;
    b /= height_coeff * width_coeff;

    std::array<uint8_t, 3> retval;

    retval[0] = r;
    retval[1] = g;
    retval[2] = b;

    return retval;
}

template<int height, int width>
char ascii_image_generator<height, width>::map_to_ascii(std::array<uint8_t, 3> pixel) {
    uint16_t x = pixel_to_gray(pixel) - min;

    return map[(int)(((double) x / diff) * gray_size)];
}

template<int height, int width>
void ascii_image_generator<height, width>::generate_matrix() {
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            auto pixel = condensate_pixel(x, y);
            char_matrix[y][x] = map_to_ascii(pixel);
        }
    }
}

template<int height, int width>
void ascii_image_generator<height, width>::print_matrix() {
    for(int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            std::cout<<char_matrix[i][j];
        }
        std::cout<<"\n";
    }
}

template<int height, int width>
void ascii_image_generator<height, width>::export_to_file(std::string filename) {
    std::ofstream file;
    file.open(filename);
    for(int i = height-1; i >= 0; i--) {
        for (int j = 0; j < width; j++) {
            file<<char_matrix[i][j];
        }
        file<<"\n";
    }
    file.close();
}
