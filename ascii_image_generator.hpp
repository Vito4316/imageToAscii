//
// Created by vitoi on 30/10/2022.
//

#ifndef IMAGEINASCII_ASCII_IMAGE_GENERATOR_HPP
#define IMAGEINASCII_ASCII_IMAGE_GENERATOR_HPP
#include <cstdint>
#include <array>
#include "image_reader.h"

template <int height, int width>
class ascii_image_generator {
    image_reader& image;
    char char_matrix[height][width];

private:
    std::array<uint8_t, 3> condensate_pixel(int x, int y);

public:
    void generate_matrix();
    void print_matrix();

public:
    ascii_image_generator(image_reader &_image);

};

#include "ascii_image_generator.tpp"

#endif //IMAGEINASCII_ASCII_IMAGE_GENERATOR_HPP
