/*
 * loopLineInAIC3204.c
 *
 *  Created on: 22 de out de 2024
 *      Author: Heverton Reis
 */
#include "stdio.h"
#include "ezdsp5502.h"
#include "ezdsp5502_mcbsp.h"
#include "csl_mcbsp.h"

extern Int16 AIC3204_rset( Uint16 regnum, Uint16 regval);


Int16 aic3204_loop_linein()
{

    Int16 sec, msec;
    Int16 sample, dataLeft, dataRight;

    /* --------------------------------------------------------------------- *
    *  Configuracao do AIC3204                                               *
    * ---------------------------------------------------------------------- */
    AIC3204_rset(0, 0);      // Seleciona a pagina 0 do codec AIC3204
    AIC3204_rset(1, 1);      // Reinicia o codec para os estados iniciais
    AIC3204_rset(0, 1);      // Seleciona a pagina 1 do codec
    AIC3204_rset(1, 8);      /* Desabilita a geracao de AVDD (alimentacao analogica) a partir de DVDD (alimentacao digital).
                             *  Isso geralmente e feito para usar uma fonte de alimentacao analogica externa.
                             */
    AIC3204_rset(2, 1);      /* Habilita os blocos analogicos do codec e seleciona o modo de alimentacaoo via LDO (Low-Dropout Regulator),
                             * que regula a tensao de alimentacao de forma estavel.
                             */
    AIC3204_rset(0, 0);      // Retorna para a pagina 0 para continuar a configuracao em registradores dessa pagina


    /* Configuracao do PLL e dos Clocks e Ligacao do Codec  */
    AIC3204_rset(27, 0x0d);  /* Configura os clocks BCLK (Bit Clock) e WCLK (Word Clock) como saidas,
                             * tornando o codec AIC3204 o master no modo de comunicacao de audio.
                             */
    AIC3204_rset(28, 0x00);  // Define o deslocamento de dados para 0, ou seja, não ha atraso entre o inicio do frame de audio e o envio dos dados.
    AIC3204_rset(4, 3);      // Configura o PLL para usar o MCLK (Master Clock) como a fonte de clock e o CLKIN do codec sera derivado do clock do PLL.
    AIC3204_rset(6, 7);      // Configura o valor de J para 7 no PLL, o que faz parte da formula de multiplicacao de frequencia no PLL.
    AIC3204_rset(7, 0x06);   // Define o valor alto de D (HI_BYTE) para 1680, um valor utilizado no calculo da frequencia de saida do PLL.
    AIC3204_rset(8, 0x90);   // Define o valor baixo de D (LO_BYTE) para 1680. O valor completo de D e usado para ajustar o fator de multiplicacao do PLL.
    AIC3204_rset(30, 0x9C);  /* Configura o codec para ter 32 clocks por frame no modo master.
                             * O calculo do BCLK (Bit Clock) e realizado como sendo o clock do DAC (Digital-to-Analog Converter) dividido por N (8),
                             * resultando em um BCLK de 1.536 MHz.
                             */
    AIC3204_rset(5, 0x91);   // Habilita o PLL e configura os divisores P e R com valor 1, ajustando os multiplicadores de clock no PLL.
    AIC3204_rset(13, 0);     // Define o Hi_Byte (byte alto) do valor DOSR (Digital Oversampling Ratio) como 128 para configurar o oversampling do DAC.
    AIC3204_rset(14, 0x80);  // Define o Lo_Byte (byte baixo) do valor DOSR como 128 para finalizar a configuracao do oversampling do DAC.
    AIC3204_rset(20, 0x80);  // Configura o AOSR (Analog Oversampling Ratio) como 128, o que afeta a filtragem de decimacao para as etapas de oversampling no lado do ADC.
    AIC3204_rset(11, 0x82);  // Habilita o NDAC (divisor de clock do DAC) e define seu valor como 2.
    AIC3204_rset(12, 0x87);  // Habilita o MDAC (multiplicador de clock do DAC) e define seu valor como 7, o que influencia a frequencia final de operacao do DAC.
    AIC3204_rset(18, 0x87);  // Habilita o NADC (divisor de clock do ADC) e define seu valor como 7.
    AIC3204_rset(19, 0x82);  // Habilita o MADC (multiplicador de clock do ADC) e define seu valor como 2.


    /* Roteamento do DAC e Ligacao. Este bloco de codigo eh responsavel por configurar o roteamento do audio para os canais de saida do codec
     * DAC (Digital-to-Analog Converter) e ligar os amplificadores de saida.
     */
    AIC3204_rset(0, 1);      // Seleciona a pagina 1 do codec AIC3204
    AIC3204_rset(0x0c, 8);   // O canal esquerdo do DAC (LDAC) eh roteado para o amplificador de saida de audio no fone de ouvido esquerdo (HPL, Headphone Left).
    AIC3204_rset(0x0d, 8);   // O canal direito do DAC (RDAC) eh roteado para o amplificador de saida de audio no fone de ouvido direito (HPR, Headphone Right).
    AIC3204_rset(0, 0);      // Retorna para a pagina 0 para configurar outros parametros gerais do codec.
    AIC3204_rset(64, 2);     // Configura o volume do canal esquerdo para ser igual ao do canal direito.
    AIC3204_rset(65, 0);     // Define o ganho do canal esquerdo do DAC para 0dB e faz com que o canal direito siga o mesmo valor de volume.
    AIC3204_rset(63, 0xd4);  // Liga os caminhos de dados para os canais esquerdo e direito e configura os canais de audio.
    AIC3204_rset(0, 1);      // Seleciona novamente a pagina 1 para configurar mais parametros especificos de saida de audio.
    AIC3204_rset(9, 0x30);   // Liga os amplificadores de saida de fone de ouvido para os canais esquerdo (HPL) e direito (HPR).
    AIC3204_rset(0x10, 0x00);// Desativa o mute (silenciamento) do amplificador esquerdo (HPL) e configura o ganho em 0dB.
    AIC3204_rset(0x11, 0x00);// Desativa o mute do amplificador direito (HPR) e configura o ganho em 0dB.
    AIC3204_rset(0, 0);      // Retorna a pagina 0 para continuar com outras configuracoes gerais.
    EZDSP5502_waitusec(1000000); // Aguarda 1 segundo para garantir que todas as configurações e as rotas de audio estejam devidamente aplicadas antes de prosseguir.


    /* Roteamento e Ligacao do ADC. Este bloco de codigo cuida de configurar o roteamento de sinais de audio para o ADC (Conversor Analogico-Digital)
     * do codec AIC3204 e ligar o conversor para habilitar a captura de audio.
     */
    AIC3204_rset(0, 1);      // Seleciona a pagina 1 do codec AIC3204
    AIC3204_rset(0x34, 0x30);/* Configura a entrada estereo 1 (jack estereo) para o ADC esquerdo (LADC_P), conectando o pino de entrada IN2_L
                              * atraves de uma resistência de 40 kOhms.
                              */
    AIC3204_rset(0x37, 0x30);// Conecta a entrada direita (IN2_R) ao ADC direito (RADC_P) tambem atraves de uma resistencia de 40 kohms.
    AIC3204_rset(0x36, 3);   // Configura o modo comum (CM_1) para o pino negativo do ADC esquerdo (LADC_M), passando atraves de uma resistencia de 40 kohms.
    AIC3204_rset(0x39, 0xc0);// Configura o modo comum (CM_1) para o pino negativo do ADC direito (RADC_M), tambem com uma resistencia de 40 kohms.
    AIC3204_rset(0x3b, 0);   // Desativa o mute (silenciamento) do pre-amplificador de ganho de microfone no canal esquerdo (MIC_PGA_L).
    AIC3204_rset(0x3c, 0);   // Desativa o mute do pre-amplificador de ganho de microfone no canal direito (MIC_PGA_R).
    AIC3204_rset(0, 0);      // Retorna para a pagina 0, onde outros parametros gerais do codec podem ser configurados.
    AIC3204_rset(0x51, 0xc0);// Liga os conversores analogico-digital (ADCs) esquerdo e direito, habilitando-os para conversao de sinal.
    AIC3204_rset(0x52, 0);   // Desativa o mute dos ADCs esquerdo e direito, permitindo que eles processem os sinais de audio recebidos.
    AIC3204_rset(0, 0);      // Seleciona novamente a pagina 0, concluindo as configuracoes.
    EZDSP5502_waitusec(200); // Aguarda 200 microsegundos para permitir que o codec estabilize as configuracoes antes de prosseguir com a captura de audio.


    /* Esta linha inicializa a interface McBSP (Multichannel Buffered Serial Port) do processador */
    EZDSP5502_MCBSP_init();

    /* As variaveis dataLeft e dataRight, que armazenam os dados de audio dos canais esquerdo e direito, sao inicializadas com o valor 0.
     * O codigo vai executar uma operaçao de loopback (reproduçao do sinal recebido de volta ao transmissor) por 10 segundos.
    */
    dataLeft = 0;
    dataRight = 0;

    for ( sec = 0 ; sec < 30 ; sec++ )                  // Controla o tempo total em segundos (10 segundos)
    {
        for ( msec = 0 ; msec < 1000 ; msec++ )         // Loop em milissegundos (1000 ms por segundo).
        {
            for ( sample = 0 ; sample < 48 ; sample++ ) // Itera sobre 48 amostras de audio por milissegundo.
            {
                    EZDSP5502_MCBSP_read(&dataLeft);      // Le dados do canal direito (RX). O valor lido eh armazenado em dataLeft, em um arranjo loopback.
                    EZDSP5502_MCBSP_write(dataLeft);      // Transmite os dados lidos para o canal esquerdo (TX).

                    EZDSP5502_MCBSP_read(&dataRight);      // Aqui, os dados do canal esquerdo sao lidos e armazenados em dataRight.
                    EZDSP5502_MCBSP_write( dataRight);     /* Logo apos, esses dados sao transmitidos de volta ao canal direito.
                                                            * Este ciclo completa o loopback: os dados do canal esquerdo e direito sao lidos
                                                            * e transmitidos de volta para os canais opostos.
                                                            */
                }
            }
        }

        EZDSP5502_MCBSP_close(); // Apos a execução do loop por 10 segundos, a funcao desativa a interface McBSP, finalizando a comunicacao serial.
        AIC3204_rset(1, 1);      // O codec AIC3204 eh reinicializado, retornando-o ao estado padrao.

        return 0;
}
