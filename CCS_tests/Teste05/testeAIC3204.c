/*
 * testeAIC3204.c
 *
 *  Created on: 22 de out de 2024
 *      Author: Heverton Reis
 */
#include "ezdsp5502.h"
#include "ezdsp5502_i2c.h"
#include "ezdsp5502_i2cgpio.h"
#include "stdio.h"

// Endereço I2C do codec AIC3204
#define AIC3204_I2C_ADDR 0x18

// Faz um loopback de audio do canal de entrada estereo para a saida de fones de ouvido.
extern Int16 aic3204_loop_linein();

/* Le o valor de um registro do codec. Envia o numero do registro via I2C e, em seguida, le o valor correspondente.
 * O valor lido e armazenado em *regval, e a funcao retorna um codigo de erro se ocorrer algum problema.
 * */
Int16 AIC3204_rget(Uint16 regnum, Uint16* regval)
{
    Int16  retcode = 0;
    Uint16 cmd[2];

    cmd[0] = regnum & 0x007F;       // Endereco de 7 bits do registro
    cmd[1] = 0;

    retcode |= EZDSP5502_I2C_write(AIC3204_I2C_ADDR, cmd, 1);  // Envia o endereco do registro
    retcode |= EZDSP5502_I2C_read( AIC3204_I2C_ADDR, cmd, 1 );   // Le o valor do registro
    *regval = cmd[0];  // Armazena o valor lido
    EZDSP5502_waitusec( 50 );  // Espera por 50 microsegundos

    return retcode;
}

/* Escreve um valor em um registro do codec AIC3204. Envia o numero do registro e o valor via I2C.
 * Novamente, a funcao retorna um codigo de erro em caso de falha.
 * */
Int16 AIC3204_rset(Uint16 regnum, Uint16 regval)
{
    Uint16 cmd[2];
    cmd[0] = regnum & 0x007F;  // Endereco de 7 bits do registro
    cmd[1] = regval;           // Valor a ser escrito no registro

    EZDSP5502_waitusec( 100 ); // Espera por 100 microsegundos
    return EZDSP5502_I2C_write(AIC3204_I2C_ADDR, cmd, 2);  // Escreve o valor no registro via I2C
}


Int16 aic3204_test()
{
    /* Configura o McBSP1 para modo apropriado */
    EZDSP5502_I2CGPIO_configLine(BSP_SEL1, OUT);
    EZDSP5502_I2CGPIO_writeLine(BSP_SEL1, LOW);

    EZDSP5502_I2CGPIO_configLine(BSP_SEL1_ENn, OUT);
    EZDSP5502_I2CGPIO_writeLine(BSP_SEL1_ENn, LOW);

    /* Testa a saida de fone de ouvido com tom de 1 KHz */
    //printf( " -> Tom de 1 KHz na saida.\n" );
    //if ( aic3204_tone_headphone( ) )  // Teste do tom de saida
    //    return 1;  // Retorna 1 se o teste falhar

    //EZDSP5502_wait( 100 );  // Espera um pouco

    /* Testa o loopback de audio */
    printf( "<-> Loopback de Audio da ENTRADA --> para a SAIDA\n" );
    if (aic3204_loop_linein()){// Teste de loopback
        return 3;  // Retorna 3 se o loopback falhar
    }

    return 0;  // Retorna 0 se todos os testes passarem
}
