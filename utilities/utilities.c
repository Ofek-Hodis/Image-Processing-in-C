#include"utilities.h"


float is_in_8bit_range(float pixel_color){
    if (pixel_color > 255) return 255;
    if (pixel_color < 0) return 0;
    return pixel_color;
}
//Size comparison function used (for example) in verification of successful reading or writing
int compare_size(const size_t size1, const size_t size2, const char* error_message){
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

int find_colorDepth(const char* filename)
{
    FILE* file = fopen(filename, "rb"); //mode rb - read binary
    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, file); // Reading the header to find file type
    fclose(file);
    unsigned short colorDepth = header[28];
    if (colorDepth == 8) return 8;
    if (colorDepth == 24) return 24;
    return 0;
}
void set_pixel_value(t_bmp8 * img, const int i, const int new_val){
    int final_value = is_in_8bit_range(new_val);
    img->colorTable[i] = final_value;
}