#ifndef DRAW
#define DRAW

#include <math.h>

#include "picture.h"
#include "arguments.h"

#define PENT_VERT 5
#define HEX_VERT 6
#define RECT_VERT 4

void repaint_pixel(PNG* img, int x, int y, colorRGB* color);
void fill_area(PNG* img, point** vert, int cnt_vert, int thick, colorRGB* color);
int check_line(int x, int y, point* p1, point* p2, int thick);
void draw_line(PNG* img, point* p1, point* p2, int thick, colorRGB* color);
void draw_circle(PNG* img, int radius, int thick, point* center, colorRGB* color);
void find_vertex(point*** vert, point* center, int radius, int cnt_vert);

int pentagram_draw(PNG* img, argStruct* data);
int rect_draw(PNG* img, argStruct* data);
int hexagon_draw(PNG* img, argStruct* data);

#endif