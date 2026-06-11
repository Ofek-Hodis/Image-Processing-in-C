#include "bmp24.h"

t_pixel ** bmp24_allocateDataPixels (int width, int height){

    t_pixel ** pixel_mat = malloc(height*sizeof(t_pixel*));

    if (pixel_mat == NULL){
        printf("unexpected malloc error in bmp24_allocateDataPixels\n");
        return NULL;
    }

    for (int i = 0; i < height; i++){
        pixel_mat[i] = malloc(width*sizeof(t_pixel));

        if (pixel_mat[i] == NULL){
            printf("unexpected malloc error in bmp24_allocateDataPixels\n");
            bmp24_freeDataPixels(pixel_mat, i - 1);
            return NULL;
        }
    }

    return pixel_mat;
}


void bmp24_freeDataPixels (t_pixel ** pixel_mat, int height){

    for (int i = 0; i < height; i ++){
        free(pixel_mat[i]);
    }

    free(pixel_mat);

}

t_bmp24 * bmp24_allocate (int width, int height, int colorDepth){
    
    t_pixel ** pixel_mat = bmp24_allocateDataPixels(width, height);
    if (pixel_mat == NULL) return NULL;

    t_bmp24 * bmp24 = malloc(sizeof(t_bmp24));
    if (bmp24 == NULL){
        printf("unexpected malloc error in bmp24_allocate\n");
        bmp24_freeDataPixels(pixel_mat, height);
    }
    
    bmp24->data = pixel_mat;
    bmp24->width = width;
    bmp24->height = height;
    bmp24->colorDepth = colorDepth;

    return bmp24;

}

void bmp24_free (t_bmp24 * img){

    bmp24_freeDataPixels(img->data, img->height);
    free(img);
}

void file_rawRead (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file) {
    fseek(file, position, SEEK_SET);
    fread(buffer, size, n, file);
}

void file_rawWrite (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file) {
    fseek(file, position, SEEK_SET);
    fwrite(buffer, size, n, file);
}