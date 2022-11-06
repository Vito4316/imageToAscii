#include <iostream>
#include <cstring>
#include "image_reader.h"
#include "ascii_image_generator.h"

int main(int argc, char** argv) {

    int height = 100;
    int width = 100;
    std::string input = "input.bmp";
    std::string output = "output.txt";

    for(int i = 1; i < argc; i++) {
        std::cout<<argv[i]<<std::endl;
        if(memcmp("-o=", argv[i], 3) == 0) output = std::string(argv[i] + 3);
        if(memcmp("-i=", argv[i], 3) == 0) input = std::string(argv[i] + 3);
        if(memcmp("-h=", argv[i], 3) == 0) height = atoi(argv[i] + 3);
        if(memcmp("-w=", argv[i], 3) == 0) width = atoi(argv[i] + 3);
    }

    image_reader image(input);
    ascii_image_generator generator(image, height, width);

    generator.generate_matrix();
    generator.print_matrix();
    generator.export_to_file(output);

    return 0;
}
