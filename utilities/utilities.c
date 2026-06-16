
#include"utilities.h"

float is_in_8bit_range(float pixel_color){
    if (pixel_color > 255) return 255;
    if (pixel_color < 0) return 0;
    return pixel_color;
}

int compare_size(const size_t size1, const size_t size2, const char* error_message){
    if (size1 == size2) return 1;
    printf("Error - %s\n", error_message);
    return 0;
}