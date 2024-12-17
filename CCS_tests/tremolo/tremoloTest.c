/*
 * flangerTest.c
 *
 *  Created on: 27 de nov de 2024
 *      Author: Heverton Reis
 */
#include "ezdsp5502.h"
#include <stdlib.h>
#include <stdio.h>
#include "tremolo.h"

// Declaracao de variaveis globais
tremolo *t;
tremolo param;
Int8   temp[2*MAX_BUF_SIZE];        // Buffer temporario para armazenar os dados de audio
Uint8  waveHeader[44];              // Cabeçalho WAV

// Funcao para gerar o cabecalho WAV
void wHeader(Uint8 *w, float f1, float f2, Uint32 bytes);

void main()
{
    FILE   *fpOut,*fpIn;            // Ponteiros para os arquivos de entrada e saida
    Int16  i, j, c;                 // Variaveis para o controle de lacos e entrada do usuario
    Int16  sample, yOut;            // Variaveis para amostra de audio e saida apos processamento
    Uint32 cnt = 0;                 // Contador de amostras processadas
    float  depth;                   // Profundidade do efeito tremolo

    // Solicita ao usuario que escolha o tipo de arquivo (PCM ou WAV)
    printf("Digite 1 para utilizar um arquivo PCM, digite 2 para utilizar arquivos WAV\n");
    scanf ("%d", &c);

    // Abre os arquivos de entrada e saída de acordo com o formato escolhido
    if (c == 2) {
        fpIn = fopen("..\\data\\piano8kHz.wav", "rb");
        fpOut = fopen("..\\data\\audioOut.wav", "wb");
    } else {
        fpIn = fopen("..\\data\\piano8kHz.pcm", "rb");
        fpOut = fopen("..\\data\\audioOut.pcm", "wb");
    }

    // Verifica se os arquivos foram abertos com sucesso
    if (fpIn == NULL) {
        printf("Problema ao abrir o arquivo de audio da entrada\n");
        exit(0);
    }

    // Se o formato for WAV, le e escreve o cabeçalho
    if (c == 2) {
        fread(waveHeader, sizeof(Int8), 44, fpIn);      // Le o cabeçalho do arquivo WAV
        fwrite(waveHeader, sizeof(Int8), 44, fpOut);    // Escreve o cabecalho no arquivo de saida
    }

    printf("Exp --- experimento tremolo\n");

    // Inicializa o efeito tremolo com a profundidade especificada
    cnt = 0;
    t = &param;                                         // Ponteiro para a estrutura tremolo
    depth = TDEPTH;                                     // Define a profundidade do efeito tremolo
    tremoloInit(depth, t);                              // Inicializa o efeito com a profundidade

    // Processa os dados de audio em blocos de tamanho MAX_BUF_SIZE
    while ((fread(&temp, sizeof(Int8), 2*MAX_BUF_SIZE, fpIn)) == 2*MAX_BUF_SIZE) {
        for (j = 0, i = 0; i < MAX_BUF_SIZE; i++) {
            // Extrai uma amostra de audio de 16 bits
            sample = (temp[j] & 0xFF) | (temp[j+1] << 8);

            // Aplica o efeito tremolo a amostra
            yOut = tremoloProcess(sample, t);

            // Atualiza o LFO (Low-Frequency Oscillator)
            tremoloSweep(t);

            // Armazena a amostra processada de volta no buffer
            temp[j++] = yOut & 0xFF;
            temp[j++] = (yOut >> 8) & 0xFF;
        }

        // Escreve os dados processados no arquivo de saida
        fwrite(&temp, sizeof(short), 2*MAX_BUF_SIZE, fpOut);
        cnt += MAX_BUF_SIZE;
        printf("%ld amostras de dados processadas\n", cnt);
    }

    // Se o arquivo de entrada for WAV, reescreve o cabecalho WAV com os dados corretos
    if (c == 2) {
        wHeader(waveHeader, 8000, 8000, cnt << 1);      // Ajusta os parametros no cabeçalho
        rewind(fpOut);                                  // Reposiciona o ponteiro de saida para o inicio do arquivo
        fwrite(waveHeader, sizeof(Int8), 44, fpOut);    // Regrava o cabecalho
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
