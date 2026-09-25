// =====================================================================
//  Lab 2 - Multithreaded programming with OpenMP
//  Exercise 1: Hello world with threads  --  SKELETON
//
//  Fill in the parts marked  TODO  in the order of the questions.
//
//  Compiler Explorer (https://godbolt.org): x86-64 gcc, with the options
//    -O2 -fopenmp
//  and "Execute the code" ticked in the "Output..." menu of the compiler pane.
// =====================================================================

// =====================================================================
//  ANSWERS to the questions of the assignment (write them here, as
//  comments; this is the only file you submit for this exercise).
//
//  (d) What changes between runs and with 2 or 8 threads, what does not;
//      what could happen without the constructs that enforce the order:
//
// =====================================================================

#include <cstdio> // printf
#include <omp.h>  // omp_get_thread_num, omp_get_num_threads

int main()
{
  // -------------------------------------------------------------------
  //  Question (a): create a parallel region with 4 threads, in which each
  //  thread prints "I am thread <id> of <n>", n being the number of threads.
  //
  //  Question (b): in the same parallel region, the master thread (the one
  //  whose id is 0) prints "Hello from the master thread, id = 0". This
  //  line must ALWAYS come after all the lines of question (a).
  //
  //  Question (c): still in the same region, ONE thread, chosen by OpenMP,
  //  prints "Hello from a random thread, id = <id>" (hint: use sections).
  //  This line must ALWAYS come after the master's line.
  //
  //  Question (d): run the program several times, then with 2 and with 8
  //  threads: what changes in the output, what does not? What could happen
  //  without the constructs you added to enforce the order in (b) and (c)?
  // -------------------------------------------------------------------
  // TODO

  return 0;
}
