#include "effects/tremolo.h"
#include "effects/reverb.h"
#include "effects/flanger.h"
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

  // copy the data to use in tremolo
  int16_t *tremolo_data = (int16_t *)malloc(numSamples * sizeof(int16_t));
  memcpy(tremolo_data, data, numSamples * sizeof(int16_t));

  // copy the data to use in tremolo
  int16_t *flanger_data = (int16_t *)malloc(numSamples * sizeof(int16_t));
  memcpy(flanger_data, data, numSamples * sizeof(int16_t));

  // Apply the reverb effect
  apply_reverb(reverb_data, numSamples, 0.5f * header.sampleRate, 0.5f);
  const char *reverb_output = "reverb_output.wav";
  write_wav(reverb_output, &header, reverb_data);
  printf("Reverb effect applied and saved to %s\n", reverb_output);
  free(reverb_data);

  // Apply the tremolo effect
  apply_tremolo(tremolo_data, numSamples, header.sampleRate, 500);
  const char *tremolo_output = "tremolo_output.wav";
  write_wav(tremolo_output, &header, tremolo_data);
  printf("Tremolo effect applied and saved to %s\n", tremolo_output);
  free(tremolo_data);

  // Apply the tremolo effect
  apply_flanger(flanger_data, numSamples, header.sampleRate, 5.0f, 2.0f, 0.25f); // Parameters: delay, depth, rate
  const char *flanger_output = "flanger_output.wav";
  write_wav(flanger_output, &header, flanger_data);
  printf("Flanger effect applied and saved to %s\n", flanger_output);
  free(flanger_data);

  // Free allocated memory
  free(data);

  return 0;
}
