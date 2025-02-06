#include "effects/tremolo.h"
#include "effects/reverb.h"
#include "effects/flanger.h"
#include "effects/pitch_shifter.h"
#include "file.h"
#include <stdio.h>
#include <string.h>

int main()
{
  const char *inputFile = "input.wav";

  // Read file
  WAVHeader header;
  int16_t *data = read_wav(inputFile, &header);
  if (!data)
    return 1;

  uint32_t numSamples = header.subchunk2Size / (header.bitsPerSample / 8);

  // Copy for the effect array
  int16_t *reverb_data = (int16_t *)malloc(numSamples * sizeof(int16_t));
  memcpy(reverb_data, data, numSamples * sizeof(int16_t));

  int16_t *tremolo_data = (int16_t *)malloc(numSamples * sizeof(int16_t));
  memcpy(tremolo_data, data, numSamples * sizeof(int16_t));

  int16_t *flanger_data = (int16_t *)malloc(numSamples * sizeof(int16_t));
  memcpy(flanger_data, data, numSamples * sizeof(int16_t));

  int16_t *pitch_data = (int16_t *)malloc(numSamples * sizeof(int16_t));
  memcpy(pitch_data, data, numSamples * sizeof(int16_t));

  // Apply effect
  apply_reverb_simple(reverb_data, numSamples, header.sampleRate, 0.5f, 0.5f);
  // apply_reverb_hall(reverb_data, numSamples, header.sampleRate, 0.5f, 0.5f);
  // apply_reverb_shroeder(reverb_data, numSamples, header.sampleRate);
  const char *reverb_output = "reverb_output.wav";
  write_wav(reverb_output, &header, reverb_data);
  printf("Reverb effect applied and saved to %s\n", reverb_output);
  free(reverb_data);

  apply_tremolo(tremolo_data, numSamples, header.sampleRate, 500);
  const char *tremolo_output = "tremolo_output.wav";
  write_wav(tremolo_output, &header, tremolo_data);
  printf("Tremolo effect applied and saved to %s\n", tremolo_output);
  free(tremolo_data);

  apply_flanger(flanger_data, numSamples, header.sampleRate, 5.0f, 2.0f, 0.25f);
  const char *flanger_output = "flanger_output.wav";
  write_wav(flanger_output, &header, flanger_data);
  printf("Flanger effect applied and saved to %s\n", flanger_output);
  free(flanger_data);

  apply_pitch_shifter(pitch_data, numSamples, header.sampleRate, 1.5f);
  const char *pitch_shifter_output = "pitch_shifter_output.wav";
  write_wav(pitch_shifter_output, &header, pitch_data);
  printf("Pitch_shifter effect applied and saved to %s\n", pitch_shifter_output);
  free(pitch_data);

  free(data);

  return 0;
}
