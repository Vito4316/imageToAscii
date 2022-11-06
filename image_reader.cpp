//
// Created by vitoi on 30/10/2022.
//

#include "image_reader.h"

image_reader::image_reader(const std::string& path) {
    file_handle.open(path, std::ifstream::binary);
    if(!file_handle.is_open()) {
        std::cerr << "error! file could not be found\n";
        isOpen = false;
        return;
    }
    isOpen = true;
    file_handle.read(header.header_string, HEADER_SIZE);

    memcpy(header.header_string + 2, header.header_string, HEADER_SIZE); /*moving 2 bytes ahead because of padding
                                                                    * char type[4]; <- two bytes of this are padding
                                                                    * uint32_t size;
                                                                    * may be unsafe */

    file_handle.read(dib.dib_string, DIB_SIZE);

    file_handle.seekg(header.offset, file_handle.beg);
    for(int i = 0; i < dib.height; i++) {
        int j;
        for (j = 0; j < dib.width; j++) {
            std::array<uint8_t, 3> pixel{};
            file_handle.read((char *) pixel.data(), dib.bits_per_pixel / 8);
            std::swap(pixel[0], pixel[2]); //swapping because of little endian
            pixels.push_back(pixel);
        }
        if(dib.width*3 % 4 == 0) file_handle.seekg(header.offset + (i+1) * (dib.width*3));
        else file_handle.seekg(header.offset + (i+1) * ((dib.width*3) + 4 - (dib.width*3) % 4)); //skipping padding
    }
}

uint32_t image_reader::get_size() const {
    if(!isOpen) return 0;
    return header.size;
}

[[maybe_unused]] std::string image_reader::get_type() const {
    if(!isOpen) return "";
    std::string retval = "";
    retval += header.type[0];
    retval += header.type[1];
    return retval;
}

uint32_t image_reader::get_width() const {
    if(!isOpen) return 0;
    return dib.width;
}

uint32_t image_reader::get_height() const {
    if(!isOpen) return 0;
    return dib.height;
}

bool image_reader::inBounds(int x, int y) {
    return x > 0 && x < dib.width && y > 0 && y < dib.height;
}

/*
 * really slow function!
 * continuous IO operations!
 */ /*
std::array<uint8_t , 3> image_reader::get_pixel(int x, int y) {
    std::array<uint8_t, 3> retval{0,0,0};

    if(!inBounds(x, y)) return retval;

    //move to corresponding bytes
    file_handle.seekg(header.offset + (x + (dib.width * y))*dib.bits_per_pixel/8, file_handle.beg);
    file_handle.read((char*) retval.data(), dib.bits_per_pixel / 8);

    std::swap(retval[0], retval[2]); //swapping because of little endian

    return retval;
} */



image_reader::~image_reader() {
    file_handle.close();
}

std::array<uint8_t, 3> image_reader::get_pixel(int x, int y) {
    if(!inBounds(x, y)) return {};
    return pixels[x + (dib.width * y)];
}
