#ifndef AUDIO_UTILS_H
#define AUDIO_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include "ezdsp5502.h"

// Global WAV header (used for manipulating WAV files)
extern Uint8 waveHeader[44];

// Function to open a WAV file for reading
FILE *open_wav_file(const char *filename);

// Function to create a new file for writing
FILE *create_file(const char *filename);

// Function to copy the WAV header from input file to output file
void write_header(FILE *fpIn, FILE *fpOut);

// Function to create the WAV header based on the given parameters
void wHeader(Uint8 *w, float f1, float f2, Uint32 bytes);

// Function to adjust and write the header with the updated parameters
void ajust_header(FILE *fpOut, Uint32 cnt);

#endif // AUDIO_UTILS_H
