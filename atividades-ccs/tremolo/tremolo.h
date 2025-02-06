/*
 * flanger.h
 *
 *  Created on: 27 de nov de 2024
 *      Author: Heverton Reis
 */

#ifndef TREMOLO_H_
#define TREMOLO_H_

#define MAX_BUF_SIZE 1024   // Define o tamanho maximo de um buffer,
                            // possivelmente usado para armazenar amostras de audio.
#define PI 3.1415926        // Define a constante PI com uma precisão suficiente para calculos de audio.
#define FS 8000             // Taxa de amostragem em Hz (8 kHz), tipica para audio de baixa qualidade.
#define FR 1                // Frequencia do oscilador de modulacao (1 Hz), comum para o efeito tremolo.
#define TDEPTH 1.0;         // Profundidade do efeito tremolo, que controla a intensidade da modulacao.

typedef struct {
    float mod;              // Valor atual da modulacao, baseado no oscilador de baixa frequencia (LFO).
    float A;                // Amplitude do oscilador, controlando a profundidade do tremolo.
    Int32 n;                // Contador para acompanhar a posicao amostral no oscilador (ou tempo discreto).
} tremolo;                  // Estrutura de dados para armazenar parametros e estado do efeito tremolo.

// Funcao de inicializacao do tremolo,
// configura os parametros iniciais (como profundidade do efeito).
void  tremoloInit(float depth, tremolo *t);

// Processa uma amostra de entrada (`xin`) aplicando o efeito tremolo
// com base nos parametros armazenados na estrutura `t`.
// Retorna a amostra processada.
Int16 tremoloProcess(Int16 xin, tremolo *t);

// Atualiza o valor do oscilador de modulacao (LFO) para gerar
// a variacao ciclica usada no efeito tremolo.
void  tremoloSweep(tremolo *t);

#endif /* TREMOLO_H_ */
