#ifndef PICTURE
#define PICTURE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <png.h>

#include "errors.h"

#define PNG_HEADER 8
#define RGB_LEN 3

typedef struct {
    png_infop info_ptr;
    png_structp png_ptr;

    int width, height;
    png_byte bit_depth;
    png_byte color_type;
    png_byte filter_method;
    png_byte interlace_type;
    png_byte compression_type;

    int number_of_passes;
    png_bytep* row_pointers;
} PNG;

int read_png(PNG* img, char* path);
int write_png(PNG* img, char* path);
void info_print(PNG* img);

#endif