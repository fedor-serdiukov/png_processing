#ifndef PNG_STRUCTS_H
#define PNG_STRUCTS_H

#include <png.h>
#include <stdbool.h>

typedef struct PNG{
    png_structp png_ptr;
    png_infop info_ptr;
    png_bytep* row_pointers;
    int width;
    int height;
    png_byte bit_depth;
    png_byte color_type;
    png_byte filter_method;
    png_byte interlace_method;
} PNG;

typedef struct RGB{
    int r, g, b;
} RGB;

typedef enum {
    COMPONENT_RED,
    COMPONENT_GREEN,
    COMPONENT_BLUE
} Component;

typedef struct Coordinates{
    int x_left;
    int y_left;
    int x_right;
    int y_right;
} Coordinates;

typedef struct Parameters{
    char* input_file_name;
    char* output_file_name;
    bool info_flag;
    bool squared_lines_flag;
    bool fill_flag;
    bool rotate_flag;
    bool rgbfilter_flag;
    bool outside_rect;
    bool paving_flag;
    bool expand_flag;
    bool square_rhombus_flag;
    Component component_name;
    int component_value;
    int thickness;
    int side_size;
    int angle;
    int width_scale;
    int upper_vertex_x;
    int upper_vertex_y;
    int size;
    Coordinates Point;
    RGB Color;
    RGB Color_Fill;
} Parameters;

#endif
