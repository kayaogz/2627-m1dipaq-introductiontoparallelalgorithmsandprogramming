#include <cstdio>
#include <unistd.h>
#include <omp.h>
#include <chrono>

double now() { // wall-clock time in seconds (not OpenMP)
  using namespace std::chrono;
  auto t = high_resolution_clock::now().time_since_epoch();
  return duration<double>(t).count();
}

#define UNIT 100000 // 1 unit of time = 0.1 s (microseconds)
int computeTwo()   { usleep(2 * UNIT); return 2; }
int computeThree() { usleep(3 * UNIT); return 3; }
int computeFour()  { usleep(4 * UNIT); return 4; }

int main()
{
  int two = 0, three = 0, four = 0, nine = 0;
  double t0 = now();

#pragma omp parallel num_threads(3)
  {
    int thid = omp_get_thread_num();
#pragma omp sections
    {
#pragma omp section
      { two = computeTwo();
        printf("two: %d\n", thid); }
#pragma omp section
      { three = computeThree();
        printf("three: %d\n", thid); }
#pragma omp section
      { four = computeFour();
        printf("four: %d\n", thid); }
    } // implicit barrier: two, three, four are ready

#pragma omp sections // a single section: one thread does it
    {
#pragma omp section
      { nine = two + three + four;
        printf("sum: %d\n", thid); }
    }
  }

  double t1 = now();
  printf("nine = %d, computed in %.2f s\n", nine, t1 - t0);
  return 0;
}
