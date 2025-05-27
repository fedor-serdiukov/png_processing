#ifndef PNG_FUNCTIONS_H
#define PNG_FUNCTIONS_H

#include "png_structs.h"
void draw_rhombus(PNG* image, Parameters* param);
void expand(PNG* image, Parameters* param);
void outside_rect(PNG* image, Parameters* param);
void paving(PNG* image, Parameters* param);
void info_png(PNG* image);
void rotate(PNG* image, Parameters* param);
void draw_square(PNG* image, Parameters* param);
void apply_rgbfilter(PNG* image, Parameters* param);
void help_note();

#endif
