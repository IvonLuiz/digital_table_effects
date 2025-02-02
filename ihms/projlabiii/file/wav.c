#include "stdio.h"
#include <stdlib.h>
#include "ezdsp5502.h"
Uint8 waveHeader[44]; // Cabecalho WAV global (usado para manipular arquivos WAV)

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
    printf("File read!");
    return fp;
}

FILE *create_file(const char *filename)
{
    // Open the file in "write" mode
    FILE *fp = fopen(filename, "wb");

    if (fp == NULL)
    {
        // Handle errors if the file cannot be created or opened
        printf("Error: Failed to create the file '%s'.\n", filename);
        return NULL; // Return NULL on error instead of exiting
    }

    // Notify the user that the file was successfully created
    printf("File '%s' created successfully.\n", filename);

    return fp; // Return the file pointer
}

void write_header(FILE *fpIn, FILE *fpOut)
{
    fread(waveHeader, sizeof(Int8), 44, fpIn);   // Lê o cabeçalho do arquivo WAV
    fwrite(waveHeader, sizeof(Int8), 44, fpOut); // Escreve o cabeçalho no arquivo de saída
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

    // Tamanho total do arquivo (dados + cabecalho)
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

void ajust_header(FILE *fpOut, Uint32 cnt)
{
    wHeader(waveHeader, 8000, 8000, cnt << 1);    // Ajusta o cabeçalho WAV
    rewind(fpOut);                                // Volta ao início do arquivo de saída
    fwrite(waveHeader, sizeof(Uint8), 44, fpOut); // Escreve o cabeçalho atualizado
}
