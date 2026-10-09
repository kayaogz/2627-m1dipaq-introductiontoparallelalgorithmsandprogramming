// =====================================================================
//  Lab 3 - GPU programming with CUDA
//  Exercise 2: saxpy on the GPU  --  SKELETON
//
//  Compiler Explorer (https://cuda.godbolt.org): language CUDA C++,
//  compiler NVCC; "Add new..." > "Executor From This" in the compiler
//  pane, then the option -O2 in the executor pane, which runs the program.
// =====================================================================

// =====================================================================
//  ANSWERS (as comments; this file is the only thing you submit)
//
//  (c) T = 1, 2, 4, 8, ..., 1024 threads per block (B adjusted so that
//      B * T >= N): is the result correct every time? how does the kernel
//      time change?
//
//  (d) Time of the copy in (CPU -> GPU) and of the copy out (GPU -> CPU),
//      compared with the time of the kernel: how many times longer?
//      What does it mean for a program that launches one kernel, and for
//      a program that launches many kernels on the same data?
//
// =====================================================================

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <chrono>
#include <cuda_runtime.h>

// Timing helper (provided): the current time in seconds.
double now()
{
  using namespace std::chrono;
  return duration<double>(high_resolution_clock::now().time_since_epoch()).count();
}

// warmup (provided): an empty kernel. The first launch of a program pays
// a start-up cost; main launches this one first, so that the times
// measured afterwards are those of the work.
__global__ void warmup() {}

// ---------------------------------------------------------------------
//  Question (a): the kernel saxpy. Every thread handles ONE element, as a
//  PU of the PRAM: its index i is blockIdx.x * blockDim.x + threadIdx.x,
//  and it computes  dy[i] = a * dx[i] + dy[i].
//  WARNING: B * T >= n, so the last block may have threads with no
//  element; the kernel must start with the guard  if (i < n).
//  dx and dy are GPU arrays (allocated and filled in main); a and n are
//  values, every thread receives its own copy.
// ---------------------------------------------------------------------
__global__ void saxpy(const float *dx, float *dy, float a, int n)
{
  // TODO (a)
}

// verify (provided): checks that res[i] = a * x[i] + y[i] for all i.
void verify(const float *x, const float *y, const float *res, float a, int n)
{
  for (int i = 0; i < n; i++) {
    float expected = a * x[i] + y[i];
    if (fabs(res[i] - expected) > 1e-3f * fabs(expected) + 1e-5f) {
      printf("WRONG at i = %d: %g instead of %g\n", i, res[i], expected);
      return;
    }
  }
  printf("correct\n");
}

int main()
{
  const int N = 1000000; // one million elements: 4 MB per array
  const float a = 2.0f;

  // CPU arrays: x and y (the inputs), res (the result brought back from
  // the GPU, for verification).
  float *x = (float *)malloc(N * sizeof(float));
  float *y = (float *)malloc(N * sizeof(float));
  float *res = (float *)malloc(N * sizeof(float));
  for (int i = 0; i < N; i++) {
    x[i] = i;
    y[i] = 1.0f;
  }

  // GPU arrays (provided): dx and dy, the GPU twins of x and y ("d" for
  // device, CUDA's word for the GPU). cudaMalloc makes room for N floats
  // in the GPU memory.
  float *dx, *dy;
  cudaMalloc(&dx, N * sizeof(float));
  cudaMalloc(&dy, N * sizeof(float));
  warmup<<<1, 1>>>();      // (provided) the start-up cost, paid here
  cudaDeviceSynchronize();

  // Copy in (provided): the N floats of x into dx, and of y into dy
  // (CPU -> GPU, cudaMemcpyHostToDevice).
  // Question (d): time the two copies with now(), as the kernel below,
  // and print the time.
  // TODO (d)
  cudaMemcpy(dx, x, N * sizeof(float), cudaMemcpyHostToDevice);
  cudaMemcpy(dy, y, N * sizeof(float), cudaMemcpyHostToDevice);

  // -------------------------------------------------------------------
  //  Question (b): set T, the number of threads per block, and compute B,
  //  the number of blocks, so that B * T >= N (one thread per element,
  //  plus the spare threads of the last block), then launch saxpy with B
  //  blocks of T threads. The kernel works on dx and dy: the GPU cannot
  //  see x and y.
  //  Question (c): T = 1, 2, 4, 8, ..., 1024 (answers at the top).
  // -------------------------------------------------------------------
  int T, B;
  double t0 = now();
  // TODO (b): T, B, and the launch
  cudaDeviceSynchronize(); // wait until the GPU is done, to time it
  printf("kernel (T = %d, B = %d): %.6f s\n", T, B, now() - t0);

  // Copy out (provided): dy comes back into res (GPU -> CPU), then res is
  // checked. Question (d): time this copy too.
  // TODO (d)
  cudaMemcpy(res, dy, N * sizeof(float), cudaMemcpyDeviceToHost);
  verify(x, y, res, a, N);

  cudaFree(dx);
  cudaFree(dy);
  free(x);
  free(y);
  free(res);
  return 0;
}
