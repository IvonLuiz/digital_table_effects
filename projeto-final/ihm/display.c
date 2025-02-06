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

Int16 show_effect(int effect, int mode)
{

    osd9616_init();           // Inicializa o Display
    osd9616_send(0x00, 0x2e); // Desativa o Scrolling

    clearLine(0);   // Limpa a linha 0
    writeOnPage(0); // Define endereco da linha inicial

    // Arrays for effect names and descriptions
    const char *effect_names_show[] = {
        "EFEITO 1 ",
        "EFEITO 2 ",
        "EFEITO 3 ",
        "EFEITO 4 ",
        "EFEITO 5 ",
        "EFEITO 6 ",
        "EFEITO 7 ",
        "EFEITO 8 "};

    const char *effect_names_apply[] = {
        "APLICANDO EFEITO 1 ",
        "APLICANDO EFEITO 2 ",
        "APLICANDO EFEITO 3 ",
        "APLICANDO EFEITO 4 ",
        "APLICANDO EFEITO 5 ",
        "APLICANDO EFEITO 6 ",
        "APLICANDO EFEITO 7 ",
        "APLICANDO EFEITO 8 "};

    const char *effect_names_ready[] = {
        "EFEITO PRONTO 1 ",
        "EFEITO PRONTO 2 ",
        "EFEITO PRONTO 3 ",
        "EFEITO PRONTO 4 ",
        "EFEITO PRONTO 5 ",
        "EFEITO PRONTO 6 ",
        "EFEITO PRONTO 7 ",
        "EFEITO PRONTO 8 "};

    const char *effect_names_executing[] = {
        "EXECUTANDO EFEITO 1 ",
        "EXECUTANDO EFEITO 2 ",
        "EXECUTANDO EFEITO 3 ",
        "EXECUTANDO EFEITO 4 ",
        "EXECUTANDO EFEITO 5 ",
        "EXECUTANDO EFEITO 6 ",
        "EXECUTANDO EFEITO 7 ",
        "EXECUTANDO EFEITO 8 "};

    const char *effect_descriptions[] = {
        "REV HALL1 ",
        "REV ROOM2 ",
        "REV STAGE B ",
        "REV STAGE D ",
        "REV STAGE F ",
        "RET STAGE Gb ",
        "FLANGER ",
        "TREMOLO "};

    // Select the appropriate effect names based on the mode
    const char **effect_names;
    switch (mode)
    {
    case 1:
        effect_names = effect_names_show; // Selection mode
        break;
    case 2:
        effect_names = effect_names_apply; // Applying mode
        break;
    case 3:
        effect_names = effect_names_ready; // Ready mode
        break;
    case 4:
        effect_names = effect_names_executing; // Executing mode
        break;
    default:
        effect_names = effect_names_show; // Default to selection mode
        break;
    }
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
