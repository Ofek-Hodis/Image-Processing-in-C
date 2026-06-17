#include "filter_functions.h"

void filter_handling(t_bmp8* img_8, t_bmp24* img_24, float** kernel, int kernelSize, int img_depth)
{
    if (img_depth == 8 )
    {
        bmp8_applyFilter(img_8, kernel, kernelSize);
        return;
    }
    if (img_depth == 24)
    {
        bmp24_convolution(img_24, kernel, kernelSize);
        return;
    }
    printf("Color depth not accepted (expected 8 or 24) - verify file type\n");
}

void free_kernel(float** kernel, int kernelSize)
{
    if (kernel == NULL)
    {
        printf("Kernell already empty");
        return;
    }
    for (int i=0; i<kernelSize; i++) // Creation of blur kernel
    {
        if (kernel[i] != NULL) free(kernel[i]);
    }
    free(kernel);
}

int allocate_kernel(float*** kernel, int kernelSize)
{
    //calloc necessary in case the second malloc fails, so all of the lignes allocated can be freed
    *kernel = calloc(kernelSize, sizeof(float*));
    if (*kernel == NULL)
    {
        printf("Memory allocation failed");
        return 0;
    }
    for (int i=0; i<kernelSize; i++)
    {
        (*kernel)[i] = malloc(kernelSize*sizeof(float));
        if ((*kernel)[i] == NULL)
        {
            printf("Memory allocation failed");
            free_kernel(*kernel, kernelSize);
            return 0;
        }
    }
    return 1;
}