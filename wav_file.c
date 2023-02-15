#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "wav_file.h"

// Raw functions
struct raw_file *file_to_raw(const char *file_name)
{
  FILE *temp_file=fopen(file_name, "r");
  if (!temp_file) return NULL;
  fseek(temp_file, 0L, SEEK_END);
  int size = ftell(temp_file);
  rewind(temp_file);
  char *text = (char *)malloc(size + 1);
  fread(text, 1, size, temp_file);
  text[size] = 0;
  fclose(temp_file);
  struct raw_file *ret_val = malloc(sizeof(struct raw_file));
  ret_val->size = size;
  ret_val->data = text;
  return ret_val;
}

void raw_to_file(const char *file_name, struct raw_file *fp)
{
  FILE *temp_file=fopen(file_name, "w");
  if (!temp_file) return;
  if (!(fp)) return;
  fwrite(fp->data, 1, fp->size, temp_file);
}

void file_delete(struct raw_file *fp)
{
  free((void*)fp->data);
  fp->data = NULL;
  free((void*)fp);
}

//Wav functions
struct wav_file *raw_to_wav(struct raw_file *rp)
{
  if (!rp) return NULL;
  struct wav_file *ret_val = (struct wav_file *)malloc(sizeof(struct wav_file));
  char *ptr = rp->data;
  bool flag = true;
  //* Parse header
  //  - Confirm that RIFF preamble exists
  if (*(uint32_t*)ptr != 0x46464952)
  {
    printf("Error: Riff ID is invalid\n");
    free((void *)ret_val);
    ret_val = NULL;
    return NULL;
  }
  else
  {
    *(uint32_t *)ret_val->riff_id = *(uint32_t*)ptr;
    printf("%d\n", *(uint32_t*)ret_val->riff_id);
  }
  ptr+=4;
  //  - Fills in riff size
  ret_val->riff_size = *(uint32_t*)ptr;
  if (ret_val->riff_size != (rp->size - 8))
  {
    printf("Error: Wav file size is not consistent\n");
    free((void *)ret_val);
    ret_val = NULL;
    return NULL;
  }
  ptr+=4;
  //  - Parse in riff format
  if (*(uint32_t*)ptr != 0x45564157)
  {
    printf("Error: Riff format is invalid\n");
    free((void *)ret_val);
    ret_val = NULL;
    return NULL;
  }
  {
    *(uint32_t *)ret_val->riff_format = *(uint32_t*)ptr;
  }
  ptr += 4;
  //  - Parse in format id
  if (*(uint32_t*)ptr != 0x20746d66)
  {
    printf("Error: Format ID is invalid\n");
    free((void *)ret_val);
    ret_val = NULL;
    return NULL;
  }
  else
  {
    *(uint32_t *)ret_val->fmt_id = *(uint32_t*)ptr;
  }
  ptr += 4;
  //  - Parse in subchunk size
  ret_val->fmt_size = *(uint32_t *)ptr;
  ptr += 4;
  //  - Parse in audio format
  ret_val->fmt_audio_fmt = *(uint16_t *) ptr;
  if (ret_val->fmt_audio_fmt != 1)
  {
    printf("Error: Unrecognized audio format\n");
    free((void *)ret_val);
    ret_val = NULL;
    return NULL;
  }
  ptr += 2;
  //  - Parse in number of channels
  ret_val->fmt_channels = *(uint16_t *) ptr;
  if (ret_val->fmt_channels == 1)
  {
    ret_val->channel2 = NULL;
  }
  else if (ret_val->fmt_channels != 2)
  {
    printf("Error: Invalid number of channels\n");
    free((void *)ret_val);
    ret_val = NULL;
    return NULL;
  }
  ptr += 2;
  //  - Parse in sample rate
  ret_val->fmt_sample_rate = *(uint32_t *)ptr;
  ptr += 4;
  //  - Parse in Byte rate
  ret_val->fmt_byte_rate   = *(uint32_t *)ptr;
  ptr += 4;
  //  - Parse in Block Aligm
  ret_val->fmt_block_align = *(uint16_t *)ptr;
  ptr += 2;
  //  - Parse Bits Per Sample
  if (ret_val->fmt_bps == 8 && ret_val->fmt_bps == 16)
  {
    printf("Error: Invalid number of bits per second\n");
    free((void *)ret_val);
    ret_val = NULL;
    return NULL;
  }
  ret_val->fmt_bps         = *(uint32_t *)ptr;
  ptr += 2;
  //  - Parse Date ID
  if (*(uint32_t*)ptr != 0x61746164)
  {
    printf("Error: Data ID is invalid\n");
    free((void *)ret_val);
    ret_val = NULL;
    return NULL;
  }
  else
  {
    *(uint32_t *)ret_val->data_id = *(uint32_t*)ptr;
  }
  ptr += 4;
  // Parse in data size
  ret_val->data_size   = *(uint32_t *)ptr;
  ptr += 4;
  // Parse in Data... Oh boy.

  uint16_t bytes_per_sample = ret_val->fmt_bps / 8;
  size_t channel_size = ret_val->data_size / (ret_val->fmt_channels * bytes_per_sample);
  ret_val->channel1 = malloc(channel_size * ret_val->fmt_bps / 8);
  if (ret_val->fmt_channels == 2)
  {
    ret_val->channel2 = malloc(channel_size * ret_val->fmt_bps / 8);
  }
  char *ptr1 = ret_val->channel1;
  char *ptr2 = ret_val->channel2;
  for (size_t x = 0; x < channel_size; x++)
  {
    *ptr1 = *ptr;
    ptr++;
    ptr1++;
    if (bytes_per_sample == 2)
    {
      *ptr1 = *ptr;
      ptr++;
      ptr1++;
    }
    if (ret_val->fmt_channels == 2)
    {
      *ptr2 = *ptr;
      ptr++;
      ptr2++;
      if (bytes_per_sample == 2)
      {
        *ptr2 = *ptr;
        ptr++;
        ptr2++;
      }
    }
  }
  return ret_val;
}

struct raw_file *wav_to_raw(struct wav_file *wp)
{
  if (!wp) return NULL;
  struct raw_file *ret_val = malloc(sizeof(struct raw_file));
  size_t size = wp->riff_size + 9;
  ret_val->size = size;
  ret_val->data = malloc(size);
  char *val = ret_val->data;
  *(uint32_t *) val = *(uint32_t *)wp->riff_id;
  val += 4;
  *(uint32_t *) val = wp->riff_size;
  val += 4;
  *(uint32_t *) val = *(uint32_t *)wp->riff_format;
  val += 4;
  *(uint32_t *) val = *(uint32_t *)wp->fmt_id;
  val += 4;
  *(uint32_t *) val = wp->fmt_size;
  val += 4;
  *(uint16_t *) val = wp->fmt_audio_fmt;
  val += 2;
  *(uint16_t *) val = wp->fmt_channels;
  val += 2;
  *(uint32_t *) val = wp->fmt_sample_rate;
  val += 4;
  *(uint32_t *) val = wp->fmt_byte_rate;
  val += 4;
  *(uint16_t *) val = wp->fmt_block_align;
  val += 2;
  *(uint16_t *) val = wp->fmt_bps;
  val += 2;
  *(uint32_t *) val = *(uint32_t *)wp->data_id;
  val += 4;
  *(uint32_t *) val = wp->data_size;
  val += 4;

  uint16_t bytes_per_sample = wp->fmt_bps / 8;
  size_t channel_size = wp->data_size / (wp->fmt_channels * bytes_per_sample);
  
  char *ptr1 = wp->channel1;
  char *ptr2 = wp->channel2;
  for (size_t x = 0; x < channel_size; x++)
  {
    *val = *ptr1;
    val++;
    ptr1++;
    if (bytes_per_sample == 2)
    {
      *val = *ptr1;
      val++;
      ptr1++;
    }
    if (wp->fmt_channels == 2)
    {
      *val = *ptr2;
      val++;
      ptr2++;
      if (bytes_per_sample == 2)
      {
        *val = *ptr2;
        val++;
        ptr2++;
      }
    }
  }
  return ret_val;
}

void wav_delete(struct wav_file *ptr)
{
  if (!ptr) return;
  free(ptr->channel1);
  free(ptr->channel2);
  ptr->channel1 = NULL;
  ptr->channel2 = NULL;
  free((void *)ptr);
}
