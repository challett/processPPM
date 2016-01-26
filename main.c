#include <stdlib.h>
#include <stdio.h>
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
  // Find how many rows per process.
  int* rows  = (int*)malloc(sizeof(int)*p);

  MPI_Status status;

  if (my_rank == 0) {
    //read whole file
    image = readPPM(argv[1], &width, &height, &max);
    for (i=0; i < p; i++) {
      rows[i] = height / p;
    }
    for (i=0; i < height % p; i++)
      rows[i]++;
  }


  MPI_Bcast(rows, p, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&width, 1, MPI_INT, 0, MPI_COMM_WORLD);
  printf("size of rgb is %d  size of double is %d\n", sizeof(RGB), sizeof(double));
  fflush(stdout);
  if (my_rank == 0){
    for (dest = 1; dest < p; dest++){
      offset += rows[dest-1];
      printf("offset is %d rows \n", offset);
      fflush(stdout);

      //send to other processes the part that they need
      if (dest < p - 1) {
        printf("sending %d rows to %d \n", rows[dest], dest);
        fflush(stdout);
        MPI_Send(image + offset * width - (N/2) * width , sizeof(RGB) * (rows[dest] * width + (N-1) * width), MPI_CHAR, dest, tag, MPI_COMM_WORLD);
        printf("sent %d rows to %d \n", rows[dest], dest);
        fflush(stdout);
      } else {
        printf("sending %d rows to %d (else branch)\n", rows[dest], dest);
        fflush(stdout);
       MPI_Send(image + offset * width - (N/2) * width , sizeof(RGB) * (rows[dest] * width + (N/2) * width), MPI_CHAR, dest, tag, MPI_COMM_WORLD);
       printf("sent %d rows to %d  (else branch) \n", rows[dest], dest);
       fflush(stdout);
      }

    }
    printf("senteverything by %d \n", my_rank);
    fflush(stdout);
    processImage(width, rows[0], image, N, 0);


  } else {
    //receive part to work on
    imagePortion = (RGB*)malloc(sizeof(RGB) * (rows[my_rank] * width + (N-1) * width));
    assert(image);
    if (my_rank < p - 1) { //last process doesn't get the bottom rows
      printf("asking for %d rows for %d \n", rows[my_rank], my_rank);
      fflush(stdout);

      MPI_Recv(imagePortion, sizeof(RGB) * (rows[my_rank] * width + (N-1) * width), MPI_CHAR, 0, tag, MPI_COMM_WORLD, &status);
      printf("received %d rows for %d \n", rows[my_rank], my_rank);
      fflush(stdout);
    } else {
      printf("asking for %d rows for %d (else branch)\n", rows[my_rank], my_rank);
      fflush(stdout);

      MPI_Recv(imagePortion, sizeof(RGB) * (rows[my_rank] * width + (N/2) * width), MPI_CHAR, 0, tag, MPI_COMM_WORLD, &status);
      printf("received %d rows for %d \n", rows[my_rank], my_rank);
      fflush(stdout);
    }

    processImage(width, rows[my_rank], image, N, 0);
    printf("%d done processing \n", my_rank);
    fflush(stdout);
  }

  if (my_rank == 0) {
    //receive filtered parts
    offset = 0;
    for (dest = 1; dest < p; dest++){
      offset += rows[dest-1] ;
     MPI_Recv(image + offset * width, sizeof(RGB) * (rows[dest] * width), MPI_CHAR, dest, tag, MPI_COMM_WORLD, &status);
    }

    //write final file
    writePPM(argv[2], width, height, max, image);
  } else {
   MPI_Send(imagePortion + (N/2) * width, sizeof(RGB) * (rows[my_rank] * width), MPI_BYTE, 0, tag, MPI_COMM_WORLD);
  }

  free(image);
  free(rows);

  MPI_Finalize();
  return(0);
}
