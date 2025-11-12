#include "picture.h"

int read_png(PNG* img, char* path){
    enum error_types err=ERR_NO;

    FILE* fp=fopen(path, "rb");
    if (!fp){
        fprintf(stderr, "Incorrect input file\n");
        err = ERR_FILE;
    }
    else{
        char header[PNG_HEADER];
        fread(header, 1, PNG_HEADER, fp);
        int is_png = !png_sig_cmp(header, 0, PNG_HEADER);
        if (!is_png) {
            fprintf(stderr, "Not PNG\n");
            err = ERR_IMAGE;
        }

        if (!err){
            img->png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
            if (!img->png_ptr){
                fprintf(stderr, "PNG_read_struct error\n");
                err = ERR_IMG_STRUCT;
            }

            if (!err){
                img->info_ptr = png_create_info_struct(img->png_ptr);
                if (!img->info_ptr){
                    png_destroy_read_struct(&img->png_ptr, NULL, NULL);
                    fprintf(stderr, "PNG_info_struct error\n");
                    err = ERR_IMG_STRUCT;
                }

                if (!err){
                    if (setjmp(png_jmpbuf(img->png_ptr))){
                        png_destroy_read_struct(&img->png_ptr, &img->info_ptr, NULL);
                        fprintf(stderr, "Input image error\n");
                        err = ERR_IMAGE;
                    }

                    if (!err){
                        png_init_io(img->png_ptr, fp);
                        png_set_sig_bytes(img->png_ptr, PNG_HEADER);

                        png_read_info(img->png_ptr, img->info_ptr);

                        img->width = png_get_image_width(img->png_ptr, img->info_ptr);
                        img->height = png_get_image_height(img->png_ptr, img->info_ptr);
                        img->bit_depth = png_get_bit_depth(img->png_ptr, img->info_ptr);
                        img->color_type = png_get_color_type(img->png_ptr, img->info_ptr);
                        img->filter_method = png_get_filter_type(img->png_ptr, img->info_ptr);
                        img->compression_type = png_get_compression_type(img->png_ptr, img->info_ptr);
                        img->interlace_type = png_get_interlace_type(img->png_ptr, img->info_ptr);
                        img->number_of_passes = png_set_interlace_handling(img->png_ptr);

                        png_read_update_info(img->png_ptr, img->info_ptr);

                        img->row_pointers = (png_bytep*)calloc(img->height, sizeof(png_bytep));
                        /*освобождение проиходит в main(...), после выполнения действий с изображением;
                            проверка корректности выделения происходит всегда, если успешно выделилась память под изображение (т.е. могло быть вызвано данное выделение),
                                если произошёл сбой выделения памяти для изображения, то даннай функция не могла быть вызвана*/
                        if (!img->row_pointers){
                            fprintf(stderr, "Memory error\n");
                            err = ERR_MEMORY;
                        }
                        for (int y=0; !err && y < img->height; y++){
                            img->row_pointers[y] = (png_byte*)calloc(1, png_get_rowbytes(img->png_ptr, img->info_ptr));
                            /*освобождение проиходит в main(...), после выполнения действий с изображением, перед очисткой img->row_pointers;
                                организовано аналогично с очисткой img->row_pointers, проверяется существование конкретного img->row_pointers[i] и его очистка*/
                            if (!img->row_pointers[y]){
                                fprintf(stderr, "Memory row error\n");
                                err = ERR_MEMORY;
                            }
                        }
                        if (!err)
                            png_read_image(img->png_ptr, img->row_pointers);
                    }
                }
            }
        }
        fclose(fp);
    }
    return err;
}

int write_png(PNG* img, char* path){
    enum error_types err=ERR_NO;

    FILE* fp = fopen(path, "wb");
    if (!fp) {
        fprintf(stderr, "Incorrect output file\n");
        err = ERR_FILE;
    }
    else{
        png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png_ptr) {
            fprintf(stderr, "PNG_write_struct error\n");
            err = ERR_IMG_STRUCT;
        }

        if (!err){
            png_infop info_ptr = png_create_info_struct(png_ptr);
            if (!info_ptr) {
                fprintf(stderr, "PNG_info_struct error\n");
                err = ERR_IMG_STRUCT;
            }

            if (!err){
                if (setjmp(png_jmpbuf(png_ptr))) {
                    fprintf(stderr, "Output image error\n");
                    err = ERR_IMAGE;
                }

                if (!err){
                    png_init_io(png_ptr, fp);

                    png_set_IHDR(png_ptr, info_ptr, img->width, img->height,
                                img->bit_depth, img->color_type, PNG_INTERLACE_NONE,  
                                PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

                    png_write_info(png_ptr, info_ptr);
                    png_write_image(png_ptr, img->row_pointers);
                    png_write_end(png_ptr, NULL);

                    png_destroy_write_struct(&png_ptr, &info_ptr);
                }
            }
        }
        fclose(fp);
    }
    return err;
}

void info_print(PNG* img){
    printf("--Image Information--\n");
    printf("    Dimensions: %d x %d pixels\n", img->width, img->height);
    printf("    Bit depth: %d\n", img->bit_depth);
    printf("    Compression method: %d\n", img->compression_type);
    printf("    Filter method: %d\n", img->filter_method);
    printf("    Number of passes: %d\n", img->number_of_passes);
}