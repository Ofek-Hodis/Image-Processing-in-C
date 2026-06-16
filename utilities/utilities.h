#ifndef UTILITIES_H
#define UTILITIES_H

#include<stdio.h>
#include"bmp24.h"

float is_in_8bit_range(float pixel_color);
int compare_size(const size_t size1, const size_t size2, const char* error_message);

#endif //UTILITIES_H