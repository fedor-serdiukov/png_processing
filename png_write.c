#include "png_write.h"
#include "png_errors.h"
#include <stdlib.h>
#include <stdio.h>

void free_png(PNG* image){
    for (int y = 0; y < image->height; y++) {
        free(image->row_pointers[y]);
    }
    free(image->row_pointers);
}

void write_png(char* file_name, PNG* image){
    FILE *fp = fopen(file_name, "wb");
    if (!fp){
        fprintf(stderr, "Failed to open file for writing.\n");
        exit(FILE_ERROR);
    }

    image->png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!image->png_ptr){
        fprintf(stderr, "Error creating PNG write structure.\n");
        fclose(fp);
        exit(FILE_ERROR);
    }

    image->info_ptr = png_create_info_struct(image->png_ptr);
    if (!image->info_ptr){
        fprintf(stderr, "Error creating PNG info structure.\n");
        png_destroy_write_struct(&image->png_ptr, (png_infopp)NULL);
        fclose(fp);
        exit(FILE_ERROR);
    }

    if (setjmp(png_jmpbuf(image->png_ptr))){
        fprintf(stderr, "Libpng internal error.\n");
        png_destroy_write_struct(&image->png_ptr, &image->info_ptr);
        fclose(fp);
        exit(LIB_PNG_ERROR);
    }
    png_init_io(image->png_ptr, fp);
    png_set_IHDR(image->png_ptr, image->info_ptr, image->width,
        image->height, image->bit_depth, image->color_type,
        PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    png_write_info(image->png_ptr, image->info_ptr);
    png_write_image(image->png_ptr, image->row_pointers);
    png_write_end(image->png_ptr, NULL);

    free_png(image);
    png_destroy_write_struct(&image->png_ptr, &image->info_ptr);
    fclose(fp);
}