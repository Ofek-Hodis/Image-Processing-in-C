#ifndef UTILITIES_H
#define UTILITIES_H

#include<stdio.h>
#include "bmp24.h"
#include "bmp8.h"

float is_in_8bit_range(float pixel_color);
int compare_size(const size_t size1, const size_t size2, const char* error_message);
void set_pixel_value(t_bmp8 * img, const int i, const int new_val);

#endif //UTILITIES_H