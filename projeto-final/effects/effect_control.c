#include "ezdsp5502.h"
#include <stdlib.h>
#include <stdio.h>
#include "pitch_shifter.h"
#include "reverb.h"
#include "flanger.h"
#include "tremolo.h"
#include "effect_control.h"
#define MAX_BUF_SIZE 256

int sampleRate = 44100;
Int8 temp[2 * MAX_BUF_SIZE]; // Buffer temporario para armazenar os dados de audio

void apply_effect(FILE *fpIn, FILE *fpOut, int effect, Uint32 *cnt)
{
	int16_t sampleBuffer[MAX_BUF_SIZE]; // 16-bit buffer for audio samples
	Uint32 position = 0;				// Persistent position tracking for tremolo
	size_t bytesRead = 0;				// Bytes read from input
	size_t i;

	printf("Applying effect: %d\n", effect);

	// Loop until end of file or input stream
	while ((bytesRead = fread(temp, sizeof(Int8), 2 * MAX_BUF_SIZE, fpIn)) > 0)
	{
		size_t numSamples = bytesRead / 2; // Convert byte count to sample count

		// Convert input buffer to 16-bit samples
		for (i = 0; i < numSamples; i++)
		{
			sampleBuffer[i] = (temp[2 * i] & 0xFF) | (temp[2 * i + 1] << 8);
		}

		switch (effect)
		{
		case 0: // "REV HALL1"
			apply_reverb_simple(sampleBuffer, numSamples, sampleRate, 0.100f, 0.85f);
			break;
		case 1: // "REV ROOM2"
			apply_reverb_simple(sampleBuffer, numSamples, sampleRate, 0.030f, 0.6f);
			break;
		case 2: // "REV STAGE B"
			apply_reverb_simple(sampleBuffer, numSamples, sampleRate, 0.050f, 0.7f);
			;
			break;
		case 3: // "REV STAGE D"
			apply_reverb_simple(sampleBuffer, numSamples, sampleRate, 0.070f, 0.8f);
			break;
		case 4: // "REV STAGE F"
			apply_reverb_simple(sampleBuffer, numSamples, sampleRate, 0.090f, 0.85f);
			break;
		case 5: // "RET STAGE Gb"
			apply_reverb_simple(sampleBuffer, numSamples, sampleRate, 0.120f, 0.9f);
			break;
		case 6: // "FLANGER"
			apply_flanger(sampleBuffer, numSamples, sampleRate, 7.0f, 15.0f, 1.0f);
			break;
		case 7: // "TREMOLO"
			apply_tremolo(sampleBuffer, numSamples, sampleRate, 500, &position);
			break;
		default:
			printf("Error: Undefined effect ID %d\n", effect);
			break;
		}

		// Convert samples back to 8-bit (little-endian) for output
		for (i = 0; i < numSamples; i++)
		{
			temp[2 * i] = sampleBuffer[i] & 0xFF;
			temp[2 * i + 1] = (sampleBuffer[i] >> 8) & 0xFF;
		}

		// Write processed audio to output stream
		fwrite(temp, sizeof(Int8), bytesRead, fpOut);

		*cnt += numSamples; // Update total sample count
		printf("%ld samples processed\n", *cnt);
	}
}
