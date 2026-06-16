#include "equalization.h"


void bmp8_equalize(t_bmp8 * img){

    unsigned int * hist = bmp8_computeHistogram(img);
    if (hist == NULL) return;

    unsigned int * hist_eq = bmp8_equalize_histogram(hist);
    if (hist_eq == NULL) return;

    free(hist);

    apply_equalization(img, hist_eq);

    free(hist_eq);

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

unsigned int * bmp8_equalize_histogram(unsigned int * hist){
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

    //cdf_min: first non-zero value in CDF
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

void apply_equalization(t_bmp8 * img, unsigned int * hist_eq){
    for (int i = 0; i < img->dataSize; i++){
        img->data[i] = hist_eq[img->data[i]];
    }
}

