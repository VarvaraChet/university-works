#include "./src/arguments.h"
#include "./src/picture.h"
#include "./src/drawing.h"
#include "./src/process.h"
#include "./src/help.h"

int main(int argc, char** argv){
    enum error_types err=ERR_NO;

    argStruct* data=(argStruct*)calloc(1, sizeof(argStruct));
    /*освобождение проиходит в main(...), внутри if (!err) после выполнения основной программы;
        ошибка на данном этапе может быть поймана только на выделении памяти под data*/
    if (!data){
        fprintf(stderr, "Memory error\n");
        err = ERR_MEMORY;
    }

    if (!err){
        err = read_arguments(&argc, &argv, data);

        if (!err){
            PNG* img=(PNG*)calloc(1, sizeof(PNG));
            /*освобождение проиходит в main(...), после выполнения всех действий с изображением;
                проверка корректности выделения находится на том же уровне вложенности, что и само выделение памяти*/
            if (!img){
                fprintf(stderr, "Memory error\n");
                err = ERR_MEMORY;
            }

            if (data->in && !err){
                err = read_png(img, data->input);
            }

            if (!err){
                printf("Course work for option 5.15, created by Varvara Chetvertnaya\n");

                if (data->help)
                    help_print();
                if (data->in){
                    if (data->info)
                        info_print(img);
                    else if (data->mirror)
                        err = mirror_draw(img, data);
                    else if (data->pentagram)
                        err = pentagram_draw(img, data);
                    else if (data->rect)
                        err = rect_draw(img, data);
                    else if (data->hexagon)
                        err = hexagon_draw(img, data);
                    else if (data->compress)
                        err = compress_img(img, data);
                
                    if (!err && data->mirror+data->pentagram+data->rect+data->hexagon+data->compress)
                        err = write_png(img, data->output);
                }
                else if (data->info+data->mirror+data->pentagram+data->rect+data->hexagon+data->compress > 0){
                    fprintf(stderr, "Input file missing\n");
                    err = ERR_FILE;
                }
            }

            if (img){
                if (img->row_pointers){
                    for (int i=0; i < img->height; i++){
                        if (img->row_pointers[i])
                            free(img->row_pointers[i]);
                    }
                    free(img->row_pointers);
                }
                free(img);
            }
        }

        if (data->left_up)
            free(data->left_up);
        if (data->right_down)
            free(data->right_down);
        if (data->center)
            free(data->center);
        if (data->color)
            free(data->color);
        if (data->output)
            free(data->output);
        if (data->fill_color)
            free(data->fill_color);
        if (data->input)
            free(data->input);

        free(data);
    }

    return err;
}