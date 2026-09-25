// =====================================================================
//  Lab 2 - Multithreaded programming with OpenMP
//  Exercise 2: Sum of an array with sections  --  SKELETON
//
//  Fill in the parts marked  TODO  in the order of the questions.
//
//  Compiler Explorer (https://godbolt.org): x86-64 gcc, with the options
//    -O2 -fopenmp
//  and "Execute the code" ticked in the "Output..." menu of the compiler pane.
//  Compiler Explorer may give you a single core: take the timings of
//  question (e) on your own machine (g++ -O2 -fopenmp sum.cpp -o sum).
// =====================================================================

// =====================================================================
//  ANSWERS to the questions of the assignment (write them here, as
//  comments; this is the only file you submit for this exercise).
//
//  (e) Timings, speedups and efficiencies for 1, 2, 4, 8 threads; why does
//      the time no longer improve beyond 4 threads?
//
// =====================================================================

#include <cstdio>  // printf
#include <cstdlib> // malloc, free
#include <chrono>  // timing
#include <omp.h>   // OpenMP

// ---------------------------------------------------------------------
//  Timing helper (provided): returns the current time in seconds.
//  Usage:  double t0 = now(); ...work... ; double elapsed = now() - t0;
// ---------------------------------------------------------------------
double now()
{
  using namespace std::chrono;
  return duration<double>(high_resolution_clock::now().time_since_epoch()).count();
}

int main()
{
  const int N = 10000000; // 10 million elements (80 MB of doubles)
  double *A = (double *)malloc(N * sizeof(double));

  // With A[i] = i, the sum is 0 + 1 + ... + (N-1) = N * (N-1) / 2.
  const double expected = (double)N * (N - 1) / 2;

  // First touch of the memory (not timed): the first write to a freshly
  // allocated page is slow, and would distort the sequential timing.
  // (A non-zero value on purpose: the compiler turns "malloc then fill
  // with zeros" into calloc, whose pages are not touched either.)
  for (int i = 0; i < N; i++) A[i] = -1.0;

  // -------------------------------------------------------------------
  //  Question (a): initialize A[i] = i for all 0 <= i < N (sequential).
  //  Question (b): sequential sum of all the elements of A.
  //  The two loops together are timed as the sequential computation.
  // -------------------------------------------------------------------
  double sum = 0.0;
  double t0 = now();
  // TODO (a)
  // TODO (b)
  double t_seq = now() - t0;
  printf("sequential: sum = %.0f (expected %.0f), %.4f s\n", sum, expected, t_seq);

  // -------------------------------------------------------------------
  //  Question (c): initialize A again, in a parallel region with 4 threads
  //  and 4 sections; each section initializes N/4 of the elements.
  //
  //  Question (d): in the same parallel region, after the initialization
  //  sections, a second sections block with 4 sections: each section adds
  //  its N/4 elements into a local accumulator (a variable declared inside
  //  the section), then stores it into its own cell of partial. After the
  //  parallel region, merge the 4 partial sums into sum, sequentially.
  // -------------------------------------------------------------------
  double partial[4] = {0.0, 0.0, 0.0, 0.0};
  sum = 0.0;
  t0 = now();
  // TODO (c) + (d): parallel region with the two sections blocks
  // TODO (d): merge partial[0..3] into sum
  double t_par = now() - t0;
  printf("parallel:   sum = %.0f (expected %.0f), %.4f s, speedup x%.2f\n",
         sum, expected, t_par, t_seq / t_par);

  // -------------------------------------------------------------------
  //  Question (e): on your own machine, run with num_threads(1), (2), (4)
  //  and (8): speedup and efficiency for each (answers at the top).
  // -------------------------------------------------------------------

  free(A);
  return 0;
}
