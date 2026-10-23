// =====================================================================
//  Lab 4 - Distributed-memory programming with MPI
//  Exercise 1: Hello world with processes  --  SKELETON
//
//  Compile, then run with 4 processes:
//    mpic++ -O2 hello.cpp -o hello
//    mpirun -np 4 ./hello
//  (add --oversubscribe after mpirun for more processes than cores)
// =====================================================================

// =====================================================================
//  ANSWERS (as comments; this file is the only thing you submit)
//
//  (b) With -np 1, 2, 4 and 8, each run twice: what is printed, what
//      changes from one run to the next, and what does not?
//
// =====================================================================

#include <cstdio>
#include <mpi.h>

int main(int argc, char **argv)
{
  MPI_Init(&argc, &argv); // (provided) the first MPI call

  // TODO (a): get my rank and the number of processes, then print
  //           "I am process <rank> of <size>"

  MPI_Finalize(); // (provided) the last MPI call
  return 0;
}
