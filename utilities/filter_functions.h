#ifndef IMAGE_PROCESSING_FILTER_FUNCTIONS_H
#define IMAGE_PROCESSING_FILTER_FUNCTIONS_H
#include "../include/bmp8.h"
#include "../include/bmp24.h"

void filter_handling(t_bmp8* img_8, t_bmp24* img_24, float** kernel, int kernelSize, int img_depth);
void free_kernel(float** kernel, int kernelSize);
int allocate_kernel(float*** kernel, int kernelSize);
#endif //IMAGE_PROCESSING_FILTER_FUNCTIONS_H