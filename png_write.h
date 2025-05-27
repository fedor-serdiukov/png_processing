#ifndef PNG_WRITE_H
#define PNG_WRITE_H

#include "png_structs.h"
void free_png(PNG* image);
void write_png(char* file_name, PNG* image);

#endif