#include "stdio.h"
#include <stdlib.h>
#include "ihm/display.h"
#include "ihm/switch.h"

#define SIZE 1024 // DMA block size for each transfer
Uint8 ch[SIZE];   // Buffer for audio data

int currentEffect = 0;
typedef unsigned int uint32_t;
void delay_ms(uint32_t milliseconds)
{
    // Approximate loop iterations for 1 ms delay (adjust based on your system's clock speed)
    volatile uint32_t i;
    for (i = 0; i < milliseconds * 1000; i++); // Busy-wait loop
}

void applyEffect(FILE *fp, int effect)
{
    Uint32 i;
    printf("Aplicando o efeito: %d\n", effect);
    show_effect(effect, 2);
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

FILE *open_wav_file(const char *filename)
{
    // Tenta abrir o arquivo em modo binario
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        printf("Failed to open input WAV file: %s\n", filename);
        exit(0);
    }

    // Move o ponteiro de leitura para ignorar o cabecalho (44 bytes)
    if (fseek(fp, 44, SEEK_SET) != 0)
    {
        printf("Failed to seek past the header in WAV file: %s\n", filename);
        fclose(fp);
        exit(0);
    }

    return fp;
}

void main(void)
{
    printf("Reading WAV...\n");
    const char *filename = "..\\data\\audio.wav";
    FILE *fp1 = open_wav_file(filename);

//    applyEffect(fp1, 1);

    printf("File read!");
    fclose(fp1);

    EZDSP5502_init();
//    applyEffect(currentEffect);
    switchEffect(currentEffect);

    fclose(fp1);
    printf("\nAudio playback processing completed.\n");
}
