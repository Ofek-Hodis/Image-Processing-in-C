#ifndef IMAGE_PROCESSING_UTILS_H
#define IMAGE_PROCESSING_UTILS_H
#include "../include/bmp8.h"
#include <stdio.h>


int compare_size(size_t size1, size_t size2, const char* error_message);
int value_limit(int new_val);
void set_pixel_value(t_bmp8 * img, int i, int new_val);

#endif //IMAGE_PROCESSING_UTILS_H