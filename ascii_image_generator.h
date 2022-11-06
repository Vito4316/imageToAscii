//
// Created by vitoi on 30/10/2022.
//

#ifndef IMAGEINASCII_ASCII_IMAGE_GENERATOR_H
#define IMAGEINASCII_ASCII_IMAGE_GENERATOR_H
#include <cstdint>
#include <array>
#include "image_reader.h"

class ascii_image_generator {
    image_reader& image;
    int height, width;
    char** char_matrix;
    uint8_t min = 0, max, diff = 255;

private:
    std::array<uint8_t, 3> condensate_pixel(int x, int y);
public:
    void generate_matrix();
    void print_matrix();
    char map_to_ascii(std::array<uint8_t, 3> pixel);
    void export_to_file(const std::string& filename);
public:
    ascii_image_generator(image_reader &_image, int _height, int _width);
    virtual ~ascii_image_generator();
};

#endif //IMAGEINASCII_ASCII_IMAGE_GENERATOR_H
