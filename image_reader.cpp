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
}

uint32_t image_reader::get_size() const {
    if(!isOpen) return 0;
    return header.size;
}

[[maybe_unused]] std::string image_reader::get_type() const {
    if(!isOpen) return "";
    std::string retval = "" ;
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

std::array<uint8_t , 3> image_reader::get_pixel(int x, int y) {
    char temp[3];
    std::array<uint8_t, 3> retval{};

    //move to corresponding bytes
    file_handle.seekg(header.offset + (x + (dib.width * y))*dib.bits_per_pixel/8, file_handle.beg);
    file_handle.read(temp, dib.bits_per_pixel/8);

    retval[2] = temp[0]; //bytes are in little endian
    retval[1] = temp[1];
    retval[0] = temp[2];

    return retval;
}

image_reader::~image_reader() {
    file_handle.close();
}