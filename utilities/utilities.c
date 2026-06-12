
#include"utilities.h"

int is_in_8bit_range(int pixel_color, int value){
    int res = pixel_color + value;
    if (res > 255) return 255;
    if (res < 0) return 0;
    return res;
}