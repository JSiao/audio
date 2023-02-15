#include <stdlib.h>
#include <math.h>
#include "dft.h"

void dft(long double complex *X, int N, int inv)
{
  if (!N) return;
  long double complex *temp_arr = (long double complex *) malloc(N * sizeof(long double complex));
  long double complex exponent = (long double complex) (inv * 2)/N * acos(-1.0f) * I;
  for (int n = 0; n < N; n++)
  {
    long double complex val = 0;
    for (int k = 0; k < N; k++)
    {
      val += (X[k] * cexpl(exponent * k * n));
    }
    temp_arr[n] = val;
    if (inv == 1)
    {
       temp_arr[n] /= N;
    }
  }
  for (int n = 0; n < N; n++)
  {
    X[n] = temp_arr[n];
  }
  free((void *)temp_arr);
}
