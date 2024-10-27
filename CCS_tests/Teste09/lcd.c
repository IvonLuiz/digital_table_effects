/*
 * lcd.c
 *
 *  Created on: 24 de out de 2024
 *      Author: Heverton Reis
 */
#include"ezdsp5502_i2c.h"
#include"ezdsp5502_gpio.h"
#include "csl_gpio.h"
#include"lcd.h"

#define OSD9616_I2C_ADDR 0x3C    // Endereco I2C do display OSD9616



Int16 osd9616_send( Uint16 comdat, Uint16 data )
{
    Uint16 cmd[2];
    cmd[0] = comdat & 0x00FF;     // Especifica se eh Comando ou Dado
    cmd[1] = data;                // Comando ou dado a ser enviado

    /* Escreve no OSD9616 */
    return EZDSP5502_I2C_write(OSD9616_I2C_ADDR, cmd, 2);
}


Int16 osd9616_multiSend( Uint16* data, Uint16 len )
{
    Uint16 x;
    Uint16 cmd[10];
    for(x=0;x<len;x++)            // Comando ou dado a ser enviado
    {
        cmd[x] = data[x];
    }

    /* Escreve no OSD9616 */
    return EZDSP5502_I2C_write( OSD9616_I2C_ADDR, cmd, len );
}


/*
 * Inicializa um display OLED (provavelmente o modelo OSD9616) via comandos I2C
 */
Int16 osd9616_init( )
{
    Uint16 cmd[10];    // Um array cmd eh definido para armazenar ate 10 comandos, mas a funcao nunca utiliza mais que 3 elementos de cmd ao mesmo tempo.

    /* Inicializacao da Alimentacao do LCD */
    EZDSP5502_GPIO_init(GPIO_GPIO_PIN1);                                 // Habilita o pino GPIO
    EZDSP5502_GPIO_setDirection(GPIO_GPIO_PIN1, GPIO_GPIO_PIN1_OUTPUT);  //  Define como saída
    EZDSP5502_GPIO_setOutput( GPIO_GPIO_PIN1, 1 );                       // Ativa 13V

    /*  Configuracao Inicial do Display */
    osd9616_send(0x00,0x00);                                             // Define endereco da coluna de baixo
    osd9616_send(0x00,0x10);                                             // Define endereco da coluna de cima
    osd9616_send(0x00,0x40);                                             // Define endereco da linha inicial

    /* Controle de Contraste */
    cmd[0] = 0x00 & 0x00FF;
    cmd[1] = 0x81;                                                       // Define o controle de contraste
    cmd[2] = 0x7f;                                                       // Nivel medio (127 em decimal).
    osd9616_multiSend(cmd, 3);

    /* Mapeamento e Configuracao de Exibicao */
    osd9616_send(0x00,0xa1);                                             /* Re-mapeia os segmentos de 95 a 0
                                                                          * O comando 0xA1 re-mapeia os segmentos da tela,
                                                                          * provavelmente para inverter a orientacao horizontal da exibicao.
                                                                          */
    osd9616_send(0x00,0xa6);                                             // 0xA6 define a tela em modo normal (nao invertido).

    /* Multiplexacao */
    cmd[0] = 0x00 & 0x00FF;
    cmd[1] = 0xa8;                                                       // O comando 0xA8 ajusta a multiplexacao da tela
    cmd[2] = 0x0f;                                                       // O valor 0x0F define um fator de multiplexacao especifico (16).
    osd9616_multiSend(cmd, 3);

    osd9616_send(0x00,0xd3);                                             // Define deslocamento de exibicao
    osd9616_send(0x00,0x00);                                             // Sem deslocamento
    osd9616_send(0x00,0xd5);                                             // Define o clock de exibicao
    osd9616_send(0x00,0xf0);                                             // Define a razao do clock

    /* Periodo de Pre-Carga */
    cmd[0] = 0x00 & 0x00FF;
    cmd[1] = 0xd9;
    cmd[2] = 0x22;
    osd9616_multiSend(cmd, 3);

    /* Configuracao de Pinos de Comunicacao */
    cmd[0] = 0x00 & 0x00FF;
    cmd[1] = 0xda;
    cmd[2] = 0x02;
    osd9616_multiSend(cmd, 3);

    /* Configuracao de Referencia de Tensao */
    osd9616_send(0x00,0xdb);                                             // Ajusta vcomh
    osd9616_send(0x00,0x49);                                             // 0.83*vref

    /* Habilitacao do Display */
    cmd[0] = 0x00 & 0x00FF;
    cmd[1] = 0x8d;
    cmd[2] = 0x14;
    osd9616_multiSend(cmd, 3);

    osd9616_send(0x00,0xaf);                                             // Liga o painel OLED

    return 0;
}

/* Responsavel por exibir uma letra ou simbolo no display OLED OSD9616 */
Int16 printLetter(Uint16 c4,Uint16 c3,Uint16 c2,Uint16 c1)
{
    osd9616_send(0x40, c4);    // Coluna 4
    osd9616_send(0x40, c3);    // Coluna 3
    osd9616_send(0x40, c2);    // Coluna 2
    osd9616_send(0x40, c1);    // Coluna 1
    osd9616_send(0x40, 0x00);

    return 0;
}
