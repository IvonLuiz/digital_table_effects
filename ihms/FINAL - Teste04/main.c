#include "ezdsp5502.h"
#include <stdlib.h>
#include <stdio.h>
#include "pitch_shifter.h"

// Declaracao de variaveis globais
#define MAX_BUF_SIZE 256
Int8 temp[2 * MAX_BUF_SIZE]; // Buffer temporario para armazenar os dados de audio
Uint8 waveHeader[44];        // Cabecalho WAV

// Funcao para gerar o cabecalho WAV
void wHeader(Uint8 *w, float f1, float f2, Uint32 bytes);

void main()
{
    FILE *fpOut, *fpIn; // Ponteiros para os arquivos de entrada e saida
    Int16 i, j, c;      // Variaveis para o controle de lacos e entrada do usuario
    Int16 sample;       // Variavel para amostra de audio
    Uint32 cnt = 0;     // Contador de amostras processadas

    int sampleRate = 8000;   // Taxa de amostragem
    float pitchShift = 1.5f; // Fator de mudança de pitch

    // Solicita ao usuário que escolha o tipo de arquivo (PCM ou WAV)
    printf("Digite 1 para utilizar um arquivo PCM, digite 2 para utilizar arquivos WAV\n");
    scanf("%d", &c);

    // Abre os arquivos de entrada e saida de acordo com o formato escolhido
    if (c == 2)
    {
        fpIn = fopen("..\\data\\piano8kHz.wav", "rb");
        fpOut = fopen("..\\data\\audioOut.wav", "wb");
    }
    else
    {
        fpIn = fopen("..\\data\\piano8kHz.pcm", "rb");
        fpOut = fopen("..\\data\\audioOut.pcm", "wb");
    }

    // Verifica se os arquivos foram abertos com sucesso
    if (fpIn == NULL)
    {
        printf("Problema ao abrir o arquivo de audio da entrada\n");
        exit(0);
    }

    // Se o formato for WAV, lê e escreve o cabeçalho
    if (c == 2)
    {
        fread(waveHeader, sizeof(Int8), 44, fpIn);   // Lê o cabeçalho do arquivo WAV
        fwrite(waveHeader, sizeof(Int8), 44, fpOut); // Escreve o cabeçalho no arquivo de saída
    }

    printf("Exp --- experimento de mudança de pitch\n");

    // Processa os dados de audio em blocos de tamanho MAX_BUF_SIZE
    while ((fread(&temp, sizeof(Int8), 2 * MAX_BUF_SIZE, fpIn)) == 2 * MAX_BUF_SIZE)
    {
        for (j = 0, i = 0; i < MAX_BUF_SIZE; i++)
        {
            // Extrai uma amostra de audio de 16 bits
            sample = (temp[j] & 0xFF) | (temp[j + 1] << 8);

            // Aplica a mudança de pitch à amostra
            apply_pitch_shifter(&sample, 1, sampleRate, pitchShift);

            // Armazena a amostra processada de volta no buffer
            temp[j++] = sample & 0xFF;
            temp[j++] = (sample >> 8) & 0xFF;
        }

        // Escreve os dados processados no arquivo de saída
        fwrite(&temp, sizeof(short), 2 * MAX_BUF_SIZE, fpOut);
        cnt += MAX_BUF_SIZE;
        printf("%ld amostras de dados processadas\n", cnt);
    }

    // Se o arquivo de entrada for WAV, reescreve o cabecalho WAV com os dados corretos
    if (c == 2)
    {
        wHeader(waveHeader, 8000, 8000, cnt << 1);   // Ajusta os parametros no cabeçalho
        rewind(fpOut);                               // Reposiciona o ponteiro de saída para o início do arquivo
        fwrite(waveHeader, sizeof(Int8), 44, fpOut); // Regrava o cabeçalho
    }

    // Fecha os arquivos de entrada e saida
    fclose(fpOut);
    fclose(fpIn);

    printf("Exp --- completado\n");
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