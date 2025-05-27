#include "png_write.h"
#include "png_read.h"
#include "png_structs.h"
#include "png_errors.h"
#include "png_functions.h"
#include <stdlib.h>
#include <getopt.h>
#include <string.h>


int main(int argc, char* argv[]){
    printf("Program created by Fedor Serdyukov.\n");
    if (argc < 2){
        fprintf(stderr, "Not enough arguments were provided.\n");
        return 0;
    }

    Parameters param = {0};
    int main_actions = 0;
    int c;
    int option_index = 0;
    const char* short_options = "hi:o:";
    const struct option long_options[] = {
        {"help", no_argument, NULL, 'h'},
        {"input", required_argument, NULL, 'i'},
        {"output", required_argument, NULL, 'o'},
        {"squared_lines", no_argument, NULL, 1},
        {"left_up", required_argument, NULL, 2},
        {"side_size", required_argument, NULL, 3},
        {"thickness", required_argument, NULL, 4},
        {"color", required_argument, NULL, 5},
        {"fill", no_argument, NULL, 6},
        {"fill_color", required_argument, NULL, 7},
        {"rgbfilter", no_argument, NULL, 8},
        {"component_name", required_argument, NULL, 9},
        {"component_value", required_argument, NULL, 10},
        {"rotate", no_argument, NULL, 11},
        {"right_down", required_argument, NULL, 12},
        {"angle", required_argument, NULL, 13},
        {"info", no_argument, NULL, 14},
        {"outside_rect", no_argument, NULL, 15},
        {"paving", no_argument, NULL, 16},
        {"expand", no_argument, NULL, 17},
        {"squre_rhombus", no_argument, NULL, 18},
        {"upper_vertex", required_argument, NULL, 19},
        {"size", required_argument, NULL, 20},
        {NULL, 0, NULL, 0}
    };

    while ((c = getopt_long(argc, argv, short_options, long_options, &option_index)) != -1) {
        switch (c) {
            case 'h':
                help_note();
                return 0;
            case 'i':
                if (optarg != NULL){
                    param.input_file_name = optarg;
                }
                break;
            case 'o':
                if (optarg != NULL){
                    param.output_file_name = optarg;
                }
                break;
            case 1: {
                param.squared_lines_flag = true;
                main_actions++;
                break;
            } case 2: {
                if (optarg != NULL){
                    int x, y;
                    char tail;
                    if (sscanf(optarg, "%d.%d%c", &x, &y, &tail) == 2) {
                        param.Point.x_left = x;
                        param.Point.y_left = y;
                    } else {
                        fprintf(stderr, "Invalid format for --left_up.\n");
                        exit(TYPE_ERROR);
                    }
                } break;
            } case 3: {
                if (optarg != NULL){
                    int side;
                    char tail;
                    if (sscanf(optarg, "%d%c", &side, &tail) == 1 && side > 0) {
                        param.side_size = side;
                    } else {
                        fprintf(stderr, "Invalid format for --side_size.\n");
                        exit(TYPE_ERROR);
                    }
                } break;
            } case 4: {
                if (optarg != NULL) {
                    int thickness;
                    char tail;
                    if (sscanf(optarg, "%d%c", &thickness, &tail) == 1 && thickness > 0) {
                        param.thickness = thickness;
                    } else {
                        fprintf(stderr, "Invalid format for --thickness.\n");
                        exit(TYPE_ERROR);
                    }
                } break;
            } case 5: {
                if (optarg != NULL){
                    int r, g, b;
                    char tail;
                    if (sscanf(optarg, "%d.%d.%d%c", &r, &g, &b, &tail) == 3 &&
                        r >= 0 && r <= 255 &&
                        g >= 0 && g <= 255 &&
                        b >= 0 && b <= 255) {

                        param.Color.r = r;
                        param.Color.g = g;
                        param.Color.b = b;
                    } else {
                        fprintf(stderr, "Invalid format for --color.\n");
                        exit(TYPE_ERROR);
                    }
                } break;
            } case 6: {
                param.fill_flag = true;
                break;
            } case 7: {
                if (optarg != NULL){
                    int r, g, b;
                    char tail;
                    if (sscanf(optarg, "%d.%d.%d%c", &r, &g, &b, &tail) == 3 &&
                        r >= 0 && r <= 255 &&
                        g >= 0 && g <= 255 &&
                        b >= 0 && b <= 255) {

                        param.Color_Fill.r = r;
                        param.Color_Fill.g = g;
                        param.Color_Fill.b = b;
                    } else {
                        fprintf(stderr, "Invalid format for --fill_color.\n");
                        exit(TYPE_ERROR);
                    }
                } break;
            } case 8: {
                param.rgbfilter_flag = true;
                main_actions++;
                break;
            } case 9: {
                if (optarg != NULL){
                    if (strcmp(optarg, "red") == 0) {
                        param.component_name = COMPONENT_RED;
                    } else if (strcmp(optarg, "green") == 0) {
                        param.component_name = COMPONENT_GREEN;
                    } else if (strcmp(optarg, "blue") == 0) {
                        param.component_name = COMPONENT_BLUE;
                    } else {
                        fprintf(stderr, "Invalid format for --component_name.\n");
                        exit(TYPE_ERROR);
                    }
                } break;
            } case 10: {
                if (optarg != NULL){
                    int component_value;
                    char tail;
                    if (sscanf(optarg, "%d%c", &component_value, &tail) == 1 &&
                        component_value >= 0 && component_value <= 255) {

                        param.component_value = component_value;
                    } else {
                        fprintf(stderr, "Invalid format for --component_value.\n");
                        exit(TYPE_ERROR);
                    }
                } break;
            } case 11: {
                param.rotate_flag = true;
                main_actions++;
                break;
            } case 12: {
                if (optarg != NULL){
                    int x, y;
                    char tail;
                    if (sscanf(optarg, "%d.%d%c", &x, &y, &tail) == 2) {
                        param.Point.x_right = x;
                        param.Point.y_right = y;
                    } else {
                        fprintf(stderr, "Invalid format for --right_down.\n");
                        exit(TYPE_ERROR);
                    }
                } break;
            } case 13: {
                if (optarg != NULL){
                    int angle;
                    char tail;
                    if (sscanf(optarg, "%d%c", &angle, &tail) == 1 &&
                        (angle == 90 || angle == 180 || angle == 270)) {

                        param.angle = angle;
                    } else {
                        fprintf(stderr, "Invalid format for --angle.\n");
                        exit(TYPE_ERROR);
                    }
                } break;
            } case 14: {
                param.info_flag = true;
                break;
            }
            case 15: {
                param.outside_rect = true;
                main_actions++;
                break;
            }
            case 16: {
                param.paving_flag = true;
                main_actions++;
                break;
            }
            case 17: {
                param.expand_flag = true;
                main_actions++;
                break;
            }
            case 18: {
                param.square_rhombus_flag = true;
                main_actions++;
                break;
            }
            case 19: {
                if (optarg != NULL){
                    int x, y;
                    char tail;
                    if (sscanf(optarg, "%d.%d%c", &x, &y, &tail) == 2) {
                        param.upper_vertex_x = x;
                        param.upper_vertex_y = y;
                    } else {
                        fprintf(stderr, "Invalid format for --right_down.\n");
                        exit(TYPE_ERROR);
                    }
                } break;
            }
            case 20: {
                int size;
                char tail;
                if (sscanf(optarg, "%d%c", &size, &tail) == 1){
                    param.size = size;
                } break;
            }
            case '?':
            	fprintf(stderr, "Invalid command. Print -h for help.\n");
            	exit(COMMAND_ERROR);
            default:
                fprintf(stderr, "Invalid command: %d\n", c);
                exit(COMMAND_ERROR);
        }
    }

    if (main_actions > 1) {
        fprintf(stderr, "Error: Only one main action (squared_lines, rgbfilter, rotate) is allowed.\n");
        exit(COMMAND_ERROR);
    } if (param.input_file_name == NULL){
        fprintf(stderr, "No input file in the stream.\n");
        exit(FILE_ERROR);
    } if (param.output_file_name == NULL) param.output_file_name = "out.png";
    if (strcmp(param.input_file_name, param.output_file_name) == 0){
        fprintf(stderr, "Input and output files have the same names.\n");
        exit(FILE_ERROR);
    }

    PNG image;
    read_png(param.input_file_name, &image);
    if (param.info_flag) info_png(&image);
    if (param.squared_lines_flag){
        draw_square(&image, &param);
    }
    else if (param.rgbfilter_flag){
        apply_rgbfilter(&image, &param);
    }
    else if (param.rotate_flag){
        rotate(&image, &param);
    }
    else if (param.outside_rect){
        outside_rect(&image, &param);
    }
    else if (param.paving_flag){
        paving(&image, &param);
    }
    else if (param.expand_flag){
        expand(&image, &param);
    }
    else if (param.square_rhombus_flag){
        draw_rhombus(&image, &param);
    }
    if (main_actions) write_png(param.output_file_name, &image);
    else fprintf(stderr, "No enough arguement were provided\n");

    return 0;
}