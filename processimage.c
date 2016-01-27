
#include "a1.h"
#include <stdio.h>

RGB * processImage(int width, int height, RGB *image, int N, int offset)
{
    RGB *processedImage;
    processedImage = malloc(sizeof(RGB) * width * height);
    
  // simple image "processing"
  int i;

  for (i=0; i < width*height; i++)
    {
      RGB *p = image+i+offset;
      (processedImage+i)->r = p->g%128;
        (processedImage+i)->b = p->b;
        (processedImage+i)->g = p->g;
    }
    return processedImage;
}
