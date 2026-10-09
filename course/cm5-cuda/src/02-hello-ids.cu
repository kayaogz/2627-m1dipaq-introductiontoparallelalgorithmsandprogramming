#include <cstdio>
#include <cuda_runtime.h>

__global__ void hello()
{
  int i = blockIdx.x * blockDim.x + threadIdx.x; // my index
  int p = gridDim.x * blockDim.x;              // how many of us
  printf("I am thread %d of %d\n", i, p);
}

int main()
{
  hello<<<2, 4>>>();        // 2 blocks of 4 threads: 8 threads
  cudaDeviceSynchronize();
  return 0;
}
