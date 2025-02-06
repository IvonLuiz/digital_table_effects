/*
 * flangerTest.c
 *
 *  Created on: 27 de nov de 2024
 *      Author: Heverton Reis
 */
#include "ezdsp5502.h"
#include <stdlib.h>
#include <stdio.h>
#include "flanger.h"

Int16  d_buffer[MAX_BUF_SIZE];      // Buffer circular que armazena o historico de amostras
                                    // para aplicar o atraso. Ele eh inicializado com valores
                                    // das amostras iniciais do arquivo.
Int8   temp[2*MAX_BUF_SIZE];        // Buffer intermediario para leitura e escrita de blocos do arquivo.
Uint8  waveHeader[44];              // Cabecalho WAV global (usado para manipular arquivos WAV)

// Funcao para configurar o cabecalho WAV
void wHeader(Uint8 *w, float f1, float f2, Uint32 bytes);

// Parametros para o flanger
float depth = 1.0;                  // Profundidade do efeito
flanger param;                      // Estrutura contendo os parametros do flanger
flanger *t;                         // Ponteiro para a estrutura do flanger

void main()
{
    FILE *fpIn, *fpOut;             // Arquivos de entrada e saida
    Int16 i, j, yOut, c;            // Variaveis auxiliares
    Uint32 cnt;                     // Contador de amostras processadas

    // Solicita ao usuario que escolha o tipo de arquivo (PCM ou WAV)
    printf("Digite 1 para utilizar um arquivo PCM, digite 2 para utilizar arquivos WAV\n");
    scanf ("%d", &c);

    // Abre os arquivos de entrada e saída de acordo com o formato escolhido
    if (c == 2) {
        // Formato WAV
        fpIn = fopen("..\\data\\Soxphone8kHz.wav", "rb");
        fpOut = fopen("..\\data\\audioOut.wav", "wb");
    } else {
        // Formato PCM
        fpIn = fopen("..\\data\\Soxphone8kHz.pcm", "rb");
        fpOut = fopen("..\\data\\audioOut.pcm", "wb");
    }

    // Se o formato for WAV, le e escreve o cabeçalho
    if (c == 2) {
        fread(waveHeader, sizeof(Int8), 44, fpIn);      // Le o cabeçalho do arquivo WAV
        fwrite(waveHeader, sizeof(Int8), 44, fpOut);    // Escreve o cabecalho no arquivo de saida
    }

    printf("Exp --- experimento flanger\n");

    /* Inicializacao do flanger */
    t = &param;               // Associa o ponteiro `t` a estrutura de parametros
    flangerInit(depth, t);    // Inicializa os parametros do flanger com a profundidade

    // Le os dados iniciais para preencher o buffer de atraso
    fread(&temp, sizeof(Int8), 2 * MAX_BUF_SIZE, fpIn);
    j = 0;                    // Indice para o buffer temporario
    for (i = MAX_BUF_SIZE - 1; i > 0; i--) {
        // Preenche o buffer de atraso com amostras de 16 bits
        d_buffer[i] = temp[j++] & 0xFF;                 // Parte baixa do valor
        d_buffer[i] |= temp[j++] << 8;                  // Parte alta do valor
    }
    cnt = 0;                                            // Inicializa o contador de amostras processadas

    // Processamento do audio
    while ((fread(&temp, sizeof(Int8), 2 * MAX_BUF_SIZE, fpIn)) == 2 * MAX_BUF_SIZE) {
        for (j = 0, i = 0; i < MAX_BUF_SIZE; i++) {
            // Converte os dois bytes em uma amostra de 16 bits
            d_buffer[0] = (temp[j] & 0xFF) | (temp[j + 1] << 8);

            // Aplica o efeito flanger na amostra atual
            yOut = Flanger_process(d_buffer[0], t);

            // Atualiza o oscilador de baixa frequencia (LFO) para o proximo ciclo
            Flanger_sweep(t);

            // Incrementa o contador de amostras no flanger
            t->n++;

            // Armazena a saida processada no buffer temporário
            temp[j++] = yOut & 0xFF;                    // Parte baixa
            temp[j++] = (yOut >> 8) & 0xFF;             // Parte alta
        }

        // Escreve o bloco processado no arquivo de saida
        fwrite(&temp, sizeof(Int8), 2 * MAX_BUF_SIZE, fpOut);

        // Atualiza o contador e imprime o progresso
        cnt += MAX_BUF_SIZE;
        printf("%ld amostras de dados processadas\n", cnt);
    }

    // Se o arquivo de entrada for WAV, ajusta o cabecalho do arquivo de saida
    if (c == 2) {
        wHeader(waveHeader, 8000, 8000, cnt << 1);      // Ajusta o cabecalho WAV
        rewind(fpOut);                                  // Volta ao inicio do arquivo de saida
        fwrite(waveHeader, sizeof(Int8), 44, fpOut);    // Escreve o cabecalho atualizado
    }

        // Fecha os arquivos de entrada e saída
        fclose(fpOut);
        fclose(fpIn);
        printf("Exp --- completed\n");
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
