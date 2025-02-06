#include "file.h"

int16_t *read_wav(const char *filename, WAVHeader *header) {
  FILE *file = fopen(filename, "rb");
  if (!file) {
    perror("Error opening file");
    return NULL;
  }

  // Read WAV header
  fread(header, sizeof(WAVHeader), 1, file);

  // Verify WAV format
  if (header->audioFormat != 1 || header->bitsPerSample != 16) {
    fprintf(stderr, "Unsupported WAV format (must be PCM, 16-bit).\n");
    fclose(file);
    return NULL;
  }

  // Allocate memory for audio data
  int16_t *data = malloc(header->subchunk2Size);
  if (!data) {
    perror("Error allocating memory");
    fclose(file);
    return NULL;
  }

  // Read audio data
  fread(data, header->subchunk2Size, 1, file);
  fclose(file);
  return data;
}

void write_wav(const char *filename, WAVHeader *header, int16_t *data) {
  FILE *file = fopen(filename, "wb");
  if (!file) {
    perror("Error opening file");
    return;
  }

  // Write WAV header
  fwrite(header, sizeof(WAVHeader), 1, file);

  // Write audio data
  fwrite(data, header->subchunk2Size, 1, file);
  fclose(file);
}
