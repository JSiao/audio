#include <math.h>
#include <complex.h>
#include <stdio.h>
#include <stdbool.h>
#include "dft.h"
#include "wav_file.h"
#include "sound.h"


int main(int argc, char **argv)
{
  struct raw_file *in_file  = file_to_raw("in.wav");
  struct wav_file *in_wav   = raw_to_wav(in_file);
  pitch_shift(in_wav);
  struct raw_file *out_file = wav_to_raw(in_wav);
  raw_to_file("out.wav", out_file);
  file_delete(in_file);
  file_delete(out_file);
  wav_delete(in_wav);
  return 0;
}
