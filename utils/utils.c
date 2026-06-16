#include "utils.h"

//Size comparison function used (for example) in verification of successful reading or writing
int compare_size(const size_t size1, const size_t size2, const char* error_message)
{
    if (size1 == size2) return 1;
    printf("Error - %s\n", error_message);
    return 0;
}

int value_limit(const int new_val)
{
    if (new_val > 255)
    {
        return 255;
    }
    if (new_val < 0)
    {
        return 0;
    }
    return new_val;
}

void set_pixel_value(t_bmp8 * img, const int i, const int new_val)
{
    int final_value = value_limit(new_val);
    img->colorTable[i] = final_value;
}