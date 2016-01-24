#include <stdlib.h>
#include <stdio.h>
#include "a1.h"

#include <mpi.h>

int main(int argc, char** argv)
{
  RGB *image;
  int width, height, max;

  int my_rank, p;
  int i, dest, source;
  int offset;

  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  if (my_rank == 0) {
    image = readPPM(argv[1], &width, &height, &max);

    processImage(width, height, image);

    writePPM(argv[2], width, height, max, image);

    free(image);
  }
  printf("Hello this is thread: %d", my_rank);

  MPI_Finalize();
  return(0);
}
