#include <cstdio>
#include <omp.h>

int main()
{
  int x = 3;                 // before the block: shared
  int A[4] = {0, 0, 0, 0};   // before the block: shared

#pragma omp parallel num_threads(4)
  {
    int thid = omp_get_thread_num(); // inside: private
    int y = x + thid;                // inside: private
    A[thid] = y;                     // my own cell of A
    printf("thread %d: x = %d, y = %d\n", thid, x, y);
  }

  printf("A = %d %d %d %d\n", A[0], A[1], A[2], A[3]);
  return 0;
}
