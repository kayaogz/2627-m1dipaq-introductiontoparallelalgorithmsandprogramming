// =====================================================================
//  Lab 3 - GPU programming with CUDA
//  Exercise 1: Hello world with GPU threads  --  SOLUTION
//
//  Compiler Explorer (https://cuda.godbolt.org): language CUDA C++,
//  compiler NVCC; "Add new..." > "Executor From This" in the compiler
//  pane, then the option -O2 in the executor pane, which runs the program.
// =====================================================================

// =====================================================================
//  ANSWERS
//
//  (b) Global: p = 64 always, and i takes every value from 0 to 63 once,
//      whatever the split. Local: T and B are the configuration itself
//      (T * B = 64); t runs from 0 to T-1 inside every block and b from 0
//      to B-1, with i = b * T + t. With 64x1, t is always 0 and i = b;
//      with 1x64, b is always 0 and i = t. The order of the lines changes
//      from one run to the next: the threads of a launch run at their own
//      pace; the lines of one block stay grouped, and in order inside it.
//
// =====================================================================

#include <cstdio>
#include <cuda_runtime.h>

// (a) The kernel: every thread prints the four built-in variables, and
//     its global index i and the total p computed from them.
__global__ void hello()
{
  int i = blockIdx.x * blockDim.x + threadIdx.x; // my global index, 0 .. p-1
  int p = gridDim.x * blockDim.x;                // how many of us in all
  printf("thread %d/%d of block %d/%d, global thread %d/%d\n",
         threadIdx.x, blockDim.x, blockIdx.x, gridDim.x, i, p);
}

int main()
{
  // (b) 64 threads in all, as B x T = 64 x 1, 32 x 2, ..., 1 x 64
  int B = 64; // blocks
  int T = 1;  // threads per block

  hello<<<B, T>>>();       // the launch: B x T threads run hello
  cudaDeviceSynchronize(); // the CPU waits until the GPU is done
  return 0;
}
