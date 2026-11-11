// =====================================================================
//  Lab 4 - Distributed-memory programming with MPI
//  Exercise 3: Parallel mergesort with processes  --  SOLUTION
//
//  Compile, then run with 4 processes (a power of two: 1, 2, 4, 8, ...):
//    mpic++ -O2 mergesort.cpp -o mergesort
//    mpirun -np 4 ./mergesort
// =====================================================================

// =====================================================================
//  ANSWERS
//
//  (d) Orders of magnitude on a laptop with 4 cores (N = 2^22):
//      sequential 0.45 s; parallel 0.45 s with 1 process (the sequential
//      sort), 0.28 s with 2 (speedup x1.6), 0.19 s with 4 (x2.4), 0.17 s
//      with 8 (x2.6, no more cores: the processes share them).
//      The speedup stays below the number of processes because the merge
//      phase is not parallel: the last step merges all N elements on
//      process 0 alone, after a message of N/2 elements; the step before
//      merges N/2 elements on two processes, and so on. The local sorts
//      are perfectly parallel (N/p log(N/p) each), the log2(p) folding
//      steps add N + N/2 + ... ~ 2N sequential operations plus the
//      transfers, whatever p.
//
// =====================================================================

#include <cstdio>  // printf
#include <cstdlib> // malloc, free, rand, srand
#include <cstring> // memcpy
#include <chrono>  // timing
#include <mpi.h>   // MPI

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
//  Helper (provided): fills a[0..n) with pseudo-random integers. The
//  sequence depends only on the seed: block k of the global array is
//  fill(..., n, k), on whichever process regenerates it.
// ---------------------------------------------------------------------
void fill(int *a, int n, int seed)
{
  srand(26 + seed);
  for (int i = 0; i < n; i++) a[i] = rand();
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

// ---------------------------------------------------------------------
//  Provided, run by process 0 at the end: regenerates the whole input
//  (the size blocks, with fill), sorts it sequentially (and times it),
//  and compares it element by element with a[0..N): equal means that a
//  is sorted AND contains exactly the initial elements.
// ---------------------------------------------------------------------
void verify(const int *a, int N, int size, double t_par)
{
  int *b = (int *)malloc(N * sizeof(int));
  int *temp = (int *)malloc(N * sizeof(int));
  int n = N / size;
  for (int k = 0; k < size; k++) fill(b + k * n, n, k); // block k of the input
  double t0 = now();
  mergesort(b, temp, N);
  double t_seq = now() - t0;
  for (int i = 0; i < N; i++) {
    if (a[i] != b[i]) {
      printf("ERROR: a[%d] = %d, expected %d\n", i, a[i], b[i]);
      free(b); free(temp);
      return;
    }
  }
  printf("OK: sorted, and the same elements as the input\n");
  printf("sequential: %.4f s\n", t_seq);
  printf("parallel:   %.4f s with %d processes, speedup x%.2f\n",
         t_par, size, t_seq / t_par);
  free(b); free(temp);
}

int main(int argc, char **argv)
{
  MPI_Init(&argc, &argv);
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  // -------------------------------------------------------------------
  //  (provided) The global array has N = 2^22 integers; process k holds
  //  its block k, n = N / size integers, in a[0..n). a and temp have room
  //  for the whole array: the blocks merged along the way grow, and
  //  process 0 ends with all N elements.
  // -------------------------------------------------------------------
  const int N = 1 << 22;
  int n = N / size;
  int *a = (int *)malloc(N * sizeof(int));
  int *temp = (int *)malloc(N * sizeof(int));
  fill(a, n, rank);

  double t0 = now();

  // -------------------------------------------------------------------
  //  Question (a): sort my block a[0..n) with mergesort.
  //
  //  Question (b): the merging steps. At each step the active processes
  //  are 0 .. 2*half - 1, each holding m sorted elements in a[0..m), and
  //  the second half folds onto the first half:
  //   - a process with rank >= half sends its m elements to process
  //     rank - half; its work is done, it leaves the loop;
  //   - a process with rank < half receives the m elements of process
  //     rank + half right after its own, in a[m..2m), merges the two
  //     sorted halves of a[0..2m), and now holds 2m elements.
  //  half takes the values size/2, size/4, ..., 1 (m starts at n and
  //  doubles at every step); at the end, process 0 holds the sorted
  //  array in a[0..N).
  // -------------------------------------------------------------------
  // (a) my block
  mergesort(a, temp, n);

  // (b) the merging steps: the second half folds onto the first half
  int m = n; // my sorted elements so far, in a[0..m)
  for (int half = size / 2; half >= 1; half = half / 2) {
    if (rank >= half) {
      // second half: I send my m sorted elements to process rank - half,
      // and I am done
      MPI_Send(a, m, MPI_INT, rank - half, 0, MPI_COMM_WORLD);
      break;
    }
    // first half: I receive the m sorted elements of process rank + half
    // right after mine, then merge the two sorted halves of a[0..2m)
    MPI_Recv(a + m, m, MPI_INT, rank + half, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    merge(a, temp, 2 * m);
    m = 2 * m;
  }

  double t_par = now() - t0;

  // -------------------------------------------------------------------
  //  Question (c): process 0 checks the result with verify, which also
  //  prints the sequential time and the speedup.
  // -------------------------------------------------------------------
  // (c) process 0 holds the sorted array: check it
  if (rank == 0) verify(a, N, size, t_par);

  free(a);
  free(temp);
  MPI_Finalize();
  return 0;
}
