#ifndef UTILITIES_H
#define UTILITIES_H
#include <stdio.h>
#include "../include/bmp8.h"
#include "../include/bmp24.h"

float is_in_8bit_range(float pixel_color);
int compare_size(const size_t size1, const size_t size2, const char* error_message);
int value_limit(int new_val);
void set_pixel_value(t_bmp8 * img, int i, int new_val);
int find_colorDepth(const char* filename);

#endif //UTILITIES_H