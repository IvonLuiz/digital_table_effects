#include "effects.h"
#include "file.h"
#include <stdio.h>
#include <string.h>

int main()
{
  const char *inputFile = "input.wav";
  // const char *outputFile = "output.wav";

  // Read input WAV file
  WAVHeader header;
  int16_t *data = read_wav(inputFile, &header);
  if (!data)
    return 1;

  uint32_t numSamples = header.subchunk2Size / (header.bitsPerSample / 8);

  // copy the data to use in reverb
  int16_t *reverb_data = (int16_t *)malloc(numSamples * sizeof(int16_t));
  memcpy(reverb_data, data, numSamples * sizeof(int16_t));
  // Apply the reverb effect
  apply_reverb(reverb_data, numSamples, 0.5f * header.sampleRate, 0.5f);
  const char *reverb_output = "reverb_output.wav";
  write_wav(reverb_output, &header, reverb_data);
  printf("Reverb Effect applied and saved to %s\n", reverb_output);
  free(reverb_data);

  // Free allocated memory
  free(data);

  return 0;
}
