/*
 * lcd.c
 *
 *  Created on: 24 de out de 2024
 *      Author: Heverton Reis
 */

#include "ezdsp5502.h"
#include "lcd.h"
#include "display.h"

int i = 0;

void printChar(char c)
{
    int index;
    // Converter caractere para indice da tabela (A-Z, a-z, 0-9)
    if (c >= 'A' && c <= 'Z')
    {
        index = c - 'A'; // Indice para letras maiusculas
    }
    else if (c >= 'a' && c <= 'z')
    {
        index = c - 'a' + 26; // Indice para letras minusculas
    }
    else if (c >= '0' && c <= '9')
    {
        index = c - '0' + 52; // Indice para numeros
    }
    else if (c == ' ')
    {
        index = 62; // Indice para o espaco
    }
    else if (c == '!')
    {
        index = 63; // Indice para o caractere '!'
    }
    else if (c == '?')
    {
        index = 64; // Indice para o caractere '?'
    }
    else
    {
        return; // Caractere nao suportado
    }

    // Usar a funcao printLetter com os valores da tabela de fontes
    printLetter(fonte[index][0], fonte[index][1], fonte[index][2], fonte[index][3]);
}

void printString(const char *str)
{
    // Encontrar o fim da string
    const char *end = str;
    while (*end)
    {
        end++;
    }
    // Agora 'end' aponta para o caractere nulo '\0'.
    // Percorrer a string ao contrario, do fim para o comeco
    while (end != str)
    {
        end--;           // Voltar um caractere antes do '\0'
        printChar(*end); // Imprimir o caractere
    }
}

void writeOnPage(Uint16 page)
{
    osd9616_send(0x00, 0x00);        // Define endereco da coluna de baixo
    osd9616_send(0x00, 0x10);        // Define endereco da coluna de cima
    osd9616_send(0x00, 0xb0 + page); // Define a linha (pagina)
}

void clearLine(Uint16 line)
{

    writeOnPage(line);

    for (i = 0; i < 128; i++)
    {
        osd9616_send(0x40, 0x00); // Envia 0x00 para desligar todos os pixels na linha
    }
}

void printSpace(Uint16 spaces)
{

    for (i = 0; i < spaces; i++)
    {
        osd9616_send(0x40, 0x00);
    }
}

Int16 show_effect(int effect)
{
    osd9616_init();           // Inicializa o Display
    osd9616_send(0x00, 0x2e); // Desativa o Scrolling

    clearLine(0);   // Limpa a linha 0
    writeOnPage(0); // Define endereco da linha inicial

    // Arrays for effect names and descriptions
    const char *effect_names[] = {
        "Efeito 1 ",
        "Efeito 2 ",
        "Efeito 3 ",
        "Efeito 4 ",
        "Efeito 5 ",
        "Efeito 6 ",
        "Efeito 7 ",
        "Efeito 8 "};

    const char *effect_descriptions[] = {
        "REV HALL1 ",
        "REV ROOM2 ",
        "REV STAGE B ",
        "REV STAGE D ",
        "REV STAGE F ",
        "RET STAGE Gb ",
        "FLANGER ",
        "TREMOLO "};

    // Check if the effect is valid (between 0 and 7)
    if (effect >= 0 && effect <= 7)
    {
        printString(effect_names[effect]); // Display effect name
    }
    else
    {
        printString("Invalid effect!"); // Display error message
    }

    clearLine(1); // Limpa a linha 1
    writeOnPage(1);

    if (effect >= 0 && effect <= 7)
    {
        printString(effect_descriptions[effect]); // Display effect description
    }
    else
    {
        printString("Unknown effect.");
    }

    clearLine(2); // Limpa a linha 2
    return 0;
}
