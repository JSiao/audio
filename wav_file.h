#pragma once

#ifndef _WAV_FILE_H
#define _WAV_FILE_H

#include <stdint.h>

struct raw_file
{
  size_t size;
  void *data;
};

struct wav_file
{
  // riff chunk
  char     riff_id[4];
  uint32_t riff_size;
  char     riff_format[4];
  // fmt chunk
  char     fmt_id[4];
  uint32_t fmt_size;
  uint16_t fmt_audio_fmt;
  uint16_t fmt_channels;
  uint32_t fmt_sample_rate;
  uint32_t fmt_byte_rate;   // == sample_rate * channels * bps / 8
  uint16_t fmt_block_align; // == channels * bps / 8
  uint16_t fmt_bps;
  // data chunk
  char     data_id[4];
  uint32_t data_size;       // == #samples * channels * bps/8
  void *channel1;
  void *channel2;
};

//Raw data
struct raw_file *file_to_raw(const char *);
void raw_to_file(const char *, struct raw_file *);
void file_delete(struct raw_file *);
//Wav data
struct wav_file *raw_to_wav(struct raw_file *);
struct raw_file *wav_to_raw(struct wav_file *);
void wav_delete(struct wav_file*);

#endif
