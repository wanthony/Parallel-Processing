#include <stdio.h>
#include <mpi.h>

/* Parallel Trapazoid with input */

float Trap(float local_a, float local_b, int local_n, float h);
float f(float x);

void Get_data(float *a_ptr, float *b_ptr, int *n_ptr, int my_rank, int p);

int main(int argc, char *argv[])
{
  int my_rank;
  int p;
  float a = 0.0;
  float b = 1.0f;
  int n = 1024;
  float h;
  float local_a;
  float local_b;
  int local_n;
  float integral;
  float total;
  int source;
  int dest = 0;
  int tag = 0;
  MPI_Status status;

  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &p);

  Get_data(&a, &b, &n, my_rank, p);

  h = (b - a) / n;
  local_n = n / p;

  local_a = a + my_rank* local_n * h;
  local_b = local_a + local_n * h;
  integral = Trap(local_a, local_b, local_n, h);

  if (my_rank == 0)
  {
    total = integral;
    for (source = 1; source < p; source++)
    {
      MPI_Recv(&integral, 1, MPI_FLOAT, source, tag, MPI_COMM_WORLD, &status);
      total = total + integral;
    }
  }
  else
  {
    MPI_Send(&integral, 1, MPI_FLOAT, dest, tag, MPI_COMM_WORLD);
  }

  if (my_rank == 0)
  {
    printf("With n = %d trapezoids, our estimate\n", n);
    printf("of the integral from %f to %f = %f\n", a, b, total);
  }

  MPI_Finalize();
}

float Trap(float local_a, float local_b, int local_n, float h)
{
  float integral;
  float x;
  int i;

  integral = (f(local_a) + f(local_b)) / 2.0;
  x = local_a;
  for (i = 1; i <= local_n - 1; i++)
  {
    x = x + h;
    integral = integral + f(x);
  }

  integral = integral * h;
  return integral;
}

float f(float x)
{
  return 10;
}

void Get_data(float *a_ptr, float *b_ptr, int *n_ptr, int my_rank, int p)
{
  int source = 0;
  int dest;
  int tag;
  MPI_Status status;

  if (my_rank == 0) {
    printf("Enter a, b, and n\n");
    scanf("%f %f %d", a_ptr, b_ptr, n_ptr);
    for (dest = 1; dest < p; dest++) {
      tag = 0;
      MPI_Send(a_ptr, 1, MPI_FLOAT, dest, tag, MPI_COMM_WORLD);
      tag = 1;
      MPI_Send(b_ptr, 1, MPI_FLOAT, dest, tag, MPI_COMM_WORLD);
      tag = 2;
      MPI_Send(n_ptr, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
    }
  } else {
    tag = 0;
    MPI_Recv(a_ptr, 1, MPI_FLOAT, source, tag, MPI_COMM_WORLD, &status);
    tag = 1;
    MPI_Recv(b_ptr, 1, MPI_FLOAT, source, tag, MPI_COMM_WORLD, &status);
    tag = 2;
    MPI_Recv(n_ptr, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
  }
}
