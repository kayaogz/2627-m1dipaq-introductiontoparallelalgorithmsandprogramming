// =====================================================================
//  Lab 2 - Multithreaded programming with OpenMP
//  Exercise 2: Sum of an array with sections  --  SOLUTION
//
//  Compiler Explorer (https://godbolt.org): x86-64 gcc, with the options
//    -O2 -fopenmp
//  and "Execute the code" ticked in the "Output..." menu of the compiler pane.
// =====================================================================

// =====================================================================
//  ANSWERS
//
//  (e) Typical 4-core machine, N = 10^7: ~12 ms sequential, ~12 ms with
//      1 thread (speedup 1: the region costs nothing measurable), ~7 ms
//      with 2 threads (speedup ~1.8, efficiency ~0.9), ~4.5-5 ms with 4
//      threads (speedup ~2.5, efficiency ~0.65). The speedup stays below
//      4 because both loops only stream through 80 MB with one operation
//      per element: the memory bandwidth, not the cores, is the limit.
//      With 8 threads, nothing improves (the differences are noise):
//      there are only 4 sections, so at most 4 threads have something to
//      do, the other 4 idle at the barrier. More threads than sections
//      cannot help; to use 8 cores, one would need 8 sections, or a
//      distribution by thread id that adapts to the number of threads,
//      as for the sum of an array in the lecture.
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
  const double expected = (double)N * (N - 1) / 2;

  // First touch of the memory (not timed): the first write to a freshly
  // allocated page is slow, and would distort the sequential timing.
  // (A non-zero value on purpose: the compiler turns "malloc then fill
  // with zeros" into calloc, whose pages are not touched either.)
  for (int i = 0; i < N; i++) A[i] = -1.0;

  // (a) + (b) sequential initialization and sum, timed together
  double sum = 0.0;
  double t0 = now();
  for (int i = 0; i < N; i++) A[i] = i;
  for (int i = 0; i < N; i++) sum = sum + A[i];
  double t_seq = now() - t0;
  printf("sequential: sum = %.0f (expected %.0f), %.4f s\n", sum, expected, t_seq);

  // (c) + (d) parallel initialization and sum, with 4 sections each
  double partial[4] = {0.0, 0.0, 0.0, 0.0};
  const int Q = N / 4; // size of a quarter (the 4th one also takes N % 4)
  sum = 0.0;
  t0 = now();
#pragma omp parallel num_threads(4)
  {
    // (c) section k initializes the quarter [k*Q, (k+1)*Q)
#pragma omp sections
    {
#pragma omp section
      { for (int i = 0 * Q; i < 1 * Q; i++) A[i] = i; }
#pragma omp section
      { for (int i = 1 * Q; i < 2 * Q; i++) A[i] = i; }
#pragma omp section
      { for (int i = 2 * Q; i < 3 * Q; i++) A[i] = i; }
#pragma omp section
      { for (int i = 3 * Q; i < N; i++) A[i] = i; }
    } // implicit barrier: A is fully initialized

    // (d) section k sums its quarter into a local accumulator, then
    //     stores it into its own cell partial[k]: no two sections write
    //     the same variable.
#pragma omp sections
    {
#pragma omp section
      {
        double local = 0.0;
        for (int i = 0 * Q; i < 1 * Q; i++) local = local + A[i];
        partial[0] = local;
      }
#pragma omp section
      {
        double local = 0.0;
        for (int i = 1 * Q; i < 2 * Q; i++) local = local + A[i];
        partial[1] = local;
      }
#pragma omp section
      {
        double local = 0.0;
        for (int i = 2 * Q; i < 3 * Q; i++) local = local + A[i];
        partial[2] = local;
      }
#pragma omp section
      {
        double local = 0.0;
        for (int i = 3 * Q; i < N; i++) local = local + A[i];
        partial[3] = local;
      }
    } // implicit barrier: the 4 partial sums are written
  }
  // (d) merge, sequentially, after the region
  for (int k = 0; k < 4; k++) sum = sum + partial[k];
  double t_par = now() - t0;

  printf("parallel:   sum = %.0f (expected %.0f), %.4f s, speedup x%.2f\n",
         sum, expected, t_par, t_seq / t_par);

  free(A);
  return 0;
}
