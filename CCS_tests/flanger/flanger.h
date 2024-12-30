/*
 * flanger.h
 *
 *  Created on: 27 de nov de 2024
 *      Author: Heverton Reis
 */

#ifndef FLANGER_H_
#define FLANGER_H_

#define MAX_BUF_SIZE 1024       // Tamanho maximo do buffer de atraso
#define LFO_F 0.5               // Frequencia do oscilador de baixa frequencia (LFO) em Hz

extern Int16 d_buffer[MAX_BUF_SIZE];

typedef struct {
    float A_maxSwing;  // Amplitude maxima de oscilacao do atraso
    float G_depth;     // Controle da intensidade do efeito flanger
    float var_delay;   // Representa o atraso atual, calculado dinamicamente pelo LFO (em amostras)
    Int16 delay;       // Atraso medio (em amostras)
    Int32 n;           // Contador usado para o LFO
} flanger;

void  flangerInit(float depth, flanger *t);     // Inicializa os parametros da estrutura flanger com
                                                // valores padrao e configuracoes iniciais.
Int16 Flanger_process(Int16 xinp, flanger *t);  // Aplica o efeito flanger ao sinal de entrada (xinp) e retorna o valor processado.
void  Flanger_sweep(flanger *t);                // Calcula a variacao do atraso utilizando um LFO senoidal.


#endif /* FLANGER_H_ */
