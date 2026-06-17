#include <stdio.h>
#include <stdlib.h>
#include "../utilities/filter_functions.h"

int main()
{   int choice = 0;
    char input_filename[256];
    char output_filename[256];
    t_bmp8* img_8 = NULL;
    t_bmp24* img_24 = NULL;
    int img_depth = 0; //Used to track file type - to apply relevant functions

    while (1)
    {
        printf("Choose action :\n");
        printf("1: Open an image\n");
        printf("2: Save an image\n");
        printf("3: Apply a filter\n");
        printf("4: Display image information\n");
        printf("5: Quit the program\n");
        int scan_result = scanf("%d", &choice);
        int c;
        while ((c = getchar()) != '\n' && c != EOF){} // Emptying the buffer to avoid infinite loop
        if (scan_result != 1)
        {
            printf("Error - please enter a valid number\n");
            continue; // Skipping iteration to get new input
        }
        switch (choice)
        {
        case 1 :
            {
                printf("Enter file name or path (of type bmp8 or bmp24): \n");
                scanf("%255s", input_filename);//Length limit on the string to avoid overflow on filename
                img_depth = find_colorDepth(input_filename);
                if (img_8 != NULL) bmp8_free(img_8); //Emptying in case of double load
                if (img_24 != NULL) bmp24_free(img_24);
                if (img_depth == 8)
                {
                    img_8 = bmp8_loadImage(input_filename);
                    if (img_8 != NULL) printf("Image loaded successfully!\n");
                    break;
                }
                if (img_depth == 24)
                {
                    img_24 = bmp24_loadImage(input_filename);
                    if (img_24 != NULL) printf("Image loaded successfully!\n");
                    break;
                }
                printf("Error loading image - verify file type or color depth\n");
                break;
            }
        case 2:
            {
                printf("Enter file name or path (of type bmp8 or bmp24): \n");
                scanf("%255s", output_filename);
                if (img_depth == 8)
                {
                    bmp8_saveImage(output_filename, img_8);
                    break;
                }
                if (img_depth == 24)
                {
                    bmp24_saveImage(img_24, output_filename);
                    break;
                }
                printf("Color depth not accepted (expected 8 or 24) - verify file type\n");
                break;
            }
        case 3 :
            {
                int run_filter_loop = 1;

                while (run_filter_loop)
                {
                    int kernelSize = 3;
                    int choice2 = 0;

                    printf("Choose filter :\n");
                    printf("1: Negative\n");
                    printf("2: Luminosity\n");
                    printf("3: Thresholding/grayscaling\n");
                    printf("4: blur\n");
                    printf("5: Gaussian blur\n");
                    printf("6: Sharpen\n");
                    printf("7: Edge detection\n");
                    printf("8: Emboss\n");
                    printf("9: Previous menu\n");
                    scan_result = scanf("%d", &choice2);
                    int c;
                    while ((c = getchar()) != '\n' && c != EOF){} // Emptying the buffer to avoid infinite loop
                    if (scan_result != 1)
                    {
                        printf("Error - please enter a valid number\n");
                        continue; // Skipping iteration to get new input
                    }

                    switch (choice2)
                    {
                    case 1:
                        {
                            if (img_depth == 8)
                            {
                                bmp8_negative(img_8);
                                printf("Negative filter applied\n");
                                break;
                            }
                            if (img_depth == 24)
                            {
                                bmp24_negative(img_24);
                                printf("Negative filter applied\n");
                                break;
                            }
                            printf("Color depth not accepted (expected 8 or 24) - verify file type\n");
                            break;
                        }
                    case 2:
                        {
                            printf("Enter luminosity value: ");
                            int luminosity = 0;
                            scan_result = scanf("%d", &luminosity);
                            int c;
                            while ((c = getchar()) != '\n' && c != EOF); // Emptying the buffer to avoid infinite loop
                            if (scan_result != 1)
                            {
                                printf("Error - please enter a valid number");
                                continue; // Skipping iteration to get new input
                            }
                            if (img_depth == 8)
                            {
                                bmp8_brightness(img_8, luminosity);
                                printf("Luminosity filter applied\n");
                                break;
                            }
                            if (img_depth == 24)
                            {
                                bmp24_brightness(img_24, luminosity);
                                printf("Luminosity filter applied\n");
                                break;
                            }
                            printf("Color depth not accepted (expected 8 or 24) - verify file type\n");
                            break;
                        }
                    case 3:
                        {
                            if (img_depth == 8)
                            {
                                printf("BMP8 recognized, please enter threshold value: ");
                                int threshold = 0;
                                scan_result = scanf("%d", &threshold);
                                int c;
                                while ((c = getchar()) != '\n' && c != EOF); // Emptying the buffer to avoid infinite loop
                                if (scan_result != 1)
                                {
                                    printf("Error - please enter a valid number\n");
                                    continue; // Skipping iteration to get new input
                                }
                                bmp8_threshold(img_8, threshold);
                                printf("Threshold filter applied\n");
                                break;
                            }
                            if (img_depth == 24) // Add grayscale
                            {
                                printf("BMP24 recognized, applying grayscale filter\n");
                                bmp24_grayscale(img_24);
                                break;
                            }
                            printf("Color depth not accepted - verify file type\n");
                            break;
                        }
                    case 4:
                        {
                            printf("Applying blur\n");
                            float ** kernel = NULL;
                            int allocation_success = allocate_kernel(&kernel, kernelSize);
                            if (!allocation_success) break;
                            for (int i=0; i<kernelSize; i++) // Creation of blur kernel
                            {
                                for (int j=0; j<kernelSize; j++)
                                {
                                    kernel[i][j] = (float)1/(float)(kernelSize * kernelSize);
                                }
                            }
                            filter_handling(img_8, img_24, kernel, kernelSize, img_depth);
                            free_kernel(kernel, kernelSize);
                            break;
                        }
                    case 5:
                        {
                            printf("Applying Gaussian blur\n");
                            float ** kernel = NULL;
                            int allocation_success = allocate_kernel(&kernel, kernelSize);
                            if (!allocation_success) break;
                            for (int i=0; i<kernelSize; i++) // Gaussian blur - formula not generalized!
                            {
                                for (int j=0; j<kernelSize; j++)
                                {
                                    if ((i+j) % 2 ==0 )
                                    {
                                        kernel[i][j] = (float)1/16;
                                    }
                                    else
                                    {
                                        kernel[i][j] = (float)2/16;
                                    }
                                }
                            }
                            kernel[kernelSize/2][kernelSize/2] = (float)4/16;//Setting the central value
                            filter_handling(img_8, img_24, kernel, kernelSize, img_depth);
                            free_kernel(kernel, kernelSize);
                            break;
                        }
                    case 6:
                        {
                            printf("Applying sharpen filter\n");
                            float ** kernel = NULL;
                            int allocation_success = allocate_kernel(&kernel, kernelSize);
                            if (!allocation_success) break;
                            for (int i=0; i<kernelSize; i++) // Sharpen - formula not generalized!
                            {
                                for (int j=0; j<kernelSize; j++)
                                {
                                    if ((i+j) % 2 ==0 )
                                    {
                                        kernel[i][j] = (float)0;
                                    }
                                    else
                                    {
                                        kernel[i][j] = (float)-1;
                                    }
                                }
                            }
                            kernel[kernelSize/2][kernelSize/2] = (float)5;//Setting the central value
                            filter_handling(img_8, img_24, kernel, kernelSize, img_depth);
                            free_kernel(kernel, kernelSize);
                            break;
                        }
                    case 7:
                        {
                            printf("Applying outline filter\n");
                            float ** kernel = NULL;
                            int allocation_success = allocate_kernel(&kernel, kernelSize);
                            if (!allocation_success) break;
                            for (int i=0; i<kernelSize; i++)
                            {
                                for (int j=0; j<kernelSize; j++)
                                {
                                    kernel[i][j] = (float)-1;
                                }
                            }
                            kernel[kernelSize/2][kernelSize/2] = (float)8;//Setting the central value
                            filter_handling(img_8, img_24, kernel, kernelSize, img_depth);
                            free_kernel(kernel, kernelSize);
                            break;
                        }
                    case 8:
                        {
                            printf("Applying emboss\n");
                            if (kernelSize != 3 )
                            {
                                printf("This filter functions only for kernelSize of 3");
                                break;
                            }
                            float ** kernel = NULL;
                            int allocation_success = allocate_kernel(&kernel, kernelSize);
                            if (!allocation_success) break;

                            float values[9] = { // Functions for kernel size 3
                                -2.0f, -1.0f, 0.0f,
                                -1.0f, 1.0f, 1.0f,
                                0.0f, 1.0f, 2.0f
                            };
                            for (int i=0; i<kernelSize; i++)
                            {
                                for (int j=0; j<kernelSize; j++)
                                {
                                    kernel[i][j] = values[i*kernelSize + j]; // Calculating based on location
                                }
                            }
                            filter_handling(img_8, img_24, kernel, kernelSize, img_depth);
                            free_kernel(kernel, kernelSize);
                            break;
                        }
                    case 9:
                        {
                            printf("Returning to previous menu\n");
                            run_filter_loop = 0;
                            break;
                        }
                    default :
                        {
                            printf("Invalid filter choice, please choose again\n");
                        }
                    }
                    }
                break;
            }
        case 4 :
            {
                if (img_depth == 8)
                {
                    bmp8_printInfo(img_8);
                    break;
                }
                if (img_depth == 24)
                {
                    bmp24_printInfo(img_24);
                }
                break;
            }
        case 5 :
            {
                printf("Quitting program\n");
                if (img_8 != NULL) bmp8_free(img_8);
                if (img_24 != NULL) bmp24_free(img_24);
                return 0;
            }
        default :
            {
                printf("Invalid choice, please try again\n");
            }
        }
    }

}
