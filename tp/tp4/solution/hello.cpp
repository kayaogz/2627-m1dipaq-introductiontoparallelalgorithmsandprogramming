// =====================================================================
//  Lab 4 - Distributed-memory programming with MPI
//  Exercise 1: Hello world with processes  --  SOLUTION
//
//  Compile, then run with 4 processes:
//    mpic++ -O2 hello.cpp -o hello
//    mpirun -np 4 ./hello
//  (add --oversubscribe after mpirun for more processes than cores)
// =====================================================================

// =====================================================================
//  ANSWERS
//
//  (b) With -np p, exactly p lines "I am process k of p", k = 0 .. p-1,
//      one per process; with -np 1 a single line "I am process 0 of 1".
//      The set of lines is the same at every run, their order is not:
//      the processes are asynchronous and print whenever they get there;
//      mpirun forwards their outputs in the order it receives them.
//
// =====================================================================

#include <cstdio>
#include <mpi.h>

int main(int argc, char **argv)
{
  MPI_Init(&argc, &argv); // (provided) the first MPI call

  // (a) my rank and the number of processes
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  printf("I am process %d of %d\n", rank, size);

  MPI_Finalize(); // (provided) the last MPI call
  return 0;
}
