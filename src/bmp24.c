#include "bmp24.h"


t_bmp24 * bmp24_loadImage (const char * filename){

    FILE * img_file = fopen(filename, "rb");
    if (img_file == NULL){
        printf("file cannot be oppen\n");
        return NULL;
    }

    t_bmp_header img_header;
    t_bmp_info img_info;

    file_rawRead(BITMAP_MAGIC, &img_header, sizeof(t_bmp_header), 1, img_file);
    file_rawRead(HEADER_SIZE, &img_info, sizeof(t_bmp_info), 1, img_file);

    t_bmp24 * img = bmp24_allocate(img_info.width, img_info.height, img_info.bits);
    img->header = img_header;
    img->header_info = img_info;

    bmp24_readPixelData(img, img_file);

    fclose(img_file);

    return img;
}

void bmp24_saveImage (t_bmp24 * image, const char * filename){

    FILE * file = fopen(filename, "wb");
    if (file == NULL){
        printf("file cannot be oppen\n");
        return;
    }

    file_rawWrite(BITMAP_MAGIC, &image->header, sizeof(t_bmp_header), 1, file);
    file_rawWrite(HEADER_SIZE, &image->header_info, sizeof(t_bmp_info), 1, file);

    bmp24_writePixelData(image, file);

    fclose(file);
}

void bmp24_convolution (t_bmp24 * img, float ** kernel, int kernelSize){
    
    if (img == NULL){
        printf("file cannot be oppen\n");
        return;
    }

    int border = (kernelSize - 1)/2;

    t_pixel ** new_matrix = bmp24_allocateDataPixels(img->width, img->height);

    apply_convolution(img, kernel, kernelSize, new_matrix, border);

    fill_border(img, kernel, kernelSize, new_matrix, border);

    bmp24_freeDataPixels(img->data, img->height);

    img->data = new_matrix;

}

void bmp24_negative (t_bmp24 * img){
    if (img == NULL){
        printf("file cannot be oppen\n");
        return;
    }

    for(int y = 0; y < img->height; y++){
        for(int x = 0; x < img->width; x++){
            img->data[y][x].blue = 255 - img->data[y][x].blue;
            img->data[y][x].green = 255 - img->data[y][x].green;
            img->data[y][x].red = 255 - img->data[y][x].red;
        }
    }
}

void bmp24_grayscale (t_bmp24 * img){
    if (img == NULL){
        printf("file cannot be oppen\n");
        return;
    }

    for(int y = 0; y < img->height; y++){
        for(int x = 0; x < img->width; x++){
            int average_color = (int)(img->data[y][x].blue + img->data[y][x].green + img->data[y][x].red)/3;
            img->data[y][x].blue = average_color;
            img->data[y][x].green = average_color;
            img->data[y][x].red = average_color;
        }
    }
}

void bmp24_brightness (t_bmp24 * img, int value){
    if (img == NULL){
        printf("file cannot be oppen\n");
        return;
    }

    for(int y = 0; y < img->height; y++){
        for(int x = 0; x < img->width; x++){
            int new_value = img->data[y][x].blue + value;
            img->data[y][x].blue = is_in_8bit_range(new_value);
            new_value = img->data[y][x].green + value;
            img->data[y][x].green = is_in_8bit_range(new_value);
            new_value = img->data[y][x].red + value;
            img->data[y][x].red = is_in_8bit_range(new_value);
        }
    }
}

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
            bmp24_freeDataPixels(pixel_mat, i);
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
        return NULL;
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

void  bmp24_readPixelValue (t_bmp24 * image, int x, int y, FILE * file){
    t_pixel pixel;
    uint32_t position = image->header.offset + (x * 3) + (image->width * 3) * (image->height - y - 1);
    file_rawRead(position, &pixel, sizeof(t_pixel), 1, file);
    image->data[y][x] = pixel;
}

void bmp24_readPixelData(t_bmp24 * image, FILE * file){
    for(int y = 0; y < image->height; y++){
        for(int x = 0; x < image->width; x++){
            bmp24_readPixelValue(image, x, y, file);
        }
    }
}

void bmp24_writePixelValue (t_bmp24 * image, int x, int y, FILE * file){
    t_pixel pixel = image->data[y][x];
    uint32_t position = image->header.offset + (x * 3) + (image->width * 3) * (image->height - y - 1);
    file_rawWrite(position, &pixel, sizeof(t_pixel), 1, file);
}

void bmp24_writePixelData (t_bmp24 * image, FILE * file){
    for(int y = 0; y < image->height; y++){
        for(int x = 0; x < image->width; x++){
            bmp24_WritePixelValue(image, x, y, file);
        }
    }
}

void apply_convolution(t_bmp24 * img, float ** kernel, int kernelSize, t_pixel ** new_matrix, int border){
    for (int y = border; y < img->height - border; y++){
        for (int x = border; x < img->width - border; x++){
            float blue_summ = 0;
            float green_summ = 0;
            float red_summ = 0;
            for (int i = -border; i <= border; i++){
                for (int j = -border; j <= border; j++){
                    blue_summ += kernel[border + i][border + j] * img->data[y + i][x + j].blue;
                    green_summ += kernel[border + i][border + j] * img->data[y + i][x + j].green;
                    red_summ +=  kernel[border + i][border + j] * img->data[y + i][x + j].red;
                }
            }
            new_matrix[y][x].blue = (int)is_in_8bit_range(blue_summ);
            new_matrix[y][x].green = (int)is_in_8bit_range(green_summ);
            new_matrix[y][x].red = (int)is_in_8bit_range(red_summ);
        }
    }
}

void fill_border(t_bmp24 * img, float ** kernel, int kernelSize, t_pixel ** new_matrix, int border){

    for (int y = border; y < img->height - border; y++){

        for (int x = 0;x < border; x++){
            new_matrix[y][x] = img->data[y][x];
        }

        for (int x = img->width - border; x < img->width; x++){
            new_matrix[y][x] = img->data[y][x];
        }
    }

    for (int y = 0; y < border; y++){
        for (int x = 0; x < img->width; x++){
            new_matrix[y][x] = img->data[y][x];
        }
    }

    for (int y = img->height - border ; y < img->height; y++){
        for (int x = 0 ;x < img->width; x++){
            new_matrix[y][x] = img->data[y][x];
        }
    }

}