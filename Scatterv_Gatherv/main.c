#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#define SIZE 4

int main(int argc, char** argv) {

  int size, rank;

  int data[SIZE*SIZE] =
  {  1,  2,  3,  4,
     5,  6,  7,  8,
     9, 10, 11, 12,
    13, 14, 15, 16  };

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int* counts = (int*)malloc(size*sizeof(int));
  int* displs = (int*)malloc(size*sizeof(int));
  int* ans = NULL;

  int sum = 0;
  int remainder = (SIZE*SIZE)%size;

  for (int i = 0; i < size; i++) {
    counts[i] = (SIZE*SIZE)/size;
    if (remainder > 0) {
      counts[i]++;
      remainder--;
    }
    displs[i] = sum;
    sum += counts[i];
  }

  if (rank == 0) {
    for (int i = 0; i < size; i++) {
      printf("counts[%d] = %d\tdispls[%d] = %d\n", i, counts[i], i, displs[i]);
    }
  }

  int* recvbuf = (int*)malloc(((SIZE*SIZE)/size + 1)*sizeof(int));

  MPI_Scatterv(&data, counts, displs, MPI_INT, recvbuf, counts[rank], MPI_INT, 0, MPI_COMM_WORLD);
  
  // DEBUG
  /**
   *  if (rank == (size - 1)) {
   *    printf("%d: ", rank);
   *    for (int i = 0; i < counts[rank]; i++) {
   *      printf("%d\t", recvbuf[i]);
   *    }
   *    printf("\n");
   *  }  
   */

  if (rank == 0) ans = (int*)malloc((SIZE*SIZE)*sizeof(int));

  MPI_Gatherv(recvbuf, counts[rank], MPI_INT, ans, counts, displs, MPI_INT, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    for (int i = 0; i < SIZE*SIZE; i++) {
      printf("%d\t", ans[i]);
    }
    printf("\n");
  }

  MPI_Finalize();
  free(counts);
  free(displs);
  free(recvbuf);
  free(ans);

  return 0;
}
