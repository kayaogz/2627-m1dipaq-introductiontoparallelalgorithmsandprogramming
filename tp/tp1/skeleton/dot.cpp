// =====================================================================
//  Lab 1 - Introduction to SIMD programming
//  Exercise 2: Vector inner product using SIMD  --  SKELETON
//
//  Fill in the parts marked  TODO  in the order of the questions.
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
//  Question (b): scalar inner product.
//  Returns the sum of x[i] * y[i] for i = 0, ..., N - 1.
//  Note: "noinline, noipa" prevent the compiler from merging the 1000
//  identical calls of the timing loop into a single one (the function has
//  no side effect, so the compiler would otherwise be allowed to call it
//  only once), which would make the measurement meaningless.
// ---------------------------------------------------------------------
__attribute__((noinline, noipa)) float dot_scalar(const float *x, const float *y, int N)
{
  // TODO
  return 0.0f;
}

// ---------------------------------------------------------------------
//  Question (c): vectorized inner product, for any N.
//  The accumulator acc holds 8 partial sums: acc[k] accumulates the
//  products x[i] * y[i] for all i with i mod 8 == k.
//  The vectorized loop handles the first (N / 8) * 8 elements; the
//  remaining N mod 8 elements (at most 7) are added by a scalar loop.
// ---------------------------------------------------------------------
__attribute__((noinline, noipa)) float dot_simd(const float *x, const float *y, int N)
{
  // TODO
  return 0.0f;
}

// ---------------------------------------------------------------------
//  Helper (provided): compares the result of a version against the
//  scalar result, and prints whether they are identical.
// ---------------------------------------------------------------------
void check(float ref, float val, const char *name)
{
  if (ref == 0.0f && val == 0.0f) {
    printf("[%s] both results are 0: functions not implemented yet?\n", name);
    return;
  }
  if (ref == val) {
    printf("[%s] OK: result = %f, identical to the scalar version\n", name, val);
  } else {
    printf("[%s] result = %f, scalar version = %f (relative difference %e)\n",
           name, val, ref, (val - ref) / ref);
  }
}

// ---------------------------------------------------------------------
//  Question (d): times REPS consecutive executions of the scalar and the
//  vectorized inner product on arrays of size N, and prints the speedup.
// ---------------------------------------------------------------------
void benchmark(int N, int REPS)
{
  // Allocating arrays x and y of size N, initializing x[i] = i, y[i] = 1.
  float *x = NULL; // TODO: allocate N floats
  float *y = NULL; // TODO: allocate N floats
  // TODO: initialize x[i] = i and y[i] = 1

  double t0, t_scalar, t_simd;
  float r_scalar = 0.0f, r_simd = 0.0f;

  t0 = now();
  // TODO: call dot_scalar REPS times, storing its result in r_scalar
  t_scalar = now() - t0;

  t0 = now();
  // TODO: call dot_simd REPS times, storing its result in r_simd
  t_simd = now() - t0;

  printf("N = %d, %d executions\n", N, REPS);
  printf("  scalar : %.6f s  (result %f)\n", t_scalar, r_scalar);
  printf("  simd   : %.6f s  (result %f, speedup x%.2f)\n", t_simd, r_simd, t_scalar / t_simd);

  // Deallocating arrays.
  // TODO
}

int main()
{
  const int N    = 1024; // array size (multiple of 8)
  const int REPS = 1000; // number of consecutive executions for timing

  // -------------------------------------------------------------------
  //  Question (a): allocate x and y, initialize x[i] = i and y[i] = 1.
  //  With this initialization, the inner product is 0 + 1 + ... + (N-1)
  //  = N * (N - 1) / 2, which is easy to verify.
  // -------------------------------------------------------------------
  float *x = NULL; // TODO: allocate N floats
  float *y = NULL; // TODO: allocate N floats
  // TODO: initialize x[i] = i and y[i] = 1

  // -------------------------------------------------------------------
  //  Question (b): scalar inner product.
  // -------------------------------------------------------------------
  float r_scalar = 0.0f; // TODO: call dot_scalar
  printf("[scalar] result = %f (expected %f)\n", r_scalar, (float) N * (N - 1) / 2);

  // -------------------------------------------------------------------
  //  Question (c): vectorized inner product + comparison with the scalar
  //  version, for N (multiple of 8) and for M (not a multiple of 8).
  // -------------------------------------------------------------------
  float r_simd = 0.0f; // TODO: call dot_simd
  check(r_scalar, r_simd, "simd");

  const int M = 1019; // 1019 = 127 * 8 + 3  ->  3 remaining elements
  // TODO: compare dot_scalar and dot_simd on the first M elements using check

  // -------------------------------------------------------------------
  //  Question (d): timing of both versions over REPS executions, for
  //  N = 1024 and N = 2^20.
  // -------------------------------------------------------------------
  benchmark(1024, REPS);
  benchmark(1 << 20, REPS); // 1 << 20 = 2^20 = 1048576

  // -------------------------------------------------------------------
  //  Question (e): look at the results printed by the benchmark for
  //  N = 2^20: are the scalar and vectorized results identical?
  // -------------------------------------------------------------------

  // -------------------------------------------------------------------
  //  Deallocating arrays.
  // -------------------------------------------------------------------
  // TODO

  return 0;
}
