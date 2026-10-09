#include <cstdio>
#include <cuda_runtime.h>
#define N 10

__global__ void multiplyByTwo(float *dA, int n)
{
  int i = blockIdx.x * blockDim.x + threadIdx.x;
  if (i < n)           // beyond n: nothing
    dA[i] = 2 * dA[i]; // my own cell
}

int main()
{
  float A[N];
  for (int i = 0; i < N; i++) A[i] = i + 1;
  float *dA;                // GPU address
  cudaMalloc(&dA, N * sizeof(float));
  cudaMemcpy(dA, A, N * sizeof(float),
             cudaMemcpyHostToDevice);

  int T = 4;                // threads per block
  int B = (N + T - 1) / T;  // = 3
  multiplyByTwo<<<B, T>>>(dA, N);

  cudaMemcpy(A, dA, N * sizeof(float),
             cudaMemcpyDeviceToHost);
  cudaFree(dA);
  for (int i = 0; i < N; i++) printf("%g ", A[i]);
  printf("\n");
  return 0;
}
