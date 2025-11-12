#include "help.h"

void help_print(){
    printf("--Functions information--\n");
    printf("\n    --help, -h: function information. Optional flag\n");
    printf("    --info: image information. Optional flag\n");
    printf("    --input, -i: input file name. Optional flag, defaults to the last argument entered as the input file name\n");
    printf("    --output, -o: output file name. Optional flag, defaults the result is saved to the out.png file\n");
    
    printf("\n    --mirror: reflection of a given area. Flags:\n");
    printf("        --left_up: coordinates of the upper left corner. The value is specified in the format 'left.up', where 'left' is the x-coordinate, 'up' is the y-coordinate\n");
    printf("        --right_down: coordinates of the lower right corner. The value is specified in the format 'right.down', where 'right' is the x-coordinate, 'down' is the y-coordinate\n");
    
    printf("\n    --pentagram: drawing a pentagram in a circle. Flags:\n");
    printf("        --center: center coordinates. The value is specified in the format 'x.y', where 'x' is the x-coordinate, 'y' is the y-coordinate\n");
    printf("        --radius: radius. Accepts a number greater than 0 as input\n");
    printf("        --thickness: line and circle thickness. Accepts a number greater than 0 as input\n");
    printf("        --color: line and circle color. Is specified by the string `rrr.ggg.bbb`, where rrr/ggg/bbb are numbers specifying the color component\n");

    printf("\n    --rect: drawing a rectangle. Flags:\n");
    printf("        --left_up: coordinates of the upper left corner. The value is specified in the format 'left.up', where 'left' is the x-coordinate, 'up' is the y-coordinate\n");
    printf("        --right_down: coordinates of the lower right corner. The value is specified in the format 'right.down', where 'right' is the x-coordinate, 'down' is the y-coordinate\n");
    printf("        --thickness: line thickness. Accepts a number greater than 0 as input\n");
    printf("        --color: line color. Is specified by the string `rrr.ggg.bbb`, where rrr/ggg/bbb are numbers specifying the color component\n");
    printf("        --fill: rectangle fill. Works as a binary value: no flag - false, flag present - true\n");
    printf("        --fill_color: rectangle fill color. Is specified by the string `rrr.ggg.bbb`, where rrr/ggg/bbb are numbers specifying the color component\n");

    printf("\n    --hexagon: drawing a regular hexagon. Flags:\n");
    printf("        --center: center coordinates. The value is specified in the format 'x.y', where 'x' is the x-coordinate, 'y' is the y-coordinate\n");
    printf("        --radius: radius. Accepts a number greater than 0 as input\n");
    printf("        --thickness: line thickness. Accepts a number greater than 0 as input\n");
    printf("        --color: line color. Is specified by the string `rrr.ggg.bbb`, where rrr/ggg/bbb are numbers specifying the color component\n");
    printf("        --fill: hexagon fill. Works as a binary value: no flag - false, flag present - true\n");
    printf("        --fill_color: hexagon fill color. Is specified by the string `rrr.ggg.bbb`, where rrr/ggg/bbb are numbers specifying the color component\n");
}