

/**
 * main.c
 */
#include "ezdsp5502.h"
#include <math.h>
#include "flanger.h"

// Inicializa a estrutura do flanger
void flangerInit(float depth, flanger *t)
{
    t->delay = 100;                         // Atraso medio (em amostras)
    t->A_maxSwing = 0.5;                    // Amplitude maxima de oscilacao do atraso
    t->G_depth = depth;                     // Intensidade do efeito flanger.
    t->n = 0;                               // Contador usado para o LFO
}

// Processamento do flanger
Int16 Flanger_process(Int16 xinp, flanger *t)
{
    float y0, y1;
    float fracY, yout;
    Int16 iy0, iy1;
    Int16 i;

    iy0 = (Int16)(t->var_delay);            // Indice da amostra mais recente no buffer.
    iy1 = (Int16)(t->var_delay + 1.0);      // Indice da amostra anterior (para interpolacao).

    // Realiza interpolacao linear entre dois valores no buffer (y0 e y1).
    y0 = (float)d_buffer[iy0];
    y1 = (float)d_buffer[iy1];
    // Calcula a fracao fracY para determinar o peso de cada amostra.
    fracY = (float)(1.0 - (t->var_delay - (float)iy0));

    yout = fracY * y0 + (1.0 - fracY) * y1;

    // Combina o sinal de entrada (xinp) com a saida interpolada,
    // aplicando o fator de profundidade do efeito.
    // Normaliza o resultado para evitar saturacao.
    yout = xinp + t->G_depth * yout ;
    yout = yout / (1.0 + t->G_depth);

    // Atualiza o buffer de atraso deslocando os valores existentes
    // e preparando espaco para uma nova amostra.
    for (i = MAX_BUF_SIZE - 1; i > 0; i--)
    {
        d_buffer[i] = d_buffer[i - 1];
    }

    return (Int16)yout;                         // Retorna o valor processado como saida.
}

#define W (2.0 * 3.1415926 * LFO_F / 8000.0)    // Define a frequencia angular (W) do LFO,
                                                // assumindo uma taxa de amostragem de 8 kHz

void Flanger_sweep(flanger *t)
{
    // Atualiza o atraso variavel (var_delay) com base em uma funcao senoidal.
    // O LFO modula o atraso em torno de um valor medio (delay)
    // com amplitude controlada por A_maxSwing.
    // Incrementa o contador n, determinando a fase do LFO.
    t->var_delay = t->delay * (1.0 + t->A_maxSwing * sin(W * t->n));
}

