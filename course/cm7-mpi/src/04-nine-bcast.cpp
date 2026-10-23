#include <cstdio>
#include <unistd.h>
#include <chrono>
#include <mpi.h>

double now() { // wall-clock time in seconds
  using namespace std::chrono;
  auto t = high_resolution_clock::now().time_since_epoch();
  return duration<double>(t).count();
}

#define UNIT 100000 // 1 unit = 0.1 s (in microseconds)
int computeTwo()   { usleep(2 * UNIT); return 2; }
int computeThree() { usleep(3 * UNIT); return 3; }
int computeFour()  { usleep(4 * UNIT); return 4; }

int main(int argc, char **argv)
{
  MPI_Init(&argc, &argv);
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  double t0 = now();

  int two = 0, three = 0, four = 0;
  if (rank == 0) two = computeTwo();
  if (rank == 1) three = computeThree();
  if (rank == 2) four = computeFour();

  // the partial results are merged in process 0
  if (rank == 1)
    MPI_Send(&three, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
  if (rank == 2)
    MPI_Send(&four, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
  if (rank == 0) {
    MPI_Recv(&three, 1, MPI_INT, 1, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    MPI_Recv(&four, 1, MPI_INT, 2, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
  }
  int nine = two + three + four;

  // the mirror: process 0 sends the result to the others
  if (rank == 0) {
    MPI_Send(&nine, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    MPI_Send(&nine, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
  }
  if (rank == 1 || rank == 2)
    MPI_Recv(&nine, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);

  double t1 = now();
  printf("process %d: nine = %d, computed in %.2f s\n",
         rank, nine, t1 - t0);
  MPI_Finalize();
  return 0;
}
