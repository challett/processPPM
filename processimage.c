
#include "a1.h"
#include <stdio.h>

void processImage(int width, int height, RGB *image)
{
  // simple image "processing"
  int i;

  for (i=0; i < width*height; i++)
    {
      RGB *p = image+i;
      printf(" %d ", p->g);
      p->r = p->g%128;
    }
}
