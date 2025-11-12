#include "drawing.h"

void repaint_pixel(PNG* img, int x, int y, colorRGB* color){
    if (x >= 0 && x < img->width && y >= 0 && y < img->height){
        png_bytep row=img->row_pointers[y];
        png_bytep ptr=&(row[x*RGB_LEN]);

        ptr[0] = color->r; ptr[1] = color->g; ptr[2] = color->b;
    }
}

void fill_area(PNG* img, point** vert, int cnt_vert, int thick, colorRGB* color){
    int x_min=vert[0]->x, x_max=vert[0]->x;
    int y_min=vert[0]->y, y_max=vert[0]->y;
    for (int i=1; i < cnt_vert; i++){
        x_min = (vert[i]->x < x_min ? vert[i]->x : x_min);
        x_max = (vert[i]->x > x_max ? vert[i]->x : x_max);

        y_min = (vert[i]->y < y_min ? vert[i]->y : y_min);
        y_max = (vert[i]->y > y_max ? vert[i]->y : y_max);
    }

    for (int y=y_min; y <= y_max; y++){
        for (int x=x_min; x <= x_max; x++){
            int board=0;
            for (int i=0; i < cnt_vert && !board; i++)
                board = check_line(x, y, vert[i], vert[(i+1)%cnt_vert], thick);
            
            int inside=0;
            for (int i=0; i < cnt_vert; i++){
                int xi=vert[i%cnt_vert]->x, yi=vert[i%cnt_vert]->y;
                int xj=vert[(i+1)%cnt_vert]->x, yj=vert[(i+1)%cnt_vert]->y;

                if ((yi > y) != (yj > y) && (x < (xj-xi)*(y-yi)/(yj-yi)+xi))
                    inside = !inside;
            }

            if (!board && inside)
                repaint_pixel(img, x, y, color);
        }
    }
}

int check_line(int x, int y, point* p1, point* p2, int thick){
    int is_line=0;
    int seg=(p2->x-p1->x)*(x-p1->x)+(p2->y-p1->y)*(y-p1->y);
    int sq_len=(p2->x-p1->x)*(p2->x-p1->x)+(p2->y-p1->y)*(p2->y-p1->y);
    int dist=abs((p2->y-p1->y)*(x-p1->x)-(p2->x-p1->x)*(y-p1->y))/sqrt(sq_len);

    if (0 <= seg && seg <= sq_len && dist <= thick/2)
        is_line = 1;
    return is_line;
}

void draw_line(PNG* img, point* p1, point* p2, int thick, colorRGB* color){
    int x_min=(p1->x < p2->x ? p1->x : p2->x), x_max=(p1->x > p2->x ? p1->x : p2->x);
    int y_min=(p1->y < p2->y ? p1->y : p2->y), y_max=(p1->y > p2->y ? p1->y : p2->y);

    for (int y=y_min-thick/2; y <= y_max+thick/2; y++){
        for (int x=x_min-thick/2; x <= x_max+thick/2; x++){
            if (check_line(x, y, p1, p2, thick))
                repaint_pixel(img, x, y, color);
        }
    }
}

void draw_circle(PNG* img, int radius, int thick, point* center, colorRGB* color){
    for (int y=-radius-thick/2; y <= radius+thick/2; y++){
        for (int x=-radius-thick/2; x <= radius+thick/2; x++){
            int d=sqrt(x*x+y*y);
            if (d >= radius-thick/2 && d <= radius+thick/2)
                repaint_pixel(img, center->x+x, center->y+y, color);
        }
    }
}

void find_vertex(point*** vert, point* center, int radius, int cnt_vert){
    *vert = (point**)calloc(cnt_vert, sizeof(point*));
    /*освобождение проиходит непосредственно в функциях рисования, откуда вызывается данная функция;
        сразу после вызова find_vertex(...), после выполнения необходимых дейтсвий, но независимо от них, выполняется проверка корректности выделения vert, проходит очистка*/
    if (!(*vert)){
        fprintf(stderr, "Memory error\n");
    }
    else{
        int err=0;
        double phase=(-M_PI/2)*(cnt_vert%2);
        for (int k=0; !err && k < cnt_vert; k++){
            (*vert)[k] = (point*)calloc(1, sizeof(point));
            /*освобождение проиходит непосредственно в функциях рисования, откуда вызывается данная функция, перед очисткой vert;
                организовано аналогично с очисткой vert, проверяется существование конкретного vert[k] и его очистка*/
            if (!(*vert)[k]){
                fprintf(stderr, "Memory error\n");
                err = ERR_MEMORY;
            }
            if (!err){
                double ang=2*M_PI*k/cnt_vert+phase;
                (*vert)[k]->x = center->x+radius*cos(ang);
                (*vert)[k]->y = center->y+radius*sin(ang);
            }
        }
    }
}

int pentagram_draw(PNG* img, argStruct* data){
    enum error_types err=ERR_NO;

    draw_circle(img, data->radius, data->thick, data->center, data->color);

    point** vert;
    find_vertex(&vert, data->center, data->radius, PENT_VERT);

    if (!vert){
        err = ERR_MEMORY;
    }
    else{
        for (int i=0; !err && i <= PENT_VERT/2; i++){
            if (!vert[i] || !vert[i+2] || !vert[i+3])
                err = ERR_MEMORY;
            
            if (!err){
                draw_line(img, vert[i], vert[i+2], data->thick, data->color);
                if (i+3 < PENT_VERT)
                    draw_line(img, vert[i], vert[i+3], data->thick, data->color);
            }
        }

        for (int i=0; i < PENT_VERT; i++){
            if (vert[i])
                free(vert[i]);
        }
        free(vert);
    }

    return err;
}

int rect_draw(PNG* img, argStruct* data){
    enum error_types err=ERR_NO;

    point** vert=(point**)calloc(RECT_VERT, sizeof(point*));
    /*освобождение проиходит в rect_draw(...), после выполнения основных действий;
        проверка корректности выделения находится на том же уровне вложенности, что и само выделение памяти*/
    if (!vert){
        fprintf(stderr, "Memory error\n");
        err = ERR_MEMORY;
    }

    for (int i=0; !err && i < RECT_VERT; i++){
        vert[i] = (point*)calloc(1, sizeof(point));
        /*освобождение проиходит в rect_draw(...), после выполнения основных действий, перед очисткой vert;
            проверка корректности выделения находится на том же уровне вложенности, что и само выделение памяти*/
        if (!vert[i]){
            fprintf(stderr, "Memory error\n");
            err = ERR_MEMORY;
        }
    }
    
    if (!err){
        vert[0]->x = data->left_up->x; vert[0]->y = data->left_up->y;
        vert[1]->x = data->right_down->x; vert[1]->y = data->left_up->y;
        vert[2]->x = data->right_down->x; vert[2]->y = data->right_down->y;
        vert[3]->x = data->left_up->x; vert[3]->y = data->right_down->y;

        for (int i=0; i < RECT_VERT; i++){
            draw_line(img, vert[i], vert[(i+1)%RECT_VERT], data->thick, data->color);
            draw_circle(img, data->thick/4, data->thick/2, vert[i], data->color);
        }

        if (data->fill){
            fill_area(img, vert, RECT_VERT, data->thick, data->fill_color);
        }
    }

    if (vert){
        for (int i=0; i < RECT_VERT; i++){
            if (vert[i])
                free(vert[i]);
        }
        free(vert);
    }
    
    return err;
}

int hexagon_draw(PNG* img, argStruct* data){
    enum error_types err=ERR_NO;

    point** vert;
    find_vertex(&vert, data->center, data->radius, HEX_VERT);

    if (!vert){
        fprintf(stderr, "Memory error\n");
        err = ERR_MEMORY;
    }
    else{
        for (int i=0; !err && i < HEX_VERT; i++){
            if (!vert[i] || !vert[(i+1)%HEX_VERT])
                err = ERR_MEMORY;
            if (!err){
                draw_line(img, vert[i], vert[(i+1)%HEX_VERT], data->thick, data->color);
                draw_circle(img, data->thick/4, data->thick/2, vert[i], data->color);
            }
        }

        if (!err && data->fill){
            fill_area(img, vert, HEX_VERT, data->thick, data->fill_color);
        }

        for (int i=0; i < HEX_VERT; i++){
            if (vert[i])
                free(vert[i]);
        }
        free(vert);
    }

    return err;
}