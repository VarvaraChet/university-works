#include "process.h"

void swap_pixel(PNG* img, int x1, int y1, int x2, int y2){
    if (x1 >= 0 && x1 < img->width && y1 >= 0 && y1 < img->height
        && x2 >= 0 && x2 < img->width && y2 >= 0 && y2 < img->height){
        png_bytep row1=img->row_pointers[y1], row2=img->row_pointers[y2];
        png_bytep p1=&(row1[x1*RGB_LEN]), p2=&(row2[x2*RGB_LEN]);

        for (int i=0; i < RGB_LEN; i++){
            png_byte tmp=p1[i];
            p1[i] = p2[i];
            p2[i] = tmp;
        }
    }
}

int mirror_draw(PNG* img, argStruct* data){
    if (data->axis == 'y'){
        for (int y=data->left_up->y; y <= (data->right_down->y+data->left_up->y)/2; y++){
            for (int x=data->left_up->x; x <= data->right_down->x; x++)
                swap_pixel(img, x, y, x, data->left_up->y+data->right_down->y-y);
        }
    }

    if (data->axis == 'x'){
        for (int y=data->left_up->y; y <= data->right_down->y; y++){
            for (int x=data->left_up->x; x <= (data->right_down->x+data->left_up->x)/2; x++)
                swap_pixel(img, x, y, data->right_down->x+data->left_up->x-x, y);
        }
    }
    return ERR_NO;
}

int compress_img(PNG* img, argStruct* data){
    enum error_types err=ERR_NO;

    int new_width = img->width/data->num;
    int new_height = img->height/data->num;

    png_bytep* new_img = (png_bytep*)calloc(new_height, sizeof(png_bytep));
    /*освобождение проиходит в compress_img(...), после выполнения основных действий;
        проверка корректности выделения находится на том же уровне вложенности, что и само выделение памяти*/
    if (!new_img){
        fprintf(stderr, "Memory error\n");
        err = ERR_MEMORY;
    }
    for (int y=0; !err && y < new_height; y++){
        new_img[y] = (png_byte*)calloc(1, new_width*RGB_LEN);
        /*освобождение проиходит в compress_img(...), после выполнения основных действий;
            проверка корректности выделения находится на том же уровне вложенности, что и само выделение памяти*/
        if (!new_img[y]){
            fprintf(stderr, "Memory row error\n");
            err = ERR_MEMORY;
        }
    }

    for (int y=0; !err && y < new_height; y++){
        for (int x=0; x < new_width; x++){
            int r=0, g=0, b=0;

            for (int i=y*data->num; i < (y+1)*data->num; i++){
                for (int j=x*data->num; j < (x+1)*data->num; j++){
                    png_bytep row=img->row_pointers[i];
                    png_bytep p=&(row[j*RGB_LEN]);
                    r += p[0]; g += p[1]; b += p[2];
                }
            }
            r /= (data->num*data->num); g /= (data->num*data->num); b /= (data->num*data->num);

            png_bytep new_row=new_img[y];
            png_bytep new_p=&(new_row[x*RGB_LEN]);
            new_p[0] = r; new_p[1] = g; new_p[2] = b;

        }
    }

    if (!err){
        img->width = new_width;
        img->height = new_height;

        img->row_pointers = (png_bytep*)realloc(img->row_pointers, img->height*sizeof(png_bytep));
        if (!img->row_pointers){
            fprintf(stderr, "Memory error\n");
            err = ERR_MEMORY;
        }
        for (int y=0; !err && y < img->height; y++){
            img->row_pointers[y] = (png_byte*)realloc(img->row_pointers[y], img->width*RGB_LEN);
            if (!img->row_pointers[y]){
                fprintf(stderr, "Memory row error\n");
                err = ERR_MEMORY;
            }
        }

        for (int y=0; !err && y < img->height; y++){
            for (int x=0; x < img->width; x++){
                png_bytep row1=img->row_pointers[y], row2=new_img[y];
                png_bytep p1=&(row1[x*RGB_LEN]), p2=&(row2[x*RGB_LEN]);
                p1[0] = p2[0]; p1[1] = p2[1]; p1[2] = p2[2];
            }
        }
    }

    if (new_img){
        for (int y=0; y < new_height; y++){
            if (new_img[y])
                free(new_img[y]);
        }
        free(new_img);
    }

    return err;
}