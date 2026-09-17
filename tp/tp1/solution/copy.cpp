// =====================================================================
//  Lab 1 - Introduction to SIMD programming
//  Exercise 1: Copying an array using SIMD  --  SOLUTION
//
//  Compiler Explorer (https://godbolt.org): x86-64 gcc, with the options
//    -O3 -fno-tree-vectorize -std=c++11 -mavx2
//  and "Execute the code" ticked in the compiler's Options menu.
// =====================================================================

#include <cstdio>      // printf
#include <cstdlib>     // malloc, free
#include <chrono>      // timing
#include <immintrin.h> // AVX intrinsics

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
//  Question (b): scalar copy.
//  Copies the N floats of A into B, one element at a time.
//  Note: "noinline" prevents the compiler from merging the 1000 identical
//  calls of the timing loop into a single one, which would make the
//  measurement meaningless.
// ---------------------------------------------------------------------
__attribute__((noinline)) void copy_scalar(const float *A, float *B, int N)
{
  for (int i = 0; i < N; i++) {
    B[i] = A[i];
  }
}

// ---------------------------------------------------------------------
//  Question (c): vectorized copy (N is assumed to be a multiple of 8).
//  Each iteration loads 8 floats of A into an AVX register, then stores
//  these 8 floats into B. Hence the loop advances by 8 at each step.
// ---------------------------------------------------------------------
__attribute__((noinline)) void copy_simd(const float *A, float *B, int N)
{
  for (int i = 0; i < N; i += 8) {
    __m256 v = _mm256_loadu_ps(&A[i]); // v = A[i], A[i+1], ..., A[i+7]
    _mm256_storeu_ps(&B[i], v);        // B[i], ..., B[i+7] = v
  }
}

// ---------------------------------------------------------------------
//  Question (d): vectorized copy for any N.
//  The vectorized loop handles the first (N / 8) * 8 elements, i.e. as
//  many full groups of 8 as possible. The remaining N mod 8 elements
//  (at most 7) are copied by a scalar loop.
// ---------------------------------------------------------------------
__attribute__((noinline)) void copy_simd_any(const float *A, float *B, int N)
{
  int N8 = (N / 8) * 8; // largest multiple of 8 that is <= N

  // Vectorized part
  for (int i = 0; i < N8; i += 8) {
    __m256 v = _mm256_loadu_ps(&A[i]);
    _mm256_storeu_ps(&B[i], v);
  }

  // Scalar remainder: elements N8, N8 + 1, ..., N - 1
  for (int i = N8; i < N; i++) {
    B[i] = A[i];
  }
}

// ---------------------------------------------------------------------
//  Helper (provided): checks that B[i] == A[i] for all i, and prints
//  the result of the check.
// ---------------------------------------------------------------------
void check(const float *A, const float *B, int N, const char *name)
{
  if (A == NULL || B == NULL) {
    printf("[%s] arrays are not allocated yet\n", name);
    return;
  }
  for (int i = 0; i < N; i++) {
    if (A[i] != B[i]) {
      printf("[%s] ERROR: B[%d] = %f, expected %f\n", name, i, B[i], A[i]);
      return;
    }
  }
  printf("[%s] OK: B is identical to A\n", name);
}

// ---------------------------------------------------------------------
//  Question (e): times REPS consecutive executions of the scalar and the
//  vectorized copy on arrays of size N, and prints the speedup.
// ---------------------------------------------------------------------
void benchmark(int N, int REPS)
{
  // Allocating arrays A and B of size N, initializing A[i] = i.
  float *A = (float *) malloc(N * sizeof(float));
  float *B = (float *) malloc(N * sizeof(float));
  for (int i = 0; i < N; i++) {
    A[i] = (float) i;
  }

  double t0, t_scalar, t_simd;

  t0 = now();
  for (int r = 0; r < REPS; r++) {
    copy_scalar(A, B, N);
  }
  t_scalar = now() - t0;

  t0 = now();
  for (int r = 0; r < REPS; r++) {
    copy_simd(A, B, N);
  }
  t_simd = now() - t0;

  printf("N = %d, %d executions\n", N, REPS);
  printf("  scalar : %.6f s\n", t_scalar);
  printf("  simd   : %.6f s  (speedup x%.2f)\n", t_simd, t_scalar / t_simd);

  // Deallocating arrays.
  free(A);
  free(B);
}

int main()
{
  const int N    = 1024; // array size (multiple of 8)
  const int REPS = 1000; // number of consecutive executions for timing

  // -------------------------------------------------------------------
  //  Question (a): allocate A and B, initialize A[i] = i.
  // -------------------------------------------------------------------
  float *A = (float *) malloc(N * sizeof(float));
  float *B = (float *) malloc(N * sizeof(float));
  for (int i = 0; i < N; i++) {
    A[i] = (float) i;
  }

  // -------------------------------------------------------------------
  //  Question (b): scalar copy + correctness check.
  // -------------------------------------------------------------------
  for (int i = 0; i < N; i++) B[i] = 0.0f; // reset B before the test
  copy_scalar(A, B, N);
  check(A, B, N, "scalar");

  // -------------------------------------------------------------------
  //  Question (c): vectorized copy + correctness check.
  // -------------------------------------------------------------------
  for (int i = 0; i < N; i++) B[i] = 0.0f; // reset B before the test
  copy_simd(A, B, N);
  check(A, B, N, "simd");

  // -------------------------------------------------------------------
  //  Question (d): N not a multiple of 8.
  //  We reuse the same arrays but only copy the first M elements, with M
  //  not divisible by 8, to test the version handling the remainder.
  // -------------------------------------------------------------------
  const int M = 1019; // 1019 = 127 * 8 + 3  ->  3 remaining elements
  for (int i = 0; i < N; i++) B[i] = 0.0f; // reset B before the test
  copy_simd_any(A, B, M);
  check(A, B, M, "simd, N = 1019");

  // -------------------------------------------------------------------
  //  Question (e): timing of both versions over REPS executions, for
  //  N = 1024 and N = 2^20.
  // -------------------------------------------------------------------
  benchmark(1024, REPS);
  benchmark(1 << 20, REPS); // 1 << 20 = 2^20 = 1048576

  // -------------------------------------------------------------------
  //  Deallocating arrays.
  // -------------------------------------------------------------------
  free(A);
  free(B);

  return 0;
}
