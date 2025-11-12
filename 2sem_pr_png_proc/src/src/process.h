#ifndef PROCESS
#define PROCESS

#include "picture.h"
#include "arguments.h"

void swap_pixel(PNG* img, int x1, int y1, int x2, int y2);
int mirror_draw(PNG* img, argStruct* data);
int compress_img(PNG* img, argStruct* data);

#endif