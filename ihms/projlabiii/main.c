#include "stdio.h"
#include <stdlib.h>
#include "ihm/display.h"
#include "ihm/switch.h"
#include "file/wav.h"
#include "effects/effect_control.h"
#define SIZE 1024 // DMA block size for each transfer
Uint8 ch[SIZE];		// Buffer for audio data

int currentEffect = 0;
typedef unsigned int uint32_t;
Uint32 cnt = 0;

void execEffect(FILE *fp, int effect)
{
	Uint32 i;
	printf("Executando o efeito: %d\n", effect);
	show_effect(effect, 4);
	i = 0;
	delay_ms(2000);
	//    while (fread(ch, sizeof(Uint8), SIZE, fp) == SIZE)
	//    {
	//        i += SIZE;
	//        // Aplicar efeito
	//        // DMA
	//        printf("%ld bytes processed\n", i); // Show progress
	//    }
}

void main(void)
{
	printf("Reading WAV...\n");
	const char *filename = "..\\data\\audio.wav";
	FILE *fpIn = open_wav_file(filename);
	EZDSP5502_init();
	switchEffect(currentEffect, fpIn);

	printf("\nAudio playback processing completed.\n");
}
