cmake_minimum_required(VERSION 3.22)
project(ImageInAscii)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(ImageInAscii main.cpp image_reader.cpp image_reader.h ascii_image_generator.cpp ascii_image_generator.h)
