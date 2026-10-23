// =====================================================================
//  Lab 4 - Distributed-memory programming with MPI
//  Exercise 2: 1-D convolution on a distributed vector  --  SKELETON
//
//  Fill in the parts marked  TODO  in the order of the questions.
//
//  Compile, then run with 4 processes:
//    mpic++ -O2 convolution.cpp -o convolution
//    mpirun -np 4 ./convolution
// =====================================================================

// =====================================================================
//  ANSWERS (as comments; this file is the only thing you submit)
//
//  (a) Which process holds gx[i], and in which cell of its x? Which
//      element of gx is the x[j] of process k? Which processes are the
//      left and right neighbours of process k (and of processes 0 and
//      size-1)?
//
//  (e) With -np 1, 2, 4 and 8: does verify say OK every time? Which
//      elements of y depend on another process's block, and how many
//      messages does each process send in all? Why two rounds (a send,
//      a receive, a send, a receive) rather than the two sends and then
//      the two receives: what would go wrong with -np 2?
//
// =====================================================================

#include <cstdio>
#include <cmath>
#include <mpi.h>

const int n = 4; // elements per process; the global vector has N = n * size

// ---------------------------------------------------------------------
//  (provided) The global vector gx[0..N-1] is given by a formula, so that
//  every process can fill its own block without any communication:
//  process k holds gx[k*n], ..., gx[k*n + n - 1] in x[0], ..., x[n-1].
// ---------------------------------------------------------------------
float gx(int i) { return (float)(i * i); }

// ---------------------------------------------------------------------
//  (provided) Checks my block of the result: y[j] must be the average of
//  gx[i-1], gx[i] and gx[i+1], where i is the global index of x[j]. The
//  vector is circular: the left neighbour of gx[0] is gx[N-1] and the
//  right neighbour of gx[N-1] is gx[0].
// ---------------------------------------------------------------------
void verify(const float *y, int n, int rank, int size)
{
  int N = n * size;
  for (int j = 0; j < n; j++) {
    int i = rank * n + j;
    float expected = (gx((i - 1 + N) % N) + gx(i) + gx((i + 1) % N)) / 3;
    if (fabs(y[j] - expected) > 1e-3 * (1 + fabs(expected))) {
      printf("process %d: ERROR, y[%d] = %g, expected %g\n",
             rank, j, y[j], expected);
      return;
    }
  }
  printf("process %d: OK\n", rank);
}

int main(int argc, char **argv)
{
  MPI_Init(&argc, &argv);
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  // (provided) my block of the global vector, and room for my block of y
  float x[n], y[n];
  for (int j = 0; j < n; j++) x[j] = gx(rank * n + j);

  // -------------------------------------------------------------------
  //  Question (b): the ranks of my left and right neighbours. The vector
  //  is circular: the left neighbour of process 0 is process size-1 and
  //  the right neighbour of process size-1 is process 0.
  // -------------------------------------------------------------------
  int left = 0;  // TODO (b)
  int right = 0; // TODO (b)

  // -------------------------------------------------------------------
  //  Question (c): the frontier elements, in two rounds.
  //  Round 1: send x[n-1] to the right neighbour, then receive xleft, the
  //           last element of the left neighbour's block (its x[n-1]).
  //  Round 2: send x[0] to the left neighbour, then receive xright, the
  //           first element of the right neighbour's block (its x[0]).
  // -------------------------------------------------------------------
  float xleft = 0, xright = 0;
  // TODO (c)

  // -------------------------------------------------------------------
  //  Question (d): the three-point average,
  //      y[j] = (x[j-1] + x[j] + x[j+1]) / 3,
  //  with xleft in place of x[-1] and xright in place of x[n].
  // -------------------------------------------------------------------
  // TODO (d)

  // (provided) print my blocks of x and y, and check y
  printf("process %d: x =", rank);
  for (int j = 0; j < n; j++) printf(" %g", x[j]);
  printf(", y =");
  for (int j = 0; j < n; j++) printf(" %g", y[j]);
  printf("\n");
  verify(y, n, rank, size);

  MPI_Finalize();
  return 0;
}
