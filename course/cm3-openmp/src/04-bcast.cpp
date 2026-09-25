#include <cstdio>
#include <unistd.h>
#include <omp.h>

int main()
{
  int e = 0;

#pragma omp parallel num_threads(4)
  {
    int thid = omp_get_thread_num();
    if (thid == 0) {
      usleep(100000);   // thread 0 takes 0.1 s to produce e
      e = 42;
    }
#pragma omp barrier   // wait here until everyone is here
    printf("thread %d sees e = %d\n", thid, e);
  }
  return 0;
}
