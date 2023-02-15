#include <stdlib.h>
#include <math.h>
#include "dft.h"

void dct(double* data, int N, int inv)
{
  if (inv == DCT_FORWARD)
  {
    fdct(data, int);
  }
  else
  {
    idct(data, int);
  }
}

void fdct(double* data, int N)
{
  if (!N) return;
  double* temp_array = malloc(sizeof(double) * N);
  for (unsigned int i = 0; i < N ; i++)
  {
    
    for (unsigned int j = 0; j < N; j++)
    {
    }
  }
  
}

void idct(double* data, int N)
{
}
