
#include "a1.h"

void processImage(int width, int height, RGB *image)
{
  // simple image "processing"
  int i;

  for (i=0; i < width*height; i++)
    {
      RGB *p = image+i;
      p->r = p->g%128;
    }
}
