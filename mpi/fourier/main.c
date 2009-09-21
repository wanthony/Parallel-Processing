#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "complex.h"

int main(int argc, char *argv[])
{
  complex *F;
  int i, j, n;

  printf("Please enter the size of the matrix: ");
  scanf("%d", &n);

  F = (complex*)malloc((n * n) * sizeof(complex));

  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++) {
      F[i * n + j].real = cos(((2 * M_PI) / n) * i * j);
      F[i * n + j].imag = sin(((2 * M_PI) / n) * i * j);
      printf("\t%.2f + %.2fi\t|", F[i * n + j].real, F[i * n + j].imag);
    }
    printf("\n");
  }

  return 0;
}
