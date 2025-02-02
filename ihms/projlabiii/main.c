#include "stdio.h"
#include <stdlib.h>
#include "ezdsp5502.h"
#include "ezdsp5502_i2cgpio.h"
#include "csl_gpio.h"
#include "ihm/display.h"
#include "ihm/switch.h"
#include "file/wav.h"
#include "effects/effect_control.h"

#define NUM_EFFECTS 8
Uint32 cnt = 0; // Global sample counter
int currentEffect = 0;

void delay_ms(Uint32 milliseconds)
{
	volatile Uint32 scs;
	for (scs = 0; scs < milliseconds * 1000; scs++)
		;
}

void execEffect(FILE *fp, int effect)
{
	printf("Executing effect: %d\n", effect);
	show_effect(effect, 3);
	delay_ms(4000);
}

void main(void)
{
	EZDSP5502_init();
	// printf("Reading WAV...\n");
	// const char *filename = "..\\data\\piano8kHz.wav";
	// const char *filename_out = "..\\data\\audio_out.wav";

	// FILE *fpIn = open_wav_file(filename);
	show_effect(currentEffect, 0);

	while (1)
	{
		int buttonStatus = check_switch(); // Read switch input

		if (buttonStatus == 0) // SW0 pressed
		{
			show_effect(currentEffect, 1);
			delay_ms(5000);
			// FILE *fpOut = create_file(filename_out);
			// // write_header(fpIn, fpOut);
			// Uint8 waveHeader[44]; // Local header
			// fread(waveHeader, sizeof(Uint8), 44, fpIn);
			// fwrite(waveHeader, sizeof(Uint8), 44, fpOut);

			// // Reset sample count and rewind file
			// cnt = 0;
			// rewind(fpIn);
			// fseek(fpIn, 44, SEEK_SET); // Skip WAV header after rewind

			// // Apply effect and update header
			// apply_effect(fpIn, fpOut, currentEffect, &cnt);
			// // ajust wave header
			// // ajust_header(fpOut, cnt);
			// wHeader(waveHeader, 8000, 8000, cnt << 1);	 // Ajusta os parametros no cabeçalho
			// rewind(fpOut);								 // Reposiciona o ponteiro de saída para o início do arquivo
			// fwrite(waveHeader, sizeof(Int8), 44, fpOut); // Regrava o cabeçalho

			// fclose(fpOut);
			// printf("Effect %d applied and saved.\n", currentEffect);
			show_effect(currentEffect, 2);

			// Wait for user response
			while (1)
			{
				buttonStatus = check_switch();

				if (buttonStatus == 0)
				{
					show_effect(currentEffect, 3);
					delay_ms(4000);
				}
				else if (buttonStatus == 1)
				{
					printf("Returning to effect selection...\n");
					// rewind(fpIn);
					// fseek(fpIn, 44, SEEK_SET); // Reset for next effect
					show_effect(currentEffect, 0);
					break;
				}
			}
		}

		if (buttonStatus == 1) // SW1 pressed
		{
			currentEffect = (currentEffect + 1) % NUM_EFFECTS; // Cycle effects
			show_effect(currentEffect, 0);
		}
	}

	fclose(fpIn);
}

// Cria o cabecalho do arquivo WAV
void wHeader(Uint8 *w, float f1, float f2, Uint32 bytes)
{
	Int32 t;

	// Tamanho do arquivo de audio
	t = bytes;
	w[40] = (t >> 0) & 0xff;
	w[41] = (t >> 8) & 0xff;
	w[42] = (t >> 16) & 0xff;
	w[43] = (t >> 24) & 0xff;

	// Tamanho total do arquivo (dados + cabeçalho)
	t += 36;
	w[4] = (t >> 0) & 0xff;
	w[5] = (t >> 8) & 0xff;
	w[6] = (t >> 16) & 0xff;
	w[7] = (t >> 24) & 0xff;

	// Ajuste da taxa de amostragem e taxa de bytes
	t = w[24] | (w[25] << 8) | ((Int32)w[26] << 16) | ((Int32)w[27] << 24);
	t = (Int32)((float)t * f2 / f1);
	w[24] = (t >> 0) & 0xff;
	w[25] = (t >> 8) & 0xff;
	w[26] = (t >> 16) & 0xff;
	w[27] = (t >> 24) & 0xff;

	t = w[28] | (w[29] << 8) | ((Int32)w[30] << 16) | ((Int32)w[31] << 24);
	t = (Int32)((float)t * f2 / f1);
	w[28] = (t >> 0) & 0xff;
	w[29] = (t >> 8) & 0xff;
	w[30] = (t >> 16) & 0xff;
	w[31] = (t >> 24) & 0xff;

	return;
}