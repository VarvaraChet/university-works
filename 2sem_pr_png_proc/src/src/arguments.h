#ifndef ARGUMENTS
#define ARGUMENTS

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

#include "errors.h"

enum flagValue{
    ABSENT = 0,
    PRESENT = 1
};

typedef struct{
    int x, y;
} point;

typedef struct{
    int r, g, b;
} colorRGB;

typedef struct{
    enum flagValue help, info, in, out, lu, rd, cent, col, fc;
    int mirror, pentagram, rect, hexagon, compress;
    
    char axis;
    char *input, *output;
    int radius, thick, fill, num;
    colorRGB *color, *fill_color;
    point *center, *left_up, *right_down;
} argStruct;

static struct option options[] = {
    {"help", no_argument, 0, 'h'},
    {"info", no_argument, 0, 'I'},
    {"input", required_argument, 0, 'i'},
    {"output", required_argument, 0, 'o'},
    {"mirror", no_argument, 0, 'M'},
    {"pentagram", no_argument, 0, 'P'},
    {"rect", no_argument, 0, 'R'},
    {"hexagon", no_argument, 0, 'H'},
    {"axis", required_argument, 0, 'a'},
    {"left_up", required_argument, 0, 'u'},
    {"right_down", required_argument, 0, 'd'},
    {"center", required_argument, 0, 'c'},
    {"radius", required_argument, 0, 'r'},
    {"thickness", required_argument, 0, 't'},
    {"color", required_argument, 0, 'C'},
    {"fill", no_argument, 0, 'f'},
    {"fill_color", required_argument, 0, 'F'},
    {"compress", no_argument, 0, 'A'},
    {"num", required_argument, 0, 'n'},
    {0, 0, 0, 0}
};

int read_arguments(int* arg_c, char*** arg_v, argStruct* data);

#endif