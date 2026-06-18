#ifndef IMAGE_PROCESSING_IN_C_EQUALIZATION_H
#define IMAGE_PROCESSING_IN_C_EQUALIZATION_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../utilities/utilities.h"
#include "bmp8.h"
#include "bmp24.h"

typedef struct {
    double Y;
    double U;
    double V;
}yuv;


unsigned int * bmp8_computeHistogram(t_bmp8 * img);
unsigned int * equalize_histogram(unsigned int * hist);
void bmp8_apply_equalization(t_bmp8 * img, unsigned int * hist_eq);
void bmp8_equalize(t_bmp8 * img);
yuv * convert_RGB_to_YUV(t_bmp24 * image);
unsigned int * bmp24_computeHistogram(t_bmp24 * img, yuv * yuv_mat);
void bmp24_apply_equalization(t_bmp24 * img, unsigned int * hist_eq, yuv * yuv_mat);
void convert_YUV_to_RGB(t_bmp24 * img, yuv * yuv_mat);
void bmp24_equalize(t_bmp24 * img);

#endif //IMAGE_PROCESSING_IN_C_EQUALIZATION_H
