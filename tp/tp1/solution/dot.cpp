// =====================================================================
//  Lab 1 - Introduction to SIMD programming
//  Exercise 2: Vector inner product using SIMD  --  SOLUTION
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
  float sum = 0.0f;
  for (int i = 0; i < N; i++) {
    sum = sum + x[i] * y[i];
  }
  return sum;
}

// ---------------------------------------------------------------------
//  Question (c): vectorized inner product, for any N.
//  Same structure as dot_scalar, with each variable replaced by its
//  vector counterpart: sum -> vsum, x[i] -> vx, y[i] -> vy.
//  The vector accumulator vsum holds 8 partial sums: vsum[k] accumulates
//  the products x[i] * y[i] for all i with i mod 8 == k.
//  The vectorized loop handles the first (N / 8) * 8 elements; the
//  remaining N mod 8 elements (at most 7) are added by a scalar loop.
// ---------------------------------------------------------------------
__attribute__((noinline, noipa)) float dot_simd(const float *x, const float *y, int N)
{
  int N8 = (N / 8) * 8; // largest multiple of 8 that is <= N

  // Initialize vsum to zero: we only have loadu/storeu/add/sub/mul/div,
  // so we prepare 8 zeros in memory and load them into a vector.
  float zeros[8] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
  __m256 vsum = _mm256_loadu_ps(zeros);

  // Vectorized part: vsum = vsum + vx * vy, i.e. the scalar line
  // sum = sum + x[i] * y[i] applied to 8 elements at a time.
  for (int i = 0; i < N8; i += 8) {
    __m256 vx = _mm256_loadu_ps(&x[i]); // vx = x[i], x[i+1], ..., x[i+7]
    __m256 vy = _mm256_loadu_ps(&y[i]); // vy = y[i], y[i+1], ..., y[i+7]
    vsum = _mm256_add_ps(vsum, _mm256_mul_ps(vx, vy));
  }

  // Store the 8 partial sums in memory, then add them with a scalar loop.
  float partial[8];
  _mm256_storeu_ps(partial, vsum);
  float sum = 0.0f;
  for (int k = 0; k < 8; k++) {
    sum = sum + partial[k];
  }

  // Scalar remainder: elements N8, N8 + 1, ..., N - 1
  for (int i = N8; i < N; i++) {
    sum = sum + x[i] * y[i];
  }

  return sum;
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
  float *x = (float *) malloc(N * sizeof(float));
  float *y = (float *) malloc(N * sizeof(float));
  for (int i = 0; i < N; i++) {
    x[i] = (float) i;
    y[i] = 1.0f;
  }

  double t0, t_scalar, t_simd;
  float r_scalar = 0.0f, r_simd = 0.0f;

  t0 = now();
  for (int r = 0; r < REPS; r++) {
    r_scalar = dot_scalar(x, y, N);
  }
  t_scalar = now() - t0;

  t0 = now();
  for (int r = 0; r < REPS; r++) {
    r_simd = dot_simd(x, y, N);
  }
  t_simd = now() - t0;

  printf("N = %d, %d executions\n", N, REPS);
  printf("  scalar : %.6f s  (result %f)\n", t_scalar, r_scalar);
  printf("  simd   : %.6f s  (result %f, speedup x%.2f)\n", t_simd, r_simd, t_scalar / t_simd);

  // Deallocating arrays.
  free(x);
  free(y);
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
  float *x = (float *) malloc(N * sizeof(float));
  float *y = (float *) malloc(N * sizeof(float));
  for (int i = 0; i < N; i++) {
    x[i] = (float) i;
    y[i] = 1.0f;
  }

  // -------------------------------------------------------------------
  //  Question (b): scalar inner product.
  // -------------------------------------------------------------------
  float r_scalar = dot_scalar(x, y, N);
  printf("[scalar] result = %f (expected %f)\n", r_scalar, (float) N * (N - 1) / 2);

  // -------------------------------------------------------------------
  //  Question (c): vectorized inner product + comparison with the scalar
  //  version, for N (multiple of 8) and for M (not a multiple of 8).
  // -------------------------------------------------------------------
  float r_simd = dot_simd(x, y, N);
  check(r_scalar, r_simd, "simd");

  const int M = 1019; // 1019 = 127 * 8 + 3  ->  3 remaining elements
  check(dot_scalar(x, y, M), dot_simd(x, y, M), "simd, N = 1019");

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
  free(x);
  free(y);

  return 0;
}
