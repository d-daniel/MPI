#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <omp.h>

#include "blowfish.h"
#include "walltime.h"

int main(int argc, char** argv) {

  size_t i;
  uint32_t lhs, rhs;
  uint64_t N, fsize;
  double t0, t1, t2, t3;
  FILE* fp;
  char fname[64];
  char key[56]; // 448 bits key

  uint64_t* p_array = NULL;
  uint64_t* ans = NULL;

  int size, rank;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    if (argc < 4) {
      printf( "  Options:\n" );
      printf( "  -file (-f): name of input file\n" );
      printf( "  -key  (-k): encryption key\n" );
      exit(1);
    }
  }
  else {
    if (argc < 4)
      exit(1);
  }

  for (i = 0; i < argc; i++) {
    if ((strcmp(argv[i], "-f") == 0) || (strcmp(argv[i], "-file") == 0)) {
      strcpy(fname, argv[++i]);
      //printf( "  User file: %s\n", fname);
    }
    else if ((strcmp(argv[i], "-k") == 0) || (strcmp(argv[i], "-key") == 0)) {
      strcpy(key, argv[++i]);
      //printf( "  User %ld bytes key: %s\n", strlen(key), key);
    }
  }

  t0 = walltime();

  fp = fopen(fname, "r");
  fseek(fp, 0L, SEEK_END);
  fsize = ftell(fp);
  rewind(fp);
  
  N = (fsize/8) + ((fsize % 8) == 0 ? 0 : 1);
  if (rank ==0) {
    printf ("File size: %ld bytes split into %ld blocks of 64 bits\n", fsize, N);
#ifdef _OPENMP
    printf("Executing with %d OpenMP threads and %d MPI ranks\n", omp_get_num_procs(), size);
#endif
  }
  
  // TODO: only rank 0 needs to know counts and displs... 
  int* counts = (int*)malloc(size*sizeof(int));
  int* displs = (int*)malloc(size*sizeof(int));

  uint64_t sum = 0;
  uint64_t remainder = N % size;

  for (i = 0; i < size; i++) {
    counts[i] = N/size;
    if (remainder > 0) {
      counts[i]++;
      remainder--;
    }
    displs[i] = sum;
    sum += counts[i];
  }

  // Have rank 0 read the whole file to have the answer
  if (rank == 0) {
    p_array = (uint64_t*)malloc(N*sizeof(uint64_t));
    fread(p_array, 8, N, fp);
    rewind(fp);
  }

  uint64_t* buff = (uint64_t*)malloc(counts[rank]*sizeof(uint64_t));
  // Each rank reads a chunk of the file
  fseek(fp, displs[rank], SEEK_SET);
  fread(buff, 8, counts[rank], fp);
  fclose(fp);
  BLOWFISH_CTX ctx;
  Blowfish_Init(&ctx, (unsigned char*)key, strlen(key));

  t1 = walltime();

  // MPI_Scatterv(p_array, counts, displs, MPI_UINT64_T, buff, counts[rank], MPI_UINT64_T, 0, MPI_COMM_WORLD);

#ifdef _OPENMP
  #pragma omp parallel for private(lhs, rhs) 
#endif
  for (i = 0; i < counts[rank]; i++) {
    lhs = (uint32_t)((buff[i] & 0xFFFFFFFF00000000LL) >> 32);
    rhs = (uint32_t) (buff[i] & 0x00000000FFFFFFFFLL);
    Blowfish_Encrypt(&ctx, &lhs, &rhs);
    Blowfish_Decrypt(&ctx, &lhs, &rhs);
    buff[i] = (((uint64_t)lhs) << 32 | rhs);
  }

  if (rank == 0) ans = (uint64_t*)malloc(N*sizeof(uint64_t));

  MPI_Gatherv(buff, counts[rank], MPI_UINT64_T, ans, counts, displs, MPI_UINT64_T, 0, MPI_COMM_WORLD);

  t2 = walltime();

  if (rank == 0) {
    if (memcmp(p_array, ans, N) == 0)
      printf("PASSED\n");
    else
      printf("FAILED\n");
  }

  free(p_array);
  free(counts);
  free(displs);
  free(buff);
  free(ans);

  t3 = walltime();

  if (rank == 0) {
      printf("  Execution times:\n");
      printf("  Initialization: %fs\n", t1 - t0);
      printf("  Computation:    %fs\n", t2 - t1);
      printf("  Total:          %fs\n", t3 - t0);
  }

  MPI_Finalize();
  return 0;
}
