#include <iostream>
#include "image_reader.h"
#include "ascii_image_generator.hpp"

int main() {
    image_reader image("input.bmp");
    ascii_image_generator<100, 100> generator(image);

    generator.generate_matrix();
    generator.export_to_file("output.txt");


    return 0;
}
