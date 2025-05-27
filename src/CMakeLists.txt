cmake_minimum_required(VERSION 3.29)
project(png_processing C)

set(CMAKE_C_STANDARD 11)

find_package(PNG REQUIRED)

add_executable(png_processing png.c
        main.c
        png_read.h
        png_read.c
        png_errors.h
        png_structs.h
        png_write.c
        png_write.h
        png_functions.c
        png_functions.h)

target_link_libraries(png_processing PNG::PNG)