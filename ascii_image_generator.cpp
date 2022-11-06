//
// Created by vitoi on 30/10/2022.
//

#include "ascii_image_generator.h"
#include "image_reader.h"

//static char map[] = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";
static char map[] = "@%#*+=-:. ";
static int gray_size = sizeof(map)-1;

size_t truncate(size_t value)
{
    if(value < 0) return 0;
    if(value > 255) return 255;

    return value;
}

std::array<uint8_t, 3> apply_contrast(std::array<uint8_t, 3> color) {
    float contrast = 60;
    float factor = (259.0 * (contrast + 255.0)) / (255.0 * (259.0 - contrast));
    color[0] = truncate((uint8_t)(factor * ((float)color[0] - 128) + 128));
    color[1] = truncate((uint8_t)(factor * ((float)color[1] - 128) + 128));
    color[2] = truncate((uint8_t)(factor * ((float)color[2] - 128) + 128));
    return color;
}

uint8_t pixel_to_gray(const std::array<uint8_t, 3> &pixel) { return (pixel[0] + pixel[1] + pixel[0]) / 3; }

ascii_image_generator::ascii_image_generator(image_reader &_image, int _height, int _width) :
                            image(_image), height(_height), width(_width)
    {
    char_matrix = new char*[height];
    for(int i = 0; i < height; i++)
        char_matrix[i] = new char[width];
}

std::array<uint8_t, 3> merge_pixels(std::array<uint8_t, 3> a, std::array<uint8_t, 3> b) {
    return { static_cast<uint8_t> (((int)a[0] + b[0])/2),
             static_cast<uint8_t> (((int)a[1] + b[1])/2),
             static_cast<uint8_t> (((int)a[2] + b[2])/2)};
}

std::array<uint8_t, 3> ascii_image_generator::condensate_pixel(int x, int y) {
    double width_coeff  = image.get_width() / (double) width;
    double height_coeff = image.get_height() / (double) height;

    std::array<uint8_t, 3> pixel {128,128,128};

    //mapping pixels
    for(int i = 0; i < height_coeff; i++) {
        for(int j = 0; j < width_coeff; j++) {
            auto temp = image.get_pixel((int) ((double) x * width_coeff + j),
                                                      (int)((double) y * height_coeff + i));
            pixel = merge_pixels(temp, pixel); //quite not the right method to calculate this
        }
    }


    return pixel;
}

char ascii_image_generator::map_to_ascii(std::array<uint8_t, 3> pixel) {
    uint16_t x = pixel_to_gray(pixel) - min;

    return map[(int)(((double) x / diff) * (double) gray_size)];
}

void ascii_image_generator::generate_matrix() {
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            auto pixel = condensate_pixel(x, y);
            pixel = apply_contrast(pixel);
            char_matrix[y][x] = map_to_ascii(pixel);
        }
    }
}

void ascii_image_generator::print_matrix() {
    for(int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            std::cout<<char_matrix[i][j];
        }
        std::cout<<"\n";
    }
}

void ascii_image_generator::export_to_file(const std::string& filename) {
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

ascii_image_generator::~ascii_image_generator() {
    for(int i = 0; i < height; i++)
        delete char_matrix[i];
    delete char_matrix;
}
