//
// Created by vitoi on 30/10/2022.
//

#include "ascii_image_generator.hpp"
#include "image_reader.h"

template <int height, int width>
ascii_image_generator<height, width>::ascii_image_generator(image_reader &_image) : image(_image) {
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

char map_to_ascii(std::array<uint8_t, 3> pixel) {
    uint16_t x = (pixel[0] + pixel[1] + pixel[0])/3;

    return ((double) x / 255) * 25 + 'a';
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
