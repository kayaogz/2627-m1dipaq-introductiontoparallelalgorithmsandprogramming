// =====================================================================
//  Lab 2 - Multithreaded programming with OpenMP
//  Exercise 3: Parallel mergesort with sections  --  SOLUTION (8 threads)
//
//  Compiler Explorer (https://godbolt.org): x86-64 gcc, with the options
//    -O2 -fopenmp
//  and "Execute the code" ticked in the "Output..." menu of the compiler pane.
//  Question (d): the 4-thread solution of mergesort.cpp extended to 8
//  threads: one more level of sections (eighths, quarters, halves).
// =====================================================================

// =====================================================================
//  ANSWERS
//
//  (d) On a 4-core machine, N = 2^22: sequential ~0.30 s, parallel
//      ~0.10 s, speedup ~3.1, efficiency ~0.8. The speedup is below 4
//      because only the first phase uses the 4 threads: the second phase
//      (two merges of N/2 elements) uses 2 threads, and the last merge of
//      N elements is sequential. Counting element moves, the sequential
//      mergesort costs about N log2(N) = 22 N; the parallel version costs
//      (N/4) log2(N/4) = 5 N for the quarters, N/2 for the half merges
//      and N for the last merge, 6.5 N in total: a speedup of 22/6.5 =
//      3.4 at best. This is Amdahl's law from session 1: the merge phases
//      run on 2 threads, then on 1.
//      With 8 threads (mergesort-8threads.cpp): 8 sections sort the 8
//      eighths, then 4 sections merge them into quarters, 2 sections into
//      halves, and the final merge is sequential. The parallel cost is
//      (N/8) log2(N/8) = 2.4 N + N/4 + N/2 + N = 4.1 N, a speedup of
//      22/4.1 = 5.3 at best on 8 cores, and about 3.5-4 in practice: the
//      sequential tail (the merges) weighs more and more.
// =====================================================================

#include <cstdio>  // printf
#include <cstdlib> // malloc, free, rand, srand
#include <cstring> // memcpy
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

void fill(int *a, int n)
{
  srand(26);
  for (int i = 0; i < n; i++) a[i] = rand();
}

void verify(const int *a, int n, const char *name)
{
  for (int i = 0; i + 1 < n; i++) {
    if (a[i] > a[i + 1]) {
      printf("[%s] ERROR: a[%d] = %d > a[%d] = %d\n", name, i, a[i], i + 1, a[i + 1]);
      return;
    }
  }
  printf("[%s] OK: the array is sorted\n", name);
}

// merges the sorted halves a[0..n/2) and a[n/2..n) into a sorted a[0..n)
void merge(int *a, int *temp, int n)
{
  int i = 0, j = n / 2, k = 0;
  while (i < n / 2 && j < n) {
    if (a[i] <= a[j]) temp[k++] = a[i++];
    else              temp[k++] = a[j++];
  }
  while (i < n / 2) temp[k++] = a[i++];
  while (j < n)     temp[k++] = a[j++];
  memcpy(a, temp, n * sizeof(int));
}

void mergesort(int *a, int *temp, int n)
{
  if (n < 2) return;
  mergesort(a, temp, n / 2);
  mergesort(a + n / 2, temp + n / 2, n - n / 2);
  merge(a, temp, n);
}

int main()
{
  const int N = 1 << 22;
  int *a = (int *)malloc(N * sizeof(int));
  int *temp = (int *)malloc(N * sizeof(int));

  // sequential
  fill(a, N);
  double t0 = now();
  mergesort(a, temp, N);
  double t_seq = now() - t0;
  verify(a, N, "sequential");
  printf("sequential: %.4f s\n", t_seq);

  // parallel
  fill(a, N);
  // Boundaries of the eight eighths, written so that they coincide with
  // the n/2 splits used by merge (see mergesort.cpp for the reasoning).
  const int q2 = N / 2;                                    // halves
  const int q1 = q2 / 2, q3 = q2 + (N - q2) / 2;           // quarters
  const int e1 = q1 / 2, e3 = q1 + (q2 - q1) / 2;          // eighths
  const int e5 = q2 + (q3 - q2) / 2, e7 = q3 + (N - q3) / 2;
  t0 = now();
#pragma omp parallel num_threads(8)
  {
    // (a) eight sections, each sorting one eighth
#pragma omp sections
    {
#pragma omp section
      { mergesort(a, temp, e1); }
#pragma omp section
      { mergesort(a + e1, temp + e1, q1 - e1); }
#pragma omp section
      { mergesort(a + q1, temp + q1, e3 - q1); }
#pragma omp section
      { mergesort(a + e3, temp + e3, q2 - e3); }
#pragma omp section
      { mergesort(a + q2, temp + q2, e5 - q2); }
#pragma omp section
      { mergesort(a + e5, temp + e5, q3 - e5); }
#pragma omp section
      { mergesort(a + q3, temp + q3, e7 - q3); }
#pragma omp section
      { mergesort(a + e7, temp + e7, N - e7); }
    } // implicit barrier: the eight eighths are sorted

    // (b) four sections merge eighths into quarters, then two sections
    //     merge quarters into halves
#pragma omp sections
    {
#pragma omp section
      { merge(a, temp, q1); }
#pragma omp section
      { merge(a + q1, temp + q1, q2 - q1); }
#pragma omp section
      { merge(a + q2, temp + q2, q3 - q2); }
#pragma omp section
      { merge(a + q3, temp + q3, N - q3); }
    } // implicit barrier: the four quarters are sorted

#pragma omp sections
    {
#pragma omp section
      { merge(a, temp, q2); }
#pragma omp section
      { merge(a + q2, temp + q2, N - q2); }
    } // implicit barrier: the two halves are sorted
  }
  // (c) one thread merges the two sorted halves
  merge(a, temp, N);
  double t_par = now() - t0;
  verify(a, N, "parallel");
  printf("parallel:   %.4f s, speedup x%.2f\n", t_par, t_seq / t_par);

  free(a);
  free(temp);
  return 0;
}
