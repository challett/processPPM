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
  int offset = 0;
  int tag = 0;
  int N = atoi(argv[3]);

  // Find how many rows per process.
  int *rows;

  rows = (int*)malloc(sizeof(int)*p);
  for (i=0; i < p; i++)
    rows[i] = height / p;
  for (i=0; i < height % p; i++)
    rows[i]++;

  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  if (my_rank == 0) {
    //read whole file
    image = readPPM(argv[1], &width, &height, &max);

    for (dest = 1; dest < p; dest++){
      offset += rows[dest-1] * width;
      //send to other processes the part that they need
      if (dest < p - 1) {
        MPI_Send(image + offset - (N/2) * width , rows[dest - 1] * width + (N-1) * width, MPI_DOUBLE, dest, tag, MPI_COMM_WORLD);
      } else {
        MPI_Send(image + offset - (N/2) * width , rows[dest - 1] * width + (N/2) * width, MPI_DOUBLE, dest, tag, MPI_COMM_WORLD);
      }

    }

    processImage(width, rows[0] * width, image);


  } else {
    //receive part to work on
    if (my_rank == p-1) { //last process doesn't get the bottom rows
      MPI_Recv(image, rows[my_rank] * width + (N/2) * width, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
    } else {
      MPI_Recv(image, rows[my_rank] * width + (N-1) * width, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
    }
    processImage(width, rows[my_rank] * width, image);
  }

  //TODO: Send results to process 0 and combine
  if (my_rank == 0) {
    //receive filtered parts
    MPI_Recv(image, rows[my_rank]*n, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);

    //write final file
    writePPM(argv[2], width, height, max, image);
  } else {
    MPI_Send()
  }

  free(image);
  printf("Hello this is thread: %d", my_rank);

  MPI_Finalize();
  return(0);
}
