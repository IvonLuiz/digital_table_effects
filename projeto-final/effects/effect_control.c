#include "ezdsp5502.h"
#include <stdlib.h>
#include <stdio.h>
#include "pitch_shifter.h"
#include "reverb.h"
#include "flanger.h"
#include "tremolo.h"
#include "effect_control.h"
#define MAX_BUF_SIZE 256

int sampleRate = 8000;
Int8 temp[2 * MAX_BUF_SIZE];

void apply_effect(FILE *fpIn, FILE *fpOut, int effect, Uint32 *cnt)
{
	int16_t sampleBuffer[MAX_BUF_SIZE];
	Uint32 position = 0;
	size_t bytesRead = 0;
	size_t i;

	printf("Applying effect: %d\n", effect);

	while ((bytesRead = fread(temp, sizeof(Int8), 2 * MAX_BUF_SIZE, fpIn)) > 0)
	{
		size_t numSamples = bytesRead / 2;

		for (i = 0; i < numSamples; i++)
		{
			sampleBuffer[i] = (temp[2 * i] & 0xFF) | (temp[2 * i + 1] << 8);
		}

		switch (effect)
		{
		case 0:
			apply_reverb_simple(sampleBuffer, numSamples, sampleRate, 0.100f, 0.85f);
			break;
		case 1:
			apply_reverb_simple(sampleBuffer, numSamples, sampleRate, 0.030f, 0.6f);
			break;
		case 2:
			apply_reverb_simple(sampleBuffer, numSamples, sampleRate, 0.050f, 0.7f);
			break;
		case 3:
			apply_reverb_simple(sampleBuffer, numSamples, sampleRate, 0.070f, 0.8f);
			break;
		case 4:
			apply_reverb_simple(sampleBuffer, numSamples, sampleRate, 0.090f, 0.85f);
			break;
		case 5:
			apply_reverb_simple(sampleBuffer, numSamples, sampleRate, 0.120f, 0.9f);
			break;
		case 6:
			apply_flanger(sampleBuffer, numSamples, sampleRate, 7.0f, 15.0f, 1.0f);
			break;
		case 7:
			apply_tremolo(sampleBuffer, numSamples, sampleRate, 500, &position);
			break;
		default:
			printf("Error: Undefined effect ID %d\n", effect);
			break;
		}

		for (i = 0; i < numSamples; i++)
		{
			temp[2 * i] = sampleBuffer[i] & 0xFF;
			temp[2 * i + 1] = (sampleBuffer[i] >> 8) & 0xFF;
		}

		fwrite(temp, sizeof(Int8), bytesRead, fpOut);

		*cnt += numSamples;
		printf("%ld samples processed\n", *cnt);
	}
}
