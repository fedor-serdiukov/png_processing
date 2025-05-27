#include "png_functions.h"
#include "png_errors.h"
#include <png.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>


void draw_rhombus(PNG* image, Parameters* param) {
    int diagonal = 0;
    int size = param->size;
    diagonal = (int)(sqrt(size*size + size*size));
    int rx = param->upper_vertex_x;
    int ry = param->upper_vertex_y + diagonal / 2;
    int r = param->Color.r;
    int g = param->Color.g;
    int b = param->Color.b;
    while (diagonal >= 0){
        for (int y = 0; y < image->height; y++){

            for (int x = 0; x < image->width; x++){
                if ((abs(x - rx) + abs(y - ry)) == diagonal/2){
                    if (x >= 0 && x < image->width && y >= 0 && y < image->height){
                        png_bytep row = image->row_pointers[y];
                        png_bytep ptr = &(row[x * 3]);
                        ptr[0] = r;
                        ptr[1] = g;
                        ptr[2] = b;
                    }
                }
            }
        }
        diagonal--;
    }
}

void expand(PNG* image, Parameters* param) {
    int new_width = param->Point.x_right;
    int new_height = param->Point.y_right;


    int shift_x = (new_width - image->width) / 2;
    int shift_y = (new_height - image->height) / 2;

    png_bytep* new_rows = malloc(new_height * sizeof(png_bytep));

    for (int y = 0; y < new_height; y++) {
        new_rows[y] = malloc(new_width * 3);
        for (int x = 0; x < new_width; x++) {
            new_rows[y][x * 3 + 0] = param->Color.r;
            new_rows[y][x * 3 + 1] = param->Color.g;
            new_rows[y][x * 3 + 2] = param->Color.b;
        }
    }
    for (int y = 0; y < image->height; y++) {
        memcpy(
            &new_rows[y + shift_y][shift_x * 3],
            image->row_pointers[y],
            image->width * 3
        );
    }

    for (int y = 0; y < image->height; y++) {
        free(image->row_pointers[y]);
    } free(image->row_pointers);
    image->row_pointers = new_rows;
    image->width = new_width;
    image->height = new_height;
}

void outside_rect(PNG* image, Parameters* param) {
    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x++) {
            if (!(x >= param->Point.x_left && x < param->Point.x_right && y >= param->Point.y_left && y < param->Point.y_right)) {
                png_bytep row = image->row_pointers[y];
                png_bytep ptr = &(row[x * 3]);
                ptr[0] = param->Color.r;
                ptr[1] = param->Color.g;
                ptr[2] = param->Color.b;
            }
        }
    }
}

void paving(PNG* image, Parameters* param) {

    if (!image || !param) {
        fprintf(stderr, "Error: Invalid input (image or parameters are NULL).\n");
        exit(TYPE_ERROR);
    }
    if (image->color_type != PNG_COLOR_TYPE_RGB) {
        fprintf(stderr, "Error: Image must be in RGB format.\n");
        exit(TYPE_ERROR);
    }

    int x_left = param->Point.x_left;
    int x_right = param->Point.x_right;
    int y_left = param->Point.y_left;
    int y_right = param->Point.y_right;

    if (x_left > x_right) {
        int temp = x_left; x_left = x_right; x_right = temp;
    }
    if (y_left > y_right) {
        int temp = y_left; y_left = y_right; y_right = temp;
    }

    int width = x_right - x_left;
    int height = y_right - y_left;

    if (x_left < 0 || x_right > image->width || y_left < 0 || y_right > image->height) {
        fprintf(stderr, "Error: Rectangle coordinates are out of image bounds.\n");
        exit(TYPE_ERROR);
    }
    if (width <= 0 || height <= 0) {
        fprintf(stderr, "Error: Invalid rectangle size.\n");
        exit(TYPE_ERROR);
    }

    png_bytep buf = malloc(width * height * 3);
    if (!buf) {
        fprintf(stderr, "Error: Memory allocation failed for buffer.\n");
        exit(MEM_ERROR);
    }

    for (int y = 0; y < height; y++) {
        int src_y = y_left + y;
        if (src_y >= 0 && src_y < image->height) {
            for (int x = 0; x < width; x++) {
                int src_x = x_left + x;
                if (src_x >= 0 && src_x < image->width) {
                    int index = (y * width + x) * 3;
                    memcpy(&buf[index], &image->row_pointers[src_y][src_x * 3], 3);
                }
            }
        }
    }
    for (int y = 0; y < image->height; y += height) {
        for (int x = 0; x < image->width; x += width) {
            int tile_height = (y + height <= image->height) ? height : (image->height - y);
            int tile_width = (x + width <= image->width) ? width : (image->width - x);
            for (int tile_y = 0; tile_y < tile_height; tile_y++) {
                for (int tile_x = 0; tile_x < tile_width; tile_x++) {
                    int buf_index = (tile_y * width + tile_x) * 3;
                    int dst_y = y + tile_y;
                    int dst_x = x + tile_x;
                    if (dst_y < image->height && dst_x < image->width) {
                        memcpy(&image->row_pointers[dst_y][dst_x * 3], &buf[buf_index], 3);
                    }
                }
            }
        }
    }
    free(buf);
}

void info_png(PNG* image){
    printf("PNG Info:\n");
    printf("Width: %d\n", image->width);
    printf("Height: %d\n", image->height);
    printf("Bit Depth: %d\n", image->bit_depth);
    printf("Color Type: %d\n", image->color_type);
    printf("Filter Method: %d\n", image->filter_method);
    printf("Interlace Method: %d\n", image->interlace_method);
}



void rotate(PNG* image, Parameters* param) {
    if (!image || !param || image->color_type != PNG_COLOR_TYPE_RGB ||
        (param->angle != 90 && param->angle != 180 && param->angle != 270)) {
        fprintf(stderr, "Error: Invalid input or angle.\n");
        exit(TYPE_ERROR);
    }

    int x_left = param->Point.x_left;
    int y_left = param->Point.y_left;
    int x_right = param->Point.x_right;
    int y_right = param->Point.y_right;
    if (x_left > x_right) {
        int temp = x_left; x_left = x_right; x_right = temp;
    }
    if (y_left > y_right) {
        int temp = y_left; y_left = y_right; y_right = temp;
    }

    int width = x_right - x_left;
    int height = y_right - y_left;
    if (width <= 0 || height <= 0) {
        fprintf(stderr, "Error: Invalid rectangle size.\n");
        exit (TYPE_ERROR);
    }

    int buf_width = (param->angle == 180) ? width : height;
    int buf_height = (param->angle == 180) ? height : width;
    png_byte* buf = malloc(buf_width * buf_height * 3);
    if (!buf) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        exit(MEM_ERROR);
    }

    for (int y = 0; y < height; y++) {
        int src_y = y_left + y;
        if (src_y >= 0 && src_y < image->height) {
            for (int x = 0; x < width; x++) {
                int src_x = x_left + x;
                if (src_x >= 0 && src_x < image->width) {
                    int dx, dy;
                    if (param->angle == 90) {
                        dx = y;
                        dy = width - x - 1;
                    } else if (param->angle == 180) {
                        dx = width - 1 - x;
                        dy = height - 1 - y;
                    } else { // 270
                        dx = height - 1 - y;
                        dy = x;
                    }
                    int index = (dy * buf_width + dx) * 3;
                    memcpy(&buf[index], &image->row_pointers[src_y][src_x * 3], 3);
                }
            }
        }
    }

    int center_x = (x_left + x_right) / 2;
    int center_y = (y_left + y_right) / 2;
    int new_x = center_x - buf_width / 2;
    int new_y = center_y - buf_height / 2;

    for (int y = 0; y < buf_height; y++) {
        int dst_y = new_y + y;
        if (dst_y >= 0 && dst_y < image->height) {
            for (int x = 0; x < buf_width; x++) {
                int dst_x = new_x + x;
                if (dst_x >= 0 && dst_x < image->width) {
                    memcpy(&image->row_pointers[dst_y][dst_x * 3], &buf[(y * buf_width + x) * 3], 3);
                }
            }
        }
    }

    free(buf);
}

static int capsule(float px, float py, float ax, float ay, float bx, float by, float r) {
    float pax = px - ax, pay = py - ay, bax = bx - ax, bay = by - ay;
    float h = fmaxf(fminf((pax * bax + pay * bay) / (bax * bax + bay * bay), 1.0f), 0.0f);
    float dx = pax - bax * h, dy = pay - bay * h;
    return dx * dx + dy * dy < r * r;
}

static void set_pixel(PNG* image, int x, int y, int r, int g, int b) {
    if (x >= 0 && x < image->width && y >= 0 && y < image->height) {
        png_byte* row = image->row_pointers[y];
        png_byte* ptr = &(row[x * 3]);
        ptr[0] = r;
        ptr[1] = g;
        ptr[2] = b;
    }
}

static void draw_line(PNG* image, float ax, float ay, float bx, float by, float r, int lr, int lg, int lb) {
    int x0 = (int)(fminf(ax, bx) - r);
    int x1 = (int)(fmaxf(ax, bx) + r);
    int y0 = (int)(fminf(ay, by) - r);
    int y1 = (int)(fmaxf(ay, by) + r);
    for (int y = y0; y <= y1; y++) {
        for (int x = x0; x <= x1; x++) {
            if (capsule((float)x, (float)y, ax, ay, bx, by, r)) {
                set_pixel(image, x, y, lr, lg, lb);
            }
        }
    }
}

void draw_square(PNG* image, Parameters* param) {
    float x0 = (float)param->Point.x_left;
    float y0 = (float)param->Point.y_left;
    float size = (float)param->side_size;
    float thickness = (float)param->thickness / 2.0f;
    int r = param->Color.r;
    int g = param->Color.g;
    int b = param->Color.b;
    bool fill = param->fill_flag;
    int fr = param->Color_Fill.r;
    int fg = param->Color_Fill.g;
    int fb = param->Color_Fill.b;

    if (param->side_size <= 0) {
        fprintf(stderr, "Error: Side size must be positive.\n");
        exit(TYPE_ERROR);
    } if (param->thickness <= 0) {
        fprintf(stderr, "Error: Thickness must be positive.\n");
        exit(TYPE_ERROR);
    } if (image->color_type != PNG_COLOR_TYPE_RGB) {
        fprintf(stderr, "Error: Image must be in RGB format.\n");
        exit(TYPE_ERROR);
    }

    int x_start = x0 < 0 ? 0 : (int)x0;
    int y_start = y0 < 0 ? 0 : (int)y0;
    int x_end = x0 + size > image->width ? image->width : (int)(x0 + size);
    int y_end = y0 + size > image->height ? image->height : (int)(y0 + size);

    if (x_end <= 0 || y_end <= 0 || x_start >= image->width || y_start >= image->height) {
        return;
    }

    if (fill) {
        for (int y = y_start; y < y_end; y++) {
            for (int x = x_start; x < x_end; x++) {
                if (!capsule(x, y, x0, y0, x0 + size, y0, thickness) && // Up
                    !capsule(x, y, x0, y0 + size, x0 + size, y0 + size, thickness) && // Down
                    !capsule(x, y, x0, y0, x0, y0 + size, thickness) && // Left
                    !capsule(x, y, x0 + size, y0, x0 + size, y0 + size, thickness) && // Right
                    !capsule(x, y, x0, y0, x0 + size, y0 + size, thickness) && // Diog 1
                    !capsule(x, y, x0 + size, y0, x0, y0 + size, thickness)) { // Diog 2
                    set_pixel(image, x, y, fr, fg, fb);
                }
            }
        }
    }

    draw_line(image, x0, y0, x0 + size, y0, thickness, r, g, b); // Up
    draw_line(image, x0, y0 + size, x0 + size, y0 + size, thickness, r, g, b); // Down
    draw_line(image, x0, y0, x0, y0 + size, thickness, r, g, b); // Left
    draw_line(image, x0 + size, y0, x0 + size, y0 + size, thickness, r, g, b); // Right

    draw_line(image, x0, y0, x0 + size, y0 + size, thickness, r, g, b); // Diog 1
    draw_line(image, x0 + size, y0, x0, y0 + size, thickness, r, g, b); // Diog 2
}

void apply_rgbfilter(PNG* image, Parameters* param) {
    if (param->component_value < 0 || param->component_value > 255) {
        fprintf(stderr, "Error: Component value must be in range 0-255.\n");
        exit(TYPE_ERROR);
    } if (image->color_type != PNG_COLOR_TYPE_RGB) {
        fprintf(stderr, "Error: Image must be in RGB format.\n");
        exit(TYPE_ERROR);
    }
    for (int y = 0; y < image->height; y++) {
        png_byte* row = image->row_pointers[y];
        for (int x = 0; x < image->width; x++) {
            png_byte* ptr = &(row[x * 3]);
            switch (param->component_name) {
                case COMPONENT_RED:
                    ptr[0] = param->component_value;
                    break;
                case COMPONENT_GREEN:
                    ptr[1] = param->component_value;
                    break;
                case COMPONENT_BLUE:
                    ptr[2] = param->component_value;
                    break;
                default:
                    fprintf(stderr, "Error: Invalid component name.\n");
                    exit(TYPE_ERROR);
            }
        }
    }
}

void help_note() {
    printf("Options available in CLI program:\n");
    printf("  -h, --help                Display help message\n");
    printf("  -i, --input  <FILE_NAME>  Specify input file name\n");
    printf("  -o, --output <FILE_NAME>  Specify output file name(default: out.png)\n");
    printf("      --info                Display PNG file information\n");
    printf("      --squared_lines       Draw a square with diagonals\n");
    printf("      --left_up <x.y>       Left-upper corner coordinates for square or rotation\n");
    printf("      --side_size <l>       Side size for square (> 0)\n");
    printf("      --thickness <t>       Line thickness for square (> 0)\n");
    printf("      --color <R.G.B>       Line color for square (e.g., 255.0.0 - red)\n");
    printf("      --fill                Fill the square(if there is a flag - true, else - false)\n");
    printf("      --fill_color <R.G.B>  Fill color for square, but diagonals are upper than fill color(e.g., 255.0.0 - red)\n");
    printf("      --rgbfilter           Apply RGB component filter\n");
    printf("      --component_name <C>  Component to filter (\"red\", \"green\", \"blue\")\n");
    printf("      --component_value <v> Component value (0-255)\n");
    printf("      --rotate              Rotate image or part of image\n");
    printf("      --right_down <x.y>    Right-lower corner coordinates for rotation\n");
    printf("      --angle <d>           Rotation angle (90, 180, 270)\n");
}