#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "ezdsp5502.h"

#include "pitch_shifter.h"
#include "reverb.h"
#include "flanger.h"
#include "tremolo.h"
#include "effect_control.h"

#define MAX_BUF_SIZE 512
typedef unsigned char uint8_t;

int sampleRate = 8000;

Int16 inputBuffer[MAX_BUF_SIZE];
Int16 outputBuffer[MAX_BUF_SIZE];
uint8_t temp[2 * MAX_BUF_SIZE];
uint8_t temp2[2 * MAX_BUF_SIZE];

void apply_effect(FILE *fpIn, FILE *fpOut, int effect, Uint32 *cnt)
{
	Int16 i, j, sample; // Declare variables locally
	printf("Aplicando o efeito: %d\n", effect);

	while (fread(&temp, sizeof(uint8_t), 2 * MAX_BUF_SIZE, fpIn) == 2 * MAX_BUF_SIZE)
	{
		for (i = 0, j = 0; i < MAX_BUF_SIZE; i++)
		{
			inputBuffer[i] = (temp[j] & 0xFF) | (temp[j + 1] << 8);
			j += 2;
		}

		switch (effect)
		{
		case 0: // "REV HALL1"
			// apply_reverb_hall(inputBuffer, MAX_BUF_SIZE, sampleRate, 0.05f, 0.8f);
			apply_reverb_simple(inputBuffer, MAX_BUF_SIZE, sampleRate, 0.02f, 0.3f);
			break;
		case 1: // "REV ROOM2"
			apply_reverb_simple(inputBuffer, MAX_BUF_SIZE, sampleRate, 0.01f, 0.8f);
			// apply_reverb_shroeder(inputBuffer, MAX_BUF_SIZE, sampleRate);
			break;
		case 2: // "REV STAGE B"
			apply_reverb_simple(inputBuffer, MAX_BUF_SIZE, sampleRate, 0.03f, 0.5f);
			break;
		case 3: // "REV STAGE D"
			apply_reverb_simple(inputBuffer, MAX_BUF_SIZE, sampleRate, 0.035f, 0.75f);
			break;
		case 4: // "REV STAGE F"
			apply_reverb_simple(inputBuffer, MAX_BUF_SIZE, sampleRate, 0.04f, 0.85f);
			break;
		case 5: // "RET STAGE Gb"
			apply_reverb_simple(inputBuffer, MAX_BUF_SIZE, sampleRate, 0.02f, 0.6f);
			break;
		case 6: // "FLANGER"
			apply_flanger(inputBuffer, MAX_BUF_SIZE, sampleRate, 5.0f, 2.0f, 0.25f);
			break;
		case 7: // "TREMOLO"
			apply_tremolo(inputBuffer, MAX_BUF_SIZE, sampleRate, 500);
			break;
		default:
			printf("Error: Undefined effect ID %d\n", effect);
			break;
		}

		for (i = 0, j = 0; i < MAX_BUF_SIZE; i++)
		{
			temp2[j++] = inputBuffer[i] & 0xFF;
			temp2[j++] = (inputBuffer[i] >> 8) & 0xFF;
		}

		// Write processed data to output file
		fwrite(temp2, sizeof(uint8_t), 2 * MAX_BUF_SIZE, fpOut);

		*cnt += MAX_BUF_SIZE; // Update sample count
		printf("%ld amostras de dados processadas\n", *cnt);
	}
}