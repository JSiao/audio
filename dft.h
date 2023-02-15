#pragma once

#ifndef _DFT_H
#define _DFT_H

#include <complex.h>
#include <stdbool.h>

#define DFT_FORWARD  -1
#define DFT_INVERSE  1

void dft(long double complex *, int, int);

#endif
