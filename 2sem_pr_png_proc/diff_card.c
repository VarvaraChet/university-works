#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <png.h>

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

int read_png(PNG* img, char* path){
    FILE* fp=fopen(path, "rb");
    if (!fp){
        fprintf(stderr, "Incorrect input file\n");
        return 1;
    }

    char header[PNG_HEADER];
    fread(header, 1, PNG_HEADER, fp);
    int is_png = !png_sig_cmp(header, 0, PNG_HEADER);
    if (!is_png) {
        fprintf(stderr, "Not PNG\n");
        return 1;
    }

    img->png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!img->png_ptr){
        fprintf(stderr, "PNG_read_struct error\n");
        return 1;
    }

    img->info_ptr = png_create_info_struct(img->png_ptr);
    if (!img->info_ptr){
        png_destroy_read_struct(&img->png_ptr, NULL, NULL);
        fprintf(stderr, "PNG_info_struct error\n");
        return 1;
    }

    if (setjmp(png_jmpbuf(img->png_ptr))){
        png_destroy_read_struct(&img->png_ptr, &img->info_ptr, NULL);
        fclose(fp);
        fprintf(stderr, "Input image error\n");
        return 1;
    }

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
    if (!img->row_pointers){
        fprintf(stderr, "Memory error\n");
        return 1;
    }
    for (int y=0; y < img->height; y++){
        img->row_pointers[y] = (png_byte*)calloc(1, png_get_rowbytes(img->png_ptr, img->info_ptr));
        if (!img->row_pointers[y]){
            fprintf(stderr, "Memory row error\n");
            return 1;
        }
    }
    
    png_read_image(img->png_ptr, img->row_pointers);
    fclose(fp);
    return 0;
}

int write_png(PNG* img, char* path){
    FILE* fp = fopen(path, "wb");
    if (!fp) {
        fprintf(stderr, "Incorrect output file\n");
        return 1;
    }

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        fprintf(stderr, "PNG_write_struct error\n");
        return 1;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        fclose(fp);
        fprintf(stderr, "PNG_info_struct error\n");
        return 1;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        fprintf(stderr, "Output image error\n");
        return 1;
    }

    png_init_io(png_ptr, fp);

    png_set_IHDR(png_ptr, info_ptr, img->width, img->height,
                 img->bit_depth, img->color_type, PNG_INTERLACE_NONE,  
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);
    png_write_image(png_ptr, img->row_pointers);
    png_write_end(png_ptr, NULL);
    
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);

    return 0;
}

int main(int argc, char** argv){
    PNG* img1=(PNG*)calloc(1, sizeof(PNG));
    PNG* img2=(PNG*)calloc(1, sizeof(PNG));

    read_png(img1, argv[1]);
    read_png(img2, argv[2]);

    for (int y=0; y < img1->height && y < img2->height; y++){
        for (int x=0; x < img1->width && x < img2->width; x++){
            png_bytep row1=img1->row_pointers[y], row2=img2->row_pointers[y];
            png_bytep p1=&(row1[x*3]), p2=&(row2[x*3]);

            p1[0] -= p2[0]; p1[1] -= p2[1]; p1[2] -= p2[2];
        }
    }

    write_png(img1, "diff_card.png");
}