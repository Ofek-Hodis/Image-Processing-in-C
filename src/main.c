#include "bmp24.h"
#include "equalization.h"

int main(){

    t_bmp24 * image = bmp24_loadImage("/home/chibremiel/code/Image-Processing-in-C/images/flowers_color.bmp");

    bmp24_equalize(image);

    bmp24_saveImage(image, "/home/chibremiel/code/Image-Processing-in-C/images/test.bmp");
    
    return 0;
}