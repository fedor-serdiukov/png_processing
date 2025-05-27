#include "png_read.h"
#include "png_errors.h"
#include "png_structs.h"
#include <stdio.h>
#include <png.h>
#include <stdlib.h>

void read_png(char* file_name, PNG* image){
    FILE* fp = fopen(file_name, "rb");
    if (!fp){
        fprintf(stderr, "Failed to open the file.\n");
        exit(FILE_ERROR);
    }

    png_byte header[PNG_HEADER];
    int bytes_read = fread(header, 1, PNG_HEADER, fp);
    if (bytes_read != PNG_HEADER) {
        fprintf(stderr, "Error: Failed to read PNG header. File may be too short.\n");
        fclose(fp);
        exit(FILE_ERROR);
    }

    int is_png = !png_sig_cmp(header, 0, PNG_HEADER);
    if (!is_png)
    {
        fprintf(stderr, "File isn't a PNG file.\n");
        fclose(fp);
        exit(TYPE_ERROR);
    }

    image->png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!image->png_ptr){
        fprintf(stderr, "Error creating PNG read structure.\n");
        fclose(fp);
        exit(FILE_ERROR);
    }

    image->info_ptr = png_create_info_struct(image->png_ptr);
    if (!image->info_ptr) {
        fprintf(stderr, "Error creating PNG info structure.\n");
        png_destroy_read_struct(&image->png_ptr, &image->info_ptr, (png_infopp)NULL);
        fclose(fp);
        exit(FILE_ERROR);
    }

    if (setjmp(png_jmpbuf(image->png_ptr))) {
        fprintf(stderr, "Libpng internal error.\n");
        png_destroy_read_struct(&image->png_ptr, &image->info_ptr, NULL);
        fclose(fp);
        exit(LIB_PNG_ERROR);
    }

    png_init_io(image->png_ptr, fp);
    png_set_sig_bytes(image->png_ptr, PNG_HEADER);
    png_read_info(image->png_ptr, image->info_ptr);

    image->width = png_get_image_width(image->png_ptr, image->info_ptr);
    image->height = png_get_image_height(image->png_ptr, image->info_ptr);
    image->bit_depth = png_get_bit_depth(image->png_ptr, image->info_ptr);
    image->color_type = png_get_color_type(image->png_ptr, image->info_ptr);
    image->filter_method = png_get_filter_type(image->png_ptr, image->info_ptr);
    image->interlace_method = png_get_interlace_type(image->png_ptr, image->info_ptr);

    image->row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * image->height);
    if (!image->row_pointers) {
        fprintf(stderr, "Memory allocation error for row_pointers.\n");
        png_destroy_read_struct(&image->png_ptr, &image->info_ptr, NULL);
        fclose(fp);
        exit(MEM_ERROR);
    }

    for (int y = 0; y < image->height; y++) {
        image->row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(image->png_ptr, image->info_ptr));
        if (!image->row_pointers[y]) {
            fprintf(stderr, "Memory allocation error for %d row.\n", y);
            for (int i = 0; i < y; i++) {
                free(image->row_pointers[i]);
            }
            free(image->row_pointers);
            png_destroy_read_struct(&image->png_ptr, &image->info_ptr, NULL);
            fclose(fp);
            exit(MEM_ERROR);
        }
    }
    png_read_image(image->png_ptr, image->row_pointers);
    fclose(fp);
}