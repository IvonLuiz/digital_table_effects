

/**
 * main.c
 */
#include "ezdsp5502.h"
#include <stdio.h>
#include <math.h>
#include "tremolo.h"

// Inicializa a estrutura do tremolo
void tremoloInit(float depth, tremolo *t)
{
    t->A     = depth;                                   // Configura a profundidade do efeito tremolo.
    t->mod   = 0.0;                                     // Inicializa o valor do modulador (LFO) como zero.
    t->n     = 0;                                       // Reseta o contador de posicao no oscilador.
    printf( "tremoloInit() com depth [%f]\n",t->A);     // Exibe no console a profundidade configurada.
}

// Realiza o arredondamento de um numero em ponto flutuante (float)
// para o formato de 16 bits (Int16).
Int16 rounding16(float xd)
{
    Int16 y16;

    xd += 0.5;                                          // Adiciona 0.5 para realizar arredondamento
                                                        // para o valor inteiro mais proximo.

    if(xd > 32767.0) y16 = 32767;                       // Limita o valor ao maximo permitido por `Int16`.
    else if (xd < -32768.0) y16 = -32768;               // Limita o valor ao minimo permitido por `Int16`.
    else y16 = (Int16) xd;                              // Converte o valor para um inteiro de 16 bits.

    return y16;
}

// Processamento do tremolo
Int16 tremoloProcess(Int16 xinp, tremolo *t)
{
    float xin;
    float yout;
    float m;

    xin = 0.7 * xinp;                                   // Reduz a amplitude da entrada em 30% para evitar saturacao.
    m = (float)t->mod * t->A;                           // Calcula o valor atual do modulador (LFO) com a profundidade aplicada.
    yout = (m + 1) * xin;                               // Aplica a modulacao ao sinal de entrada.

    return rounding16 (yout);                           // Retorna o valor processado, arredondado para `Int16`.
}


#define W (2.0 * PI * FR / FS)                          // Define a frequencia angular (W) do LFO

void tremoloSweep(tremolo *t)
{
    t->n++;                                             // Incrementa o contador de posicao.
    t->mod = sin(W * t->n);                             // Calcula o proximo valor do LFO como uma senoide.
}


