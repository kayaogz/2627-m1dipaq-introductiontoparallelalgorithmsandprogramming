// =====================================================================
//  Lab 4 - Distributed-memory programming with MPI
//  Exercise 2: 1-D convolution on a distributed vector  --  SOLUTION
//
//  Compile, then run with 4 processes:
//    mpic++ -O2 convolution.cpp -o convolution
//    mpirun -np 4 ./convolution
// =====================================================================

// =====================================================================
//  ANSWERS
//
//  (a) gx[i] is held by process i / n (integer division), in its cell
//      x[i % n]. The x[j] of process k is gx[k*n + j]. The left neighbour
//      of process k is process k-1 and its right neighbour process k+1,
//      except at the ends: the left neighbour of process 0 is process
//      size-1 and the right neighbour of process size-1 is process 0
//      (the vector is circular), hence (k - 1 + size) % size and
//      (k + 1) % size.
//
//  (e) OK on every process for every number of processes (with -np 1,
//      process 0 is its own left and right neighbour: it sends the two
//      messages to itself and receives them). Only y[0] and y[n-1] of
//      each block depend on another process's block (xleft and xright);
//      the n-2 others are computed from x alone. Every process sends
//      exactly two messages and receives two, whatever n: the
//      communication does not grow with n, the computation does.
//      With -np 2 (and -np 1), the left and the right neighbour are the
//      same process, and both messages carry one float: a receive takes
//      the first message that arrived from that process. With the two
//      sends first, the receive of xleft could get the element sent to
//      the left (the neighbour's x[0]) instead of the one sent to the
//      right (its x[n-1]). In two rounds, each receive can only match
//      the message of its own round.
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
  int left = (rank - 1 + size) % size; // (b)
  int right = (rank + 1) % size;       // (b)

  // -------------------------------------------------------------------
  //  Question (c): the frontier elements, in two rounds.
  //  Round 1: send x[n-1] to the right neighbour, then receive xleft, the
  //           last element of the left neighbour's block (its x[n-1]).
  //  Round 2: send x[0] to the left neighbour, then receive xright, the
  //           first element of the right neighbour's block (its x[0]).
  // -------------------------------------------------------------------
  float xleft = 0, xright = 0;
  // (c) round 1: my last element goes right, the left neighbour's arrives
  MPI_Send(&x[n - 1], 1, MPI_FLOAT, right, 0, MPI_COMM_WORLD);
  MPI_Recv(&xleft, 1, MPI_FLOAT, left, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  // (c) round 2: my first element goes left, the right neighbour's arrives
  MPI_Send(&x[0], 1, MPI_FLOAT, left, 0, MPI_COMM_WORLD);
  MPI_Recv(&xright, 1, MPI_FLOAT, right, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

  // -------------------------------------------------------------------
  //  Question (d): the three-point average,
  //      y[j] = (x[j-1] + x[j] + x[j+1]) / 3,
  //  with xleft in place of x[-1] and xright in place of x[n].
  // -------------------------------------------------------------------
  // (d) the frontier elements replace x[-1] and x[n]
  for (int j = 0; j < n; j++) {
    float before = (j == 0) ? xleft : x[j - 1];
    float after = (j == n - 1) ? xright : x[j + 1];
    y[j] = (before + x[j] + after) / 3;
  }

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
