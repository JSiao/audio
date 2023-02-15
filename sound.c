#include <stdlib.h>
#include <stdio.h>
#include <complex.h>
#include <math.h>
#include "sound.h"

void pitch_shift(struct wav_file *wp)
{
  int channels = wp->fmt_channels;
  
  uint16_t bytes_per_sample = wp->fmt_bps / 8;
  size_t channel_size = wp->data_size / (wp->fmt_channels * bytes_per_sample);
  
  long double complex *temp_c1 = (long double complex *)malloc(sizeof(long double complex) * channel_size);
  long double complex *temp_f1 = (long double complex *)malloc(sizeof(long double complex) * channel_size);
  long double complex *temp_c2 = NULL;
  long double complex *temp_f2 = NULL;
  if (channels == 2)
  {
    temp_c2 = (long double complex *)malloc(sizeof(long double complex) * channel_size);
    temp_f2 = (long double complex *)malloc(sizeof(long double complex) * channel_size);
  }
  char *ptr1 = wp->channel1;
  char *ptr2 = wp->channel2;

  for (int x = 0; x < channel_size; x++)
  {
    if (bytes_per_sample == 2)
      temp_c1[x] = *(uint16_t *)ptr1;
    else
      temp_c1[x] = *ptr1;
    ptr1 += bytes_per_sample;

    if (channels == 2)
    {
      if (bytes_per_sample == 2)
        temp_c2[x] = *(uint16_t *)ptr2;
      else
        temp_c2[x] = *ptr2;
      ptr2 += bytes_per_sample;
    }
  }

  dft(temp_c1, channel_size, DFT_FORWARD); 
  if (channels == 2)
    dft(temp_c2, channel_size, DFT_FORWARD); 
  for (int x = 0; x < channel_size; x++)
  {
    temp_f1[x] = temp_c1[x/2] / 2;
    if (channels == 2)
      temp_f2[x] = temp_c2[x/2] / 2;
  }

  dft(temp_f1, channel_size, DFT_INVERSE); 
  if (channels == 2)
    dft(temp_f2, channel_size, DFT_INVERSE); 

  ptr1 = (char *)wp->channel1;
  ptr2 = (char *)wp->channel2;

  for (int x = 0; x < channel_size; x++)
  {
    if (bytes_per_sample == 2)
      *(uint16_t *)ptr1 = (uint16_t)creal(temp_f1[x]);
    else
      *ptr1 = (char)creal(temp_f1[x]);
    ptr1 += bytes_per_sample;

    if (channels == 2)
    {
    if (bytes_per_sample == 2)
      *(uint16_t *)ptr2 = (uint16_t)creal(temp_f2[x]);
    else
      *ptr2 = (char)creal(temp_f2[x]);
    ptr2 += bytes_per_sample;
    }
  }

  free (temp_c1);
  free (temp_f1);
  if (temp_c2) free(temp_c2);
  if (temp_f2) free(temp_f2);
}
