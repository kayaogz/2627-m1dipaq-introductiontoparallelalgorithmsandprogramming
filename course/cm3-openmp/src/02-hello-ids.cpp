#include <cstdio>
#include <omp.h>

int main()
{
  printf("before: thread %d of %d\n",
         omp_get_thread_num(), omp_get_num_threads());

#pragma omp parallel num_threads(4)
  {
    int thid = omp_get_thread_num();   // my index
    int numth = omp_get_num_threads(); // how many of us
    printf("I am thread %d of %d\n", thid, numth);
  }

  printf("after: thread %d of %d\n",
         omp_get_thread_num(), omp_get_num_threads());
  return 0;
}
