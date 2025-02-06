#ifndef WAV_H
#define WAV_H

#include <stdio.h>
#include <stdlib.h>
#include "ezdsp5502.h"

FILE *open_wav_file(const char *filename);

FILE *create_file(const char *filename);

void write_header(FILE *fpIn, FILE *fpOut);

void wHeader(Uint8 *w, float f1, float f2, Uint32 bytes);

void ajust_heeader(FILE *fpOut, Uint32 cnt);

#endif // WAV_H
