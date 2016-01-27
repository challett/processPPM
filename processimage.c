
#include "a1.h"
#include <stdio.h>

void processImage(int width, int height, RGB *image, int N, int offset)
{
  // simple image "processing"
  int i;

  for (i=offset; i < width*height; i++)
    {
      RGB *p = image+i;
      p->r = p->g%128;
    }
}
