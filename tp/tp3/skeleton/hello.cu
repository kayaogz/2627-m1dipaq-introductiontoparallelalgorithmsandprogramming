// =====================================================================
//  Lab 3 - GPU programming with CUDA
//  Exercise 1: Hello world with GPU threads  --  SKELETON
//
//  Compiler Explorer (https://cuda.godbolt.org): language CUDA C++,
//  compiler NVCC; "Add new..." > "Executor From This" in the compiler
//  pane, then the option -O2 in the executor pane, which runs the program.
// =====================================================================

// =====================================================================
//  ANSWERS (as comments; this file is the only thing you submit)
//
//  (b) With 64 threads split as 64x1, 32x2, 16x4, 8x8, 4x16, 2x32, 1x64:
//      how do the six values change from one configuration to the next,
//      and which do not change?
//
// =====================================================================

#include <cstdio>
#include <cuda_runtime.h>

// ---------------------------------------------------------------------
//  Question (a): the kernel hello, the program of ONE thread; the launch
//  in main runs it on every thread. Every thread prints one line
//      "thread <t>/<T> of block <b>/<B>, global thread <i>/<p>"
//  t: its number inside its block, T: the number of threads of a block,
//  b: the number of its block, B: the number of blocks, i: its global
//  index among all the threads of the launch, p: the total number of
//  threads. The four variables of the lecture: threadIdx.x (t),
//  blockDim.x (T), blockIdx.x (b), gridDim.x (B); i and p are computed
//  from them.
// ---------------------------------------------------------------------
__global__ void hello()
{
  // TODO (a)
}

int main()
{
  // Question (b): 64 threads in all, as B x T = 64 x 1, 32 x 2, ..., 1 x 64
  int B = 64; // blocks
  int T = 1;  // threads per block

  hello<<<B, T>>>();       // the launch: B x T threads run hello
  cudaDeviceSynchronize(); // the CPU waits until the GPU is done
  return 0;
}
