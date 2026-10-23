#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <mpi.h>

// my block: n elements of a global vector gx, gx[i] = i,
// N = n * size elements in all, gx[k*n] first on process k
void init(float **x, int *n)
{
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  *n = 3;
  *x = (float *)malloc(*n * sizeof(float));
  for (int j = 0; j < *n; j++) (*x)[j] = rank * (*n) + j;
}

int main(int argc, char **argv)
{
  MPI_Init(&argc, &argv);
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  float *x; int n; // my block of the global vector
  init(&x, &n);    // its n elements, see init above

  // step 1: the sum of the squares of my block
  float s = 0;
  for (int j = 0; j < n; j++) s = s + x[j] * x[j];

  // step 2: process 0 adds up the sums of the others
  if (rank == 0) {
    for (int k = 1; k < size; k++) {
      float t;
      MPI_Recv(&t, 1, MPI_FLOAT, k, 0, MPI_COMM_WORLD,
               MPI_STATUS_IGNORE);
      s = s + t;
    }
  } else {
    MPI_Send(&s, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
  }

  // step 3: process 0 computes the norm, sends it out
  float norm;
  if (rank == 0) {
    norm = sqrt(s);
    for (int k = 1; k < size; k++)
      MPI_Send(&norm, 1, MPI_FLOAT, k, 0, MPI_COMM_WORLD);
  } else {
    MPI_Recv(&norm, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
  }

  // step 4: every process normalizes its own block
  for (int j = 0; j < n; j++) x[j] = x[j] / norm;
  printf("process %d: norm = %.2f, x =", rank, norm);
  for (int j = 0; j < n; j++) printf(" %.2f", x[j]);
  printf("\n");

  free(x);
  MPI_Finalize();
  return 0;
}
