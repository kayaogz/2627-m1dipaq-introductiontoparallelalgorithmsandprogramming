#include <cstdio>
#include <omp.h>
#define N 1000000

int main()
{
  int n = 0, partial[4]; // partial[t]: count of thread t

#pragma omp parallel num_threads(4)
  {
    int thid = omp_get_thread_num();
    int mine = 0;           // private accumulator
    for (int k = 0; k < N; k++)
      mine = mine + 1;
    partial[thid] = mine;   // my own cell, no conflict
#pragma omp barrier
#pragma omp sections
    {
#pragma omp section
      {
        for (int t = 0; t < 4; t++)  // one thread combines
          n = n + partial[t];
      }
    } // implicit barrier
  } // implicit barrier: all the cells are written


  printf("n = %d, expected %d\n", n, 4 * N);
  return 0;
}
