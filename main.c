#include <stdlib.h>
#include "a1.h"

int main(int argc, char** argv)
{
  RGB *image;
  int width, height, max;
  
  image = readPPM(argv[1], &width, &height, &max);
  
  processImage(width, height, image);
  
  writePPM(argv[2], width, height, max, image);
  
  free(image);
  return(0);
}
