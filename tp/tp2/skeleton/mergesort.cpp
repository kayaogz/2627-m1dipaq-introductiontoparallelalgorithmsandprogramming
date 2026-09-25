// =====================================================================
//  Lab 2 - Multithreaded programming with OpenMP
//  Exercise 3: Parallel mergesort with sections  --  SKELETON
//
//  Fill in the parts marked  TODO  in the order of the questions.
//
//  Compiler Explorer (https://godbolt.org): x86-64 gcc, with the options
//    -O2 -fopenmp
//  and "Execute the code" ticked in the "Output..." menu of the compiler pane.
//  Compiler Explorer may give you a single core: take the timings of
//  question (d) on your own machine.
// =====================================================================

// =====================================================================
//  ANSWERS to the questions of the assignment (write them here, as
//  comments; this is the only file you submit for this exercise).
//
//  (d) Sequential and parallel times, speedup, efficiency, and why the
//      speedup is below 4; the same with 8 threads if your machine has
//      at least 8 cores:
//
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

// ---------------------------------------------------------------------
//  Helper (provided): fills a[0..n) with pseudo-random integers. The seed
//  is fixed, so every call produces the same sequence: the sequential and
//  the parallel sorts work on the same input.
// ---------------------------------------------------------------------
void fill(int *a, int n)
{
  srand(26);
  for (int i = 0; i < n; i++) a[i] = rand();
}

// ---------------------------------------------------------------------
//  Helper (provided): checks that a[0..n) is sorted in non-decreasing
//  order and prints the verdict.
// ---------------------------------------------------------------------
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

// ---------------------------------------------------------------------
//  Provided: merge(a, temp, n) takes an array a of n elements whose two
//  halves a[0..n/2) and a[n/2..n) are ALREADY SORTED, and rearranges a
//  into a single sorted array of n elements. temp is a scratch buffer of
//  at least n elements: the merged sequence is built in temp, then copied
//  back into a.
//
//    a    = [ 2 5 9 | 1 3 8 ]   ->   a = [ 1 2 3 5 8 9 ]
//            sorted   sorted
// ---------------------------------------------------------------------
void merge(int *a, int *temp, int n)
{
  int i = 0;     // next element of the left half  a[0..n/2)
  int j = n / 2; // next element of the right half a[n/2..n)
  int k = 0;     // next free slot of temp
  while (i < n / 2 && j < n) {
    if (a[i] <= a[j]) temp[k++] = a[i++];
    else              temp[k++] = a[j++];
  }
  while (i < n / 2) temp[k++] = a[i++]; // left half not exhausted
  while (j < n)     temp[k++] = a[j++]; // right half not exhausted
  memcpy(a, temp, n * sizeof(int));     // copy the result back into a
}

// ---------------------------------------------------------------------
//  Provided: sequential mergesort of a[0..n). Sorts each half recursively,
//  then merges them. temp is a scratch buffer of at least n elements.
//  "a + n/2" is the address of a[n/2], i.e. the right half seen as an
//  array of its own (pointer arithmetic); same for temp.
// ---------------------------------------------------------------------
void mergesort(int *a, int *temp, int n)
{
  if (n < 2) return;                             // 0 or 1 element: sorted
  mergesort(a, temp, n / 2);                     // sort the left half
  mergesort(a + n / 2, temp + n / 2, n - n / 2); // sort the right half
  merge(a, temp, n);                             // merge the two halves
}

int main()
{
  const int N = 1 << 22; // 4M elements; a power of two, so N/2, N/4 are exact
  int *a = (int *)malloc(N * sizeof(int));
  int *temp = (int *)malloc(N * sizeof(int));

  // -------------------------------------------------------------------
  //  Sequential sort (provided), for reference and timing.
  // -------------------------------------------------------------------
  fill(a, N);
  double t0 = now();
  mergesort(a, temp, N);
  double t_seq = now() - t0;
  verify(a, N, "sequential");
  printf("sequential: %.4f s\n", t_seq);

  // -------------------------------------------------------------------
  //  Parallel sort, on the same input.
  //
  //  Question (a): a parallel region with 4 threads and a sections block
  //  with 4 sections; each section sorts one quarter of a with mergesort.
  //  Hint: the quarter starting at index b is the array "a + b" of N/4
  //  elements, and its scratch buffer is "temp + b".
  //
  //  Question (b): in the same region, after the implicit barrier of the
  //  first sections block, a second sections block with 2 sections: each
  //  merges two sorted quarters into a sorted half, with merge.
  //
  //  Question (c): after the parallel region, merge the two sorted halves
  //  into the final sorted array, with merge.
  //
  //  Question (d): timings on your own machine with 4 threads (answers at
  //  the top); if your machine has at least 8 cores, modify this code to
  //  work with 8 threads and measure again.
  // -------------------------------------------------------------------
  fill(a, N);
  t0 = now();
  // TODO
  double t_par = now() - t0;
  verify(a, N, "parallel");
  printf("parallel:   %.4f s, speedup x%.2f\n", t_par, t_seq / t_par);

  free(a);
  free(temp);
  return 0;
}
