
#include"utilities.h"

float is_in_8bit_range(float pixel_color){
    if (pixel_color > 255) return 255;
    if (pixel_color < 0) return 0;
    return pixel_color;
}
