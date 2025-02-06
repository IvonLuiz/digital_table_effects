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

    if (c >= 'A' && c <= 'Z')
    {
        index = c - 'A';
    }
    else if (c >= 'a' && c <= 'z')
    {
        index = c - 'a' + 26;
    }
    else if (c >= '0' && c <= '9')
    {
        index = c - '0' + 52;
    }
    else if (c == ' ')
    {
        index = 62;
    }
    else if (c == '!')
    {
        index = 63;
    }
    else if (c == '?')
    {
        index = 64;
    }
    else
    {
        return;
    }

    printLetter(fonte[index][0], fonte[index][1], fonte[index][2], fonte[index][3]);
}

void printString(const char *str)
{

    const char *end = str;
    while (*end)
    {
        end++;
    }

    while (end != str)
    {
        end--;
        printChar(*end);
    }
}

void writeOnPage(Uint16 page)
{
    osd9616_send(0x00, 0x00);
    osd9616_send(0x00, 0x10);
    osd9616_send(0x00, 0xb0 + page);
}

void clearLine(Uint16 line)
{

    writeOnPage(line);

    for (i = 0; i < 128; i++)
    {
        osd9616_send(0x40, 0x00);
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

    osd9616_init();
    osd9616_send(0x00, 0x2e);

    clearLine(0);
    writeOnPage(0);

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

    const char **effect_names;
    switch (mode)
    {
    case 1:
        effect_names = effect_names_show;
        break;
    case 2:
        effect_names = effect_names_apply;
        break;
    case 3:
        effect_names = effect_names_ready;
        break;
    case 4:
        effect_names = effect_names_executing;
        break;
    default:
        effect_names = effect_names_show;
        break;
    }

    if (effect >= 0 && effect <= 7)
    {
        printString(effect_names[effect]);
    }
    else
    {
        printString("Invalid effect!");
    }

    clearLine(1);
    writeOnPage(1);

    if (effect >= 0 && effect <= 7)
    {
        printString(effect_descriptions[effect]);
    }
    else
    {
        printString("Unknown effect.");
    }

    clearLine(2);
    return 0;
}
