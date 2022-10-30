//
// Created by vitoi on 30/10/2022.
//

#ifndef IMAGEINASCII_IMAGE_READER_H
#define IMAGEINASCII_IMAGE_READER_H
#include <iostream>
#include <fstream>
#include <array>

#define HEADER_SIZE 14
#define DIB_SIZE 40

class image_reader {
    bool isOpen = false;
    union{
        struct {
            char type[4];
            uint32_t size;
            uint32_t unused;
            uint32_t offset;
        };
        char header_string[HEADER_SIZE];
    } header;

    union{
        struct {
            uint32_t size;
            uint32_t width;
            uint32_t height;
            uint16_t color_depth;
            uint16_t bits_per_pixel;
        };
        char dib_string[DIB_SIZE];
    }dib;

    std::ifstream file_handle;
public:

    std::string get_type() const;
    uint32_t get_size() const;
    uint32_t get_width() const;
    uint32_t get_height() const;
    std::array<uint8_t, 3> get_pixel(int x, int y) const;

public:

    image_reader(const std::string& path);
    ~image_reader();
};

#endif //IMAGEINASCII_IMAGE_READER_H