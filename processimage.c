
#include "a1.h"
#include <stdio.h>

RGB * processImage(int width, int height, RGB *image, int N, int offset)
{
    RGB *processedImage;
    processedImage = (RGB*)malloc(sizeof(RGB) * width * height);
    RGB window [N*N];
  // simple image "processing"
  int i;

  for (i=0; i < width*height; i++)
    {
        RGB *p = image+i+offset;
        window[0] = *p;
        int windowSize = 1;
        
        if (offset) {
            
        } else {
            
        }
        

        (processedImage+i)->r = window[0].g%128;
        (processedImage+i)->b = p->b;
        (processedImage+i)->g = p->g;
    }
    return processedImage;
}
