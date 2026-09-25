#include <cstdio>

int main()
{
  printf("Oh,");

#pragma omp parallel num_threads(3)
  {
    printf(" no");
  }

  printf("!\n");
  return 0;
}
