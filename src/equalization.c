#include "equalization.h"


void bmp8_equalize(t_bmp8 * img){

    unsigned int * hist = bmp8_computeHistogram(img);
    if (hist == NULL) return;

    unsigned int * hist_eq = equalize_histogram(hist);
    if (hist_eq == NULL) return;

    free(hist);

    bmp8_apply_equalization(img, hist_eq);

    free(hist_eq);

}

void bmp24_equalize(t_bmp24 * img){

    yuv * yuv_tab = convert_RGB_to_YUV(img);
    if (yuv_tab == NULL) return;
    
    unsigned int * hist = bmp24_computeHistogram(img, yuv_tab);
    if (hist == NULL) return;

    unsigned int * hist_eq = equalize_histogram(hist);
    if (hist_eq == NULL) return;

    free(hist);

    bmp24_apply_equalization(img, hist_eq, yuv_tab);

    convert_YUV_to_RGB(img, yuv_tab);

    free(hist_eq);
    free(yuv_tab);
}

unsigned int * bmp8_computeHistogram(t_bmp8 * img){
    
    unsigned int * hist = calloc(256, sizeof(unsigned int));
    if (hist == NULL){
        printf("unexpected calloc error in bmp8_computeHistogram\n");
        return NULL;
    }

    for (int i = 0; i < img->dataSize; i++){
        hist[img->data[i]]++;
    }

    return hist;
}

unsigned int * equalize_histogram(unsigned int * hist){
    float * cdf = malloc(256 * sizeof(float));
    unsigned int * hist_eq = malloc(256 * sizeof(unsigned int));
    if (cdf == NULL || hist_eq == NULL){
        printf("unexpected malloc error in bmp8_computeCDF\n");
        free(cdf);
        free(hist_eq);
        return NULL;
    }

    // Compute CDF: each entry is the cumulative sum of all
    // histogram values from index 0 up to index i.
    cdf[0] = (float)hist[0];
    for (int i = 1; i < 256; i++){
        cdf[i] = cdf[i-1] + hist[i];
    }

    //cdf_min: first non-zero value in CDFvoid bmp24_equalize(t_bmp24 * img)
    float cdf_min = 0;
    for (int i = 0; i < 256; i++){
        if (cdf[i] != 0){
            cdf_min = cdf[i];
            break;
        }
    }

    for (int i = 0; i < 256; i++){
        hist_eq[i] = round(((cdf[i] - cdf_min)/(cdf[255] - cdf_min))*255);
    }

    free(cdf);

    return hist_eq;

}

void bmp8_apply_equalization(t_bmp8 * img, unsigned int * hist_eq){
    for (int i = 0; i < img->dataSize; i++){
        img->data[i] = hist_eq[img->data[i]];
    }
}


yuv * convert_RGB_to_YUV(t_bmp24 * image){
    yuv * yuv_tab = malloc(image->height * image->width * sizeof(yuv));
    if (yuv_tab == NULL){
        printf("unexpected calloc error in convert_RGB_to_YUV\n");
        return NULL;
    }

    for (int y = 0; y < image->height; y++){
        for (int x = 0; x < image->width; x++){
            yuv_tab[y * image->width + x].Y = is_in_8bit_range(0.299 * image->data[x][y].red + 0.587 * image->data[x][y].green + 0.114 * image->data[x][y].blue);
            yuv_tab[y * image->width + x].U = -0.14713 * image->data[x][y].red - 0.28886 * image->data[x][y].green + 0.436 * image->data[x][y].blue;
            yuv_tab[y * image->width + x].V = 0.615 * image->data[x][y].red - 0.51499 * image->data[x][y].green - 0.10001 * image->data[x][y].blue;
        }
    }
    return yuv_tab;
}

unsigned int * bmp24_computeHistogram(t_bmp24 * img, yuv * yuv_tab){
    
    unsigned int * hist = calloc(256, sizeof(unsigned int));
    if (hist == NULL){
        printf("unexpected calloc error in bmp24_computeHistogram\n");
        return NULL;
    }

    for (int i = 0; i < img->height * img->width; i++){
        hist[(unsigned int)round(yuv_tab[i].Y)]++;
    }

    return hist;
}

void bmp24_apply_equalization(t_bmp24 * img, unsigned int * hist_eq, yuv * yuv_tab){
    for (int i = 0; i < img->height * img->width; i++){
        yuv_tab[i].Y = hist_eq[(unsigned int)round(yuv_tab[i].Y)];
    }
}

//each pixels are converted to RGB then clamp to 0 <= val < 256 and round to lose less precision
void convert_YUV_to_RGB(t_bmp24 * img, yuv * yuv_tab){
    for (int y = 0; y < img->height; y++){
        for (int x = 0; x < img->width; x++){
            img->data[x][y].red = (unsigned int)round(is_in_8bit_range(yuv_tab[y * img->width + x].Y + 1.13983 * yuv_tab[y * img->width + x].V));
            img->data[x][y].green = (unsigned int)round(is_in_8bit_range(yuv_tab[y * img->width + x].Y - 0.39465 * yuv_tab[y * img->width + x].U - 0.5806 * yuv_tab[y * img->width + x].V));
            img->data[x][y].blue = (unsigned int)round(is_in_8bit_range(yuv_tab[y * img->width + x].Y + 2.03211 * yuv_tab[y * img->width + x].U));
        }
    }
}