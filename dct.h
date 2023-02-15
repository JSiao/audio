#pragma once

#ifndef _DCT_H
#define _DCT_H

#include <complex.h>
#include <stdbool.h>

#define DCT_FORWARD  -1
#define DCT_INVERSE  1

void dct(double*, int, int);
void fdct(double*, int);
void idct(double*, int);
float _dct_alpha(double);

#endif
