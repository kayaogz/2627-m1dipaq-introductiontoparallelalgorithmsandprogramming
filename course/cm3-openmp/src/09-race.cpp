#include <cstdio>
#include <omp.h>
#define N 1000000

int main()
{
  int n = 0;   // 4 threads x N increments: counts to 4N

#pragma omp parallel num_threads(4)
  {
    for (int k = 0; k < N; k++)
      n = n + 1;              // all 4 threads write n
  }

  printf("n = %d, expected %d\n", n, 4 * N);
  return 0;
}
