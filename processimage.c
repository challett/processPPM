
#include "a1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

RGB * processImage(int width, int height, RGB *image, int N, int offset, char *type)
{
    RGB *processedImage;
    processedImage = (RGB*)malloc(sizeof(RGB) * width * height);
    RGB window [N*N];
    int j;
    int k;
    int column;
    int averageRed;
    int averageGreen;
    int averageBlue;
  // simple image "processing"
  int i;

  for (i=0; i < width*height; i++)
    {
        RGB *p = image+i+offset;
        window[0] = *p;
        int windowSize = 1;
        column = i % width;

        if (offset)
        {
            for (k=0; k <= N/2; k++)
            {
                for (j=0; j<= N/2; j++)
                {
                    if ((i+offset) / width == (i+offset-j) / width)
                    {//if the pixel is on the same row as the pixel after it when going left
                        window[windowSize] = *(image+i+offset-j + k*width);
                        windowSize++;
                    }
                    if ((i+offset) / width == (i+offset+j) / width)
                    {//if the pixel is on the same row as the pixel before it when going right
                        window[windowSize] = *(image+i+offset+j - k*width);
                        windowSize++;
                    }

                }
            }

        } else
        {
            for (k=0; k <= N/2; k++)
            {
                if (k <= i/width)
                {//if we have the row
                    for (j=0; j<= N/2; j++)
                    {
                        if (i>j && ((i+offset) / width == (i+offset-j) / width))
                        {//if the pixel is on the same row as the pixel after it when going left
                            window[windowSize] = *(image+i+offset-j + k*width);
                            windowSize++;
                        }
                        if ((i+offset) / width == (i+offset+j) / width)
                        {//if the pixel is on the same row as the pixel before it when going right
                            window[windowSize] = *(image+i+offset+j - k*width);
                            windowSize++;
                        }

                    }
                }

            }
        }
        if (strchr(type,'A'))
        {
            averageBlue = 0;
            averageGreen = 0;
            averageRed = 0;
            for(k=0;k<windowSize;k++)
            {
                averageBlue += window[k].b;
                averageRed += window[k].r;
                averageGreen += window[k].g;
            }
            averageGreen = averageGreen/windowSize;
            averageRed = averageRed/windowSize;
            averageBlue = averageBlue/windowSize;

            (processedImage+i)->r = averageRed;
            (processedImage+i)->b = averageBlue;
            (processedImage+i)->g = averageGreen;
        } else if (strchr(type, 'M'))
        {
            int redArray[windowSize], greenArray[windowSize], blueArray[windowSize];

            for(k=0;k<windowSize;k++)
            {
                redArray[k] = (int)(window[k].r);
                greenArray[k] = (int)window[k].g;
                blueArray[k] = (int)window[k].b;
            }

            qsort(&redArray, windowSize, sizeof(int), cmpfunc);
            qsort(&greenArray, windowSize, sizeof(int), cmpfunc);
            qsort(&blueArray, windowSize, sizeof(int), cmpfunc);

            (processedImage+i)->r = redArray[windowSize/2];
            (processedImage+i)->b = blueArray[windowSize/2];
            (processedImage+i)->g = greenArray[windowSize/2];
        }

    }
    return processedImage;
}

int cmpfunc (const void * a, const void * b)
{
    return ( *(int*)a - *(int*)b );
}
