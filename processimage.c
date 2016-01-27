
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
                    if ((i+offset-j) / width == (i+offset-j+1) / width)
                    {//if the pixel is on the same row as the pixel after it when going left
                        window[windowSize] = *(image+i+offset-j + k*width);
                        windowSize++;
                    }
                    if ((i+offset+j) / width == (i+offset+j-1) / width)
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
                        if ((i+offset-j) / width == (i+offset-j+1) / width)
                        {//if the pixel is on the same row as the pixel after it when going left
                            window[windowSize] = *(image+i+offset-j + k*width);
                            windowSize++;
                        }
                        if ((i+offset+j) / width == (i+offset+j-1) / width)
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
                redArray[k] = window[k].r;
                greenArray[k] = window[k].g;
                blueArray[k] = window[k].b;
            }
            
            (processedImage+i)->r = select(*redArray , 0, windowSize-1 , windowSize/2);
            (processedImage+i)->b = select(*blueArray , 0, windowSize-1 , windowSize/2);
            (processedImage+i)->g = select(*greenArray , 0, windowSize-1 , windowSize/2);
        }
        
    }
    return processedImage;
}

// selects the median of medians in an array
int select(int *a, int s, int e, int k){
    
    if(e-s+1 <= 5){
        qsort(a+s, a+e, sizeof(int), cmpfunc);
        return s+k-1;
    }
    
    for(int i=0; i<(e+1)/5; i++){
        int left = 5*i;
        int right = left + 4;
        if(right > e) right = e;
        int median = select(a, 5*i, 5*i+4, 3);
        swap(a[median], a[i]);
    }
    return select(a, 0, (e+1)/5, (e+1)/10);
}

int cmpfunc (const void * a, const void * b)
{
    return ( *(int*)a - *(int*)b );
}

void swap(int *i, int *j) {
    int t = *i;
    *i = *j;
    *j = t;
}
