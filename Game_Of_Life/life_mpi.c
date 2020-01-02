#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

double Walltime(void);
void InitGrid(int, int, int, int **, int **);
void DumpLocalGrid(int **, int, int);
void DumpGrid(int **, int, int);
void OneLife(int**, int**, int, int);
int Correct(int, int, int, int **);

int main(int argc, char **argv) {
  int i, corr;
  int** gridIn;
  int** gridOut;
  int next, prev;
  int ranks, rank;
  int mySize, myOffset;
  double t0, t1, t2, t3;

  MPI_Request request[4];
  MPI_Status status[4];

  int N = 1024; // default grid size
  for (i = 0; i < argc; i++) {
    if ((strcmp(argv[i], "-e") == 0) || (strcmp(argv[i], "-edge") == 0)) {
      N = 1 << atoi(argv[++i]);
    }
  }

  // number of MPI ranks, my rank

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &ranks);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);


  if (ranks > N) {
    printf("Too many MPI ranks (%d)! \n", ranks);
    MPI_Finalize();
    exit(0);
  }

  if (rank == 0) printf("Execution with %d MPI ranks\n", ranks);

  // neighbors

  if (rank == 0) prev = MPI_PROC_NULL;
  else prev = rank - 1;
  if (rank == ranks - 1) next = MPI_PROC_NULL;
  else next = rank + 1;

  // offset

  myOffset = rank*(N/ranks) + 1;
  if (N % ranks != 0) {
    if (rank > (N % ranks)) myOffset += (N % ranks);
    else myOffset += rank;
  }

// memory allocation

  t0 = Walltime();
  mySize = N/ranks + ((rank < (N % ranks)) ? 1 : 0);
  gridIn = (int**)malloc(sizeof(int*)*(mySize + 2));
  gridOut = (int**)malloc(sizeof(int*)*(mySize + 2));
  for (i = 0; i < (mySize + 2); i++) {
    gridIn[i] = (int*)malloc(sizeof(int)*(N + 2));
    gridOut[i] = (int*)malloc(sizeof(int)*(N + 2));
  }

  InitGrid(mySize, myOffset, N, gridIn, gridOut);

  t1 = Walltime();
  for (i = 0; i < 4*(N - 3); i++) {
    if (i % 2 == 0) {
      OneLife(gridIn, gridOut, mySize, N);
      MPI_Isend(&gridOut[1][0], N + 2, MPI_INT, prev, 0, MPI_COMM_WORLD, request);
      MPI_Irecv(&gridOut[0][0], N + 2, MPI_INT, prev, 0, MPI_COMM_WORLD, request + 1);
      MPI_Isend(&gridOut[mySize][0], N + 2, MPI_INT, next, 0, MPI_COMM_WORLD, request + 2);
      MPI_Irecv(&gridOut[mySize+1][0], N + 2, MPI_INT, next, 0, MPI_COMM_WORLD, request + 3);
      MPI_Waitall(4, request, status);
    }
    else {
      OneLife(gridOut, gridIn, mySize, N);
      MPI_Isend(&gridIn[1][0], N + 2, MPI_INT, prev, 0, MPI_COMM_WORLD, request);
      MPI_Irecv(&gridIn[0][0], N + 2, MPI_INT, prev, 0, MPI_COMM_WORLD, request + 1);
      MPI_Isend(&gridIn[mySize][0], N + 2, MPI_INT, next, 0, MPI_COMM_WORLD, request + 2);
      MPI_Irecv(&gridIn[mySize+1][0], N + 2, MPI_INT, next, 0, MPI_COMM_WORLD, request + 3);
      MPI_Waitall(4, request, status);
    }
  }
  t2 = Walltime();
  corr = Correct(mySize, myOffset, N, gridIn);
  if (rank == 0) {
    if (corr == ranks) printf("**CORRECT RESULT**\n");
    else printf("**WRONG RESULT**\n");
  }
  t3 = Walltime();
  
  if (rank == 0) {
    printf("N=%d; times: init=%f, comp=%f, end=%f, tot=%f \n", N, t1-t0, t2-t1, t3-t2, t3-t0);
  }

  for (i = 0; i < (mySize + 2); i++) {
    free(gridIn[i]);
    free(gridOut[i]);
  }
  free(gridIn);
  free(gridOut);

  MPI_Finalize();
  exit(0);
}
