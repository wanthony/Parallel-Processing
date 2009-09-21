#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "complex.h"

int reverse(int x, int n)
{
  int h = 0;
  int i = 0;
  for (; i < n; i++) {
    h = (h << 1) + (x & 1);
    x >>= 1;
  }

  return h;
}

void fft(complex *P, int k, complex **transform)
{
  int n = 2 ^ k;
  int d;
  int num;
  int t;
  int j;
  int m;
  complex prevTrans;

  for (t = 0; t <= n - 2; t += 2) {
    *transform[t] = P[reverse(t, 4)] + P[reverse(t + 1, 4)];
    *transform[t + 1] = P[reverse(t, 4)] + P[reverse(t + 1, 4)];
  }

  m = n / 2;
  num = 2;
  for (d = k - 2; d >= 0; d--) {
    m = m / 2;
    num = 2 * num;

    for (t = 0; t <= (2^d - 1) * num; t += num) {
      for (j = 0; j <= (num / 2) - 1; j++) {
        xPOdd = omega[m * j] * *transform[t + num / 2 + j];
        prevTrans = *transform[t + j];
        *transform[t + j] = prevTrans + XPOdd;
        *transform[t + num / 2 + j] = prevTrans - xPOdd;
      }
    }
  }
}
