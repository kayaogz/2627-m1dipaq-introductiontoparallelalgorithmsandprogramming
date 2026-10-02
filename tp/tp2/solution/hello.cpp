// =====================================================================
//  Lab 2 - Multithreaded programming with OpenMP
//  Exercise 1: Hello world with threads  --  SOLUTION
//
//  Compiler Explorer (https://godbolt.org): x86-64 gcc, with the options
//    -O2 -fopenmp
//  and "Execute the code" ticked in the "Output..." menu of the compiler pane.
// =====================================================================

// =====================================================================
//  ANSWERS
//
//  (d) The number of "I am thread" lines follows the number of threads
//      (2, 4 or 8: with 8 threads on a 4-core machine, all 8 lines still
//      appear, the threads simply take turns on the cores), and there is
//      always exactly one "master" line (thread 0 always exists) and
//      exactly one "random thread" line, in this order, thanks to the
//      two barriers. What changes between runs is the order of the
//      "I am thread" lines among themselves (the threads are
//      asynchronous, and nothing orders them) and the id printed by the
//      single section (OpenMP gives it to whichever thread reaches the
//      sections block first). Without the first barrier, the master's
//      line could appear before some "I am thread" lines; without the
//      second one, the "random thread" line could appear before the
//      master's line.
// =====================================================================

#include <cstdio> // printf
#include <omp.h>  // omp_get_thread_num, omp_get_num_threads

int main()
{
#pragma omp parallel num_threads(4)
  {
    // (a) Every thread executes this block; thid and numth are declared
    //     inside, so each thread has its own copy.
    int thid = omp_get_thread_num();
    int numth = omp_get_num_threads();
    printf("I am thread %d of %d\n", thid, numth);
    // Nobody goes on before every thread has printed its line: without
    // this barrier, thread 0 could print "Hello" while the others are
    // still on the line above.
#pragma omp barrier

    // (b) The master thread is the one with id 0: a plain test on thid.
    if (thid == 0) {
      printf("Hello from the master thread, id = %d\n", thid);
    }
    // Nobody goes on before the master has printed its line: without
    // this barrier, the section below could be executed first.
#pragma omp barrier

    // (c) A sections block with a single section: exactly one thread,
    //     chosen by OpenMP, executes it (which one may change between
    //     runs).
#pragma omp sections
    {
#pragma omp section
      {
        printf("Hello from a random thread, id = %d\n", thid);
      }
    } // implicit barrier
  }

  return 0;
}
