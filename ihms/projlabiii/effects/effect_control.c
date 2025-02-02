#include "ezdsp5502.h"
#include <stdlib.h>
#include <stdio.h>
#include "pitch_shifter.h"
#include "reverb.h"
#include "flanger.h"
#include "tremolo.h"
#define MAX_BUF_SIZE 256

int sampleRate = 8000;
Int8 temp[2 * MAX_BUF_SIZE]; // Buffer temporario para armazenar os dados de audio
Int16 i, j, c;							 // Variaveis para o controle de lacos e entrada do usuario
Int16 sample;								 // Variavel para amostra de audio

void apply_effect(FILE *fpIn, FILE *fpOut, int effect, Uint32 *cnt)
{
	Uint32 i;
	printf("Aplicando o efeito: %d\n", effect);
	show_effect(effect, 2);
	i = 0;
	while ((fread(&temp, sizeof(Int8), 2 * MAX_BUF_SIZE, fpIn)) == 2 * MAX_BUF_SIZE)
	{
		for (j = 0, i = 0; i < MAX_BUF_SIZE; i++)
		{
			// Extrai uma amostra de audio de 16 bits
			sample = (temp[j] & 0xFF) | (temp[j + 1] << 8);

			// Apply the chosen effect
			switch (effect)
			{
			case 0: // "REV HALL1"
				apply_reverb_simple(&sample, 1, sampleRate, 0.05f, 0.8f);
				break;
			case 1: // "REV ROOM2"
				apply_reverb_simple(&sample, 1, sampleRate, 0.02f, 0.6f);
				break;
			case 2: // "REV STAGE B"
				apply_reverb_simple(&sample, 1, sampleRate, 0.03f, 0.7f);
				break;
			case 3: // "REV STAGE D"
				apply_reverb_simple(&sample, 1, sampleRate, 0.035f, 0.75f);
				break;
			case 4: // "REV STAGE F"
				apply_reverb_simple(&sample, 1, sampleRate, 0.04f, 0.85f);
				break;
			case 5: // "RET STAGE Gb"
				apply_reverb_simple(&sample, 1, sampleRate, 0.02f, 0.6f);
				break;
			case 6: // "FLANGER"
				apply_flanger(&sample, 1, sampleRate, 5.0f, 2.0f, 0.25f);
				break;
			case 7: // "TREMOLO"
				apply_tremolo(&sample, 1, sampleRate, 500);
				break;
			case 8: // "PITCH SHIFTER"
				apply_pitch_shifter(&sample, 1, sampleRate, 1.5f);
				break;
			default:
				printf("Error: Undefined effect ID %d\n", effect);
				break;
			};

			// Armazena a amostra processada de volta no buffer
			temp[j++] = sample & 0xFF;
			temp[j++] = (sample >> 8) & 0xFF;
		}

		// Escreve os dados processados no arquivo de saída
		fwrite(&temp, sizeof(short), 2 * MAX_BUF_SIZE, fpOut);
		cnt += MAX_BUF_SIZE;
		printf("%ld amostras de dados processadas\n", cnt);
		fclose(fpIn);
		fclose(fpOut);
	}
}