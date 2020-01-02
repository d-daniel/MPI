#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define DATA_TAG 33


void OneLife(int** gridIn, int** gridOut, int size, int N) {
  int i, j, alive;

  for (i = 1; i <= size; i++) {
    for (j = 1; j <= N; j++) {
      alive =
        gridIn[i-1][j-1] +
        gridIn[i-1][j]   +
        gridIn[i-1][j+1] +
        gridIn[i][j-1]   +
        gridIn[i][j+1]   +
        gridIn[i+1][j-1] +
        gridIn[i+1][j]   +
        gridIn[i+1][j+1];

        if (alive < 2 || alive > 3) gridOut[i][j] = 0;
        else if (alive == 3) gridOut[i][j] = 1;
        else gridOut[i][j] = gridIn[i][j];
    }
  }
}


void DumpLocalGrid(int **grid, int size, int N) {
  int i, j;

  for (i = 1; i <= N; i++) printf("=");
  printf("=\n");
  for (i = 0; i < (size + 2); i++) {
    for (j = 1; j <= N; j++) {
      printf("%c", grid[i][j]? 'X' : '.');
    }
    printf("\n");
  }
  for (i = 1; i <= N; i++) printf("=");
  printf("=\n");
}

void DumpGrid(int **grid, int size, int N) {
  int ranks, rank;
  int i, j, p;
  int auxSize;
  int data[N + 2];
  MPI_Status status;

  MPI_Comm_size(MPI_COMM_WORLD, &ranks);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    // print "...===..." first
    for (i = 1; i <= N; i++) printf("=");
    printf("=\n");

    // print local grid of rank 0 without the ghost zones 
    for (i = 1; i <= size; i++) {
      for (j = 1; j <= N; j++) {
        printf("%c", grid[i][j]? 'X' : '.');
      }
      printf("\n");
    }

    // wait for the next MPI rank
    for (p = 1; p < ranks; p++) {
      auxSize = N/ranks + ((p < (N % ranks)) ? 1 : 0);
      for (i = 1; i <= auxSize; i++) {
        MPI_Recv(&data[0], N + 2, MPI_INT, p, DATA_TAG, MPI_COMM_WORLD, &status);
        for (j = 1; j <= N; j++) {
          printf("%c", data[j]? 'X' : '.');
        }
        printf("\n");
      }
    }

    // print "...===..." at the end
    for (i = 1; i <= N; i++) printf("=");
    printf("=\n");
  }
  else {
    for (i = 1; i <= size; i++) {
      MPI_Send(&grid[i][0], N + 2, MPI_INT, 0, DATA_TAG, MPI_COMM_WORLD);
    }
  }
}

void InitGrid(int size, int offset, int N, int **gridIn, int **gridOut) {
    int i, j;

    // init gridOut and glider on gridIn
    // this is the glider:
    // .....
    // ..X..
    // ...X.
    // .XXX.
    // .....

    for (i = 0; i < (size + 2); i++) {
        for (j = 0; j < (N + 2); j++) {
            if (offset - 1 + i == 1 && j == 2) gridIn[i][j] = 1;
            else if (offset - 1 + i == 2 && j == 3) gridIn[i][j] = 1;
            else if (offset - 1 + i == 3 && (j == 1 || j == 2 || j == 3)) gridIn[i][j] = 1;
            else gridIn[i][j] = 0;
            gridOut[i][j] = 0;
        }
    }
}

// Correct: check the final config of the grid
//          (glider on the bottom right corner)

int Correct(int size, int offset, int N, int **grid) {
    int i, j, r, c;
    int ranks, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &ranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    r = 1;

    for (i = 0; i < (size + 2); i++) {
        for (j = 0; j < (N + 2); j++) {
            if (offset - 1 + i == N - 2 && j == N - 1) r &= (grid[i][j] == 1);
            else if (offset - 1 + i == N - 1 && j == N) r &= (grid[i][j] == 1);
            else if (offset - 1 + i == N && (j == N - 2 || j == N - 1 || j == N)) r &= (grid[i][j] == 1);
            else r &= (grid[i][j] == 0);
        }
    }

    MPI_Reduce(&r, &c, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    return c;
}
