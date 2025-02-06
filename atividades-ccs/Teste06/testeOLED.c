#include"ezdsp5502.h"
#include"lcd.h"

const Uint16 fonte[65][4] = {
    // Letras maiúsculas A-Z
    {0x1F, 0x05, 0x05, 0x1F},  // A
    {0x36, 0x49, 0x49, 0x7F},  // B
    {0x22, 0x41, 0x41, 0x3E},  // C
    {0x3E, 0x41, 0x41, 0x7F},  // D
    {0x41, 0x49, 0x49, 0x7F},  // E
    {0x01, 0x09, 0x09, 0x7F},  // F
    {0x72, 0x51, 0x41, 0x3E},  // G
    {0x7F, 0x08, 0x08, 0x7F},  // H
    {0x00, 0x41, 0x7F, 0x41},  // I
    {0x3F, 0x41, 0x40, 0x20},  // J
    {0x63, 0x14, 0x08, 0x7F},  // K
    {0x40, 0x40, 0x40, 0x7F},  // L
    {0x7F, 0x02, 0x02, 0x7F},  // M
    {0x7F, 0x18, 0x06, 0x7F},  // N
    {0x3E, 0x41, 0x41, 0x3E},  // O
    {0x06, 0x09, 0x09, 0x7F},  // P
    {0x3E, 0x61, 0x41, 0x3E},  // Q
    {0x66, 0x19, 0x09, 0x7F},  // R
    {0x32, 0x49, 0x49, 0x26},  // S
    {0x01, 0x01, 0x7F, 0x01},  // T
    {0x3F, 0x40, 0x40, 0x3F},  // U
    {0x1F, 0x20, 0x20, 0x1F},  // V
    {0x7F, 0x20, 0x20, 0x7F},  // W
    {0x63, 0x08, 0x14, 0x63},  // X
    {0x03, 0x04, 0x78, 0x03},  // Y
    {0x47, 0x49, 0x51, 0x61},  // Z

    // Letras minúsculas a-z
    {0x78, 0x54, 0x54, 0x20},  // a
    {0x30, 0x48, 0x48, 0x7F},  // b
    {0x28, 0x44, 0x44, 0x38},  // c
    {0x7F, 0x48, 0x48, 0x30},  // d
    {0x18, 0x54, 0x54, 0x38},  // e
    {0x01, 0x09, 0x7E, 0x08},  // f
    {0x3C, 0x54, 0x54, 0x08},  // g
    {0x70, 0x08, 0x08, 0x7F},  // h
    {0x40, 0x7A, 0x48, 0x00},  // i
    {0x3A, 0x48, 0x40, 0x20},  // j
    {0x44, 0x28, 0x10, 0x7F},  // k
    {0x00, 0x40, 0x7F, 0x41},  // l
    {0x78, 0x04, 0x04, 0x78},  // m
    {0x78, 0x04, 0x08, 0x7C},  // n
    {0x38, 0x44, 0x44, 0x38},  // o
    {0x08, 0x14, 0x14, 0x7C},  // p
    {0x7C, 0x14, 0x14, 0x08},  // q
    {0x08, 0x04, 0x08, 0x7C},  // r
    {0x24, 0x54, 0x54, 0x48},  // s
    {0x20, 0x44, 0x3F, 0x04},  // t
    {0x7C, 0x40, 0x40, 0x3C},  // u
    {0x1C, 0x20, 0x20, 0x1C},  // v
    {0x3C, 0x40, 0x40, 0x3C},  // w
    {0x44, 0x28, 0x28, 0x44},  // x
    {0x3C, 0x50, 0x50, 0x0C},  // y
    {0x4C, 0x54, 0x64, 0x44},  // z

    // Números 0-9
    {0x45, 0x49, 0x51, 0x3E},  // 0
    {0x40, 0x7F, 0x42, 0x00},  // 1
    {0x49, 0x51, 0x61, 0x42},  // 2
    {0x3B, 0x45, 0x41, 0x21},  // 3
    {0x7F, 0x12, 0x14, 0x18},  // 4
    {0x39, 0x45, 0x45, 0x27},  // 5
    {0x31, 0x49, 0x4A, 0x3C},  // 6
    {0x07, 0x09, 0x71, 0x01},  // 7
    {0x36, 0x49, 0x49, 0x36},  // 8
    {0x1E, 0x29, 0x49, 0x06},  // 9

    // Espaço e caracteres especiais
    {0x00, 0x00, 0x00, 0x00},  // ' ' (Espaço)
    {0x00, 0x00, 0x5F, 0x00},  // '!' (Exclamacao)
    {0x0E, 0x51, 0x01, 0x02}  // '?' (Interrogacao)
};

int i = 0;

void printChar(char c) {
    int index;

    // Converter caractere para indice da tabela (A-Z, a-z, 0-9)
    if (c >= 'A' && c <= 'Z') {
        index = c - 'A';  // Indice para letras maiusculas
    } else if (c >= 'a' && c <= 'z') {
        index = c - 'a' + 26;  // Indice para letras minusculas
    } else if (c >= '0' && c <= '9') {
        index = c - '0' + 52;  // Indice para numeros
    } else if (c == ' ') {
        index = 62;  // Indice para o espaço
    } else if (c == '!') {
        index = 63;  // Indice para o caractere '!'
    } else if (c == '?') {
        index = 64;  // Indice para o caractere '?'
    } else {
        return;  // Caractere nao suportado
    }

    // Usar a funcao printLetter com os valores da tabela de fontes
    printLetter(fonte[index][0], fonte[index][1], fonte[index][2], fonte[index][3]);
}

void printString(const char* str) {
    // Encontrar o fim da string
    const char* end = str;
    while (*end) {
       end++;
    }

    // Agora 'end' aponta para o caractere nulo '\0'.
    // Percorrer a string ao contrário, do fim para o começo
    while (end != str) {
       end--;         // Voltar um caractere antes do '\0'
       printChar(*end); // Imprimir o caractere
    }
}

void writeOnPage(Uint16 page){
    osd9616_send(0x00, 0x00);           // Define endereco da coluna de baixo
    osd9616_send(0x00, 0x10);           // Define endereco da coluna de cima
    osd9616_send(0x00, 0xb0 + page);    // Define a linha (pagina)
}

void clearLine(Uint16 line) {

    writeOnPage(line);

    for (i = 0; i < 128; i++) {
        osd9616_send(0x40, 0x00);       // Envia 0x00 para desligar todos os pixels na linha
    }
}

void printSpace(Uint16 spaces){

    for (i = 0; i < spaces; i++) {
        osd9616_send(0x40,0x00);
    }
}

Int16 oled_test()
{
    //Int16 i;
    //Uint16 cmd[10];                    // Para comandos multibyte

    /* Garante que o display esteja configurado corretamente e nao esteja em modo de rolagem antes de qualquer operacao de escrita. */
    osd9616_init();                   // Inicializa o Display
    osd9616_send(0x00,0x2e);           // Desativa o Scrolling

    clearLine(0);                      // Limpa a linha 0

    /* Escrever na pagina 0 */
    writeOnPage(0);                     // Define endereco da linha inicial

    printString("HEY! HELLO WORLD!!?"); // Limite de caracteres que podem ser exibidos na linha de um unica vez

    clearLine(1);                       // Limpa a linha 1

    /* Escrever na pagina 1 */
    writeOnPage(1);         // Define endereco da linha inicial

    printString("este texto esta ocupando");

    clearLine(2);                       // Limpa a linha 2

    /* Escrever na pagina 2 */
    writeOnPage(2);         // Define endereco da linha inicial

    printString("duas linhas a mais");

    /* Desativar a rolagem vertical */
    //osd9616_send(0x00, 0x2E);  // Desativa a rolagem

    /* Configuração de rolagem vertical e horizontal */
    /*cmd[0] = 0x00;
    cmd[1] = 0x28;  // Rolagem vertical e horizontal para a direita
    cmd[2] = 0x00;  // Byte ficticio
    cmd[3] = 0x01;  // Define o endereço da pagina inicial (pagina 2)
    cmd[4] = 0x03;  // Define o intervalo de tempo entre cada passo da rolagem
    cmd[5] = 0x01;  // Define o endereço da pagina final (pagina 3)
    cmd[6] = 0x00;  // Offset de rolagem vertical
    osd9616_multiSend(cmd, 7);
    osd9616_send(0x00, 0x2F);  // Ativar Scrolling

    /* Manter as primeiras 8 linhas fora da rolagem vertical */
    /*cmd[0] = 0x00;
    cmd[1] = 0xa3;  // Configura area de rolagem vertical
    cmd[2] = 0x08;  // Define o número de linhas na área fixa superior
    cmd[3] = 0x10;  // Define o número de linhas na área de rolagem (16 linhas para cobrir as páginas 2 e 3)
    osd9616_multiSend(cmd, 4);*/

    return 0;
}
