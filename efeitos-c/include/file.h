#pragma once
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// WAV header
typedef struct
{
  char chunkID[4];        // "RIFF"
  uint32_t chunkSize;     // Size of the file minus 8 bytes
  char format[4];         // "WAVE"
  char subchunk1ID[4];    // "fmt "
  uint32_t subchunk1Size; // Size of the fmt chunk (16 for PCM)
  uint16_t audioFormat;   // Audio format (1 for PCM)
  uint16_t numChannels;   // Number of channels
  uint32_t sampleRate;    // Sampling frequency
  uint32_t byteRate;      // sampleRate * numChannels * bitsPerSample/8
  uint16_t blockAlign;    // numChannels * bitsPerSample/8
  uint16_t bitsPerSample; // Bits per sample (8, 16, 24, or 32)
  char subchunk2ID[4];    // "data"
  uint32_t subchunk2Size; // Data size
} WAVHeader;

int16_t *read_wav(const char *filename, WAVHeader *header);
void write_wav(const char *filename, WAVHeader *header, int16_t *data);
