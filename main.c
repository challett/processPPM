#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "a1.h"

#include <mpi.h>

int main(int argc, char** argv)
{
  int width, height, max;
  RGB *image;
  RGB *imagePortion;

  int my_rank, p;
  int i, dest, source;
  int offset = 0;
  int tag = 0;
  int N = atoi(argv[3]);

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  int* rows  = (int*)malloc(sizeof(int)*p);

  MPI_Status status;

  if (my_rank == 0)
  {//read whole file
    image = readPPM(argv[1], &width, &height, &max);
    for (i=0; i < p; i++)
    {
        rows[i] = height / p + (i<height%p ? 1 : 0);
    }
  }


  MPI_Bcast(rows, p, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&width, 1, MPI_INT, 0, MPI_COMM_WORLD);

  if (my_rank == 0)
  {
    for (dest = 1; dest < p; dest++)
    {
      offset += rows[dest-1];

      //send to other processes the part that they need
      if (dest < p - 1)
      {
        MPI_Send(image + offset * width - (N/2) * width , sizeof(RGB) * (rows[dest] * width + (N-1) * width), MPI_CHAR, dest, tag, MPI_COMM_WORLD);
      } else
      {
        MPI_Send(image + offset * width - (N/2) * width , sizeof(RGB) * (rows[dest] * width + (N/2) * width), MPI_CHAR, dest, tag, MPI_COMM_WORLD);
      }

    }
    imagePortion = (RGB*)malloc(sizeof(RGB) * (rows[my_rank] * width + (N-1) * width));
    imagePortion = processImage(width, rows[0], image, N, 0, argv[4]);


  } else {
    //receive part to work on
    imagePortion = (RGB*)malloc(sizeof(RGB) * (rows[my_rank] * width + (N-1) * width));
    assert(imagePortion);
    if (my_rank < p - 1)
    { //last process doesn't get the bottom rows
      MPI_Recv(imagePortion, sizeof(RGB) * (rows[my_rank] * width + (N-1) * width), MPI_CHAR, 0, tag, MPI_COMM_WORLD, &status);
    } else
    {
      MPI_Recv(imagePortion, sizeof(RGB) * (rows[my_rank] * width + (N/2) * width), MPI_CHAR, 0, tag, MPI_COMM_WORLD, &status);
    }

    imagePortion = processImage(width, rows[my_rank], imagePortion, N, (N/2) * width, argv[4]);
  }

  if (my_rank == 0)
  {//receive filtered parts
    offset = 0;
      memcpy(image, imagePortion, sizeof(RGB) * (rows[my_rank] * width));
    for (source = 1; source < p; source++){
      offset += rows[source-1] ;
      MPI_Recv(image + offset * width, sizeof(RGB) * (rows[source] * width), MPI_CHAR, source, tag, MPI_COMM_WORLD, &status);
    }

    //write final file
    writePPM(argv[2], width, height, max, image);
      free(image);
  } else
  {
      MPI_Send(imagePortion, sizeof(RGB) * (rows[my_rank] * width), MPI_CHAR, 0, tag, MPI_COMM_WORLD);
  }

    free(imagePortion);

  free(rows);

  MPI_Finalize();
  return(0);
}
