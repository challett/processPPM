
#include "a1.h"
#include <stdio.h>

RGB * processImage(int width, int height, RGB *image, int N, int offset)
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
        
        if (offset) {
            for (k=0; k <= N/2; k++)
            {
                for (j=0; j<= N/2; j++)
                {
                    //if the pixel is on the same row as the pixel after it when going left
                    if ((i+offset-j) / width == (i+offset-j+1) / width)
                    {
                        window[windowSize] = *(image+i+offset-j + k*width);
                        windowSize++;
                    }
                    //if the pixel is on the same row as the pixel before it when going right
                    if ((i+offset+j) / width == (i+offset+j-1) / width)
                    {
                        window[windowSize] = *(image+i+offset+j - k*width);
                        windowSize++;
                    }
                    
                }
            }
            
        } else {
            for (k=0; k <= N/2; k++)
            {
                if (k <= i/width)
                //if we have the row
                {
                    for (j=0; j<= N/2; j++)
                    {
                        //if the pixel is on the same row as the pixel after it when going left
                        if ((i+offset-j) / width == (i+offset-j+1) / width)
                        {
                            window[windowSize] = *(image+i+offset-j + k*width);
                            windowSize++;
                        }
                        //if the pixel is on the same row as the pixel before it when going right
                        if ((i+offset+j) / width == (i+offset+j-1) / width)
                        {
                            window[windowSize] = *(image+i+offset+j - k*width);
                            windowSize++;
                        }
                        
                    }
                }
                
            }
        }
        averageBlue = 0;
        averageGreen = 0;
        averageRed = 0;
        for(k=0;k<windowSize;k++){
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
    }
    return processedImage;
}
