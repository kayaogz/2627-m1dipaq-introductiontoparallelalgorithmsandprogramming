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
    two = computeTwo();
    three = computeThree();
    four = computeFour();
    nine = two + three + four;
  }

  double t1 = now();
  printf("nine = %d, computed in %.2f s\n", nine, t1 - t0);
  return 0;
}
