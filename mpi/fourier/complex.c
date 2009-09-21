#include "complex.h"

complex add_complex(complex a, complex b)
{
  complex sum;

  sum.real = a.real + b.real;
  sum.imag = a.imag + b.imag;

  return sum;
}

complex sub_complex(complex a, complex b)
{
  complex dif;

  dif.real = a.real - b.real;
  dif.imag = a.imag - b.imag;

  return dif;
}

/* (a + bi)(c + di)
 * ac + adi + bci + dbi^2
 * ac + adi + bci - db
 * (ac - db) + (ad + bc)i */
complex mul_complex(complex a, complex b)
{
  complex prod;

  prod.real = (a.real * b.real - b.imag * a.imag);
  prod.imag = (a.real * b.imag + a.imag * b.real);

  return prod;
}
