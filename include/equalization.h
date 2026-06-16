#ifndef IMAGE_PROCESSING_IN_C_EQUALIZATION_H
#define IMAGE_PROCESSING_IN_C_EQUALIZATION_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "utilities.h"
#include "bmp8.h"
#include "bmp24.h"

unsigned int * bmp8_computeHistogram(t_bmp8 * img);
unsigned int * bmp8_equalize_histogram(unsigned int * hist);
void apply_equalization(t_bmp8 * img, unsigned int * hist_eq);
void bmp8_equalize(t_bmp8 * img);

#endif //IMAGE_PROCESSING_IN_C_EQUALIZATION_H