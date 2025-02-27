/*
 * oled.c
 *
 *  Created on: 13 de out de 2024
 *      Author: Heverton Reis
 */
#include "lcd.h"

Int16 oled_start()
{
    Int16 i;
    Uint16 cmd[10];              // For multibyte commands

    /* Initialize  Display */
    osd9616_init( );

    osd9616_send(0x00,0x2e);     // Deactivate Scrolling

    /* Fill page 0 */
    i = osd9616_send(0x00,0x00); // Set low column address
    osd9616_send(0x00,0x10);     // Set high column address
    osd9616_send(0x00,0xb0+0);   // Set page for page 0 to page 5
    for(i=0;i<128;i++)
    {
        osd9616_send(0x40,0xff);
    }

    /* Write to page 0 */
    osd9616_send(0x00,0x00);     // Set low column address
    osd9616_send(0x00,0x10);     // Set high column address
    osd9616_send(0x00,0xb0+0);   // Set page for page 0 to page 5
    for(i=0;i<22;i++)
    {
        osd9616_send(0x40,0x00); // Spaces
    }

    /* Writing letters in a dot matrix
     * Each byte is 8 lines of a column */
    printLetter(0x32,0x49,0x49,0x26);  // S
    printLetter(0x01,0x7F,0x01,0x01);  // T
    printLetter(0x7F,0x30,0x0E,0x7F);  // N
    printLetter(0x41,0x49,0x49,0x7F);  // E
    printLetter(0x7F,0x06,0x06,0x7F);  // M
    printLetter(0x3F,0x40,0x40,0x3F);  // U
    printLetter(0x46,0x29,0x19,0x7F);  // R
    printLetter(0x01,0x7F,0x01,0x01);  // T
    printLetter(0x32,0x49,0x49,0x26);  // S
    printLetter(0x7F,0x30,0x0E,0x7F);  // N
    printLetter(0x00,0x7F,0x00,0x00);  // I
    for(i=0;i<5;i++)
    {
        osd9616_send(0x40,0x00);  // Spaces
    }
    printLetter(0x32,0x49,0x49,0x26);  // S
    printLetter(0x7C,0x09,0x0A,0x7C);  // A
    printLetter(0x63,0x1C,0x1C,0x63);  // X
    printLetter(0x41,0x49,0x49,0x7F);  // E
    printLetter(0x01,0x7F,0x01,0x01);  // T

    for(i=0;i<23;i++)
    {
        osd9616_send(0x40,0x00);  // Spaces
    }

    /* Fill page 1*/
    osd9616_send(0x00,0x00);   // Set low column address
    osd9616_send(0x00,0x10);   // Set high column address
    osd9616_send(0x00,0xb0+1); // Set page for page 0 to page 5
    for(i=0;i<128;i++)
    {
        osd9616_send(0x40,0xff);
    }

    /* Write to page 1*/
    osd9616_send(0x00,0x00);   // Set low column address
    osd9616_send(0x00,0x10);   // Set high column address
    osd9616_send(0x00,0xb0+1); // Set page for page 0 to page 5
    for(i=0;i<46;i++)
    {
        osd9616_send(0x40,0x00);
    }

    printLetter(0x06,0x09,0x09,0x7F);  // P
    printLetter(0x32,0x49,0x49,0x26);  // S
    printLetter(0x3E,0x41,0x41,0x7F);  // D
    printLetter(0x43,0x4D,0x51,0x61);  // Z
    printLetter(0x10,0x58,0x58,0x30);  // e
    for(i=0;i<5;i++)
    {
        osd9616_send(0x40,0x00);  // Spaces
    }
    printLetter(0x47,0x49,0x51,0x62);  // 2
    printLetter(0x3E,0x41,0x41,0x3E);  // 0
    printLetter(0x31,0x49,0x49,0x2F);  // 5
    printLetter(0x31,0x49,0x49,0x2F);  // 5
    printLetter(0x22,0x41,0x41,0x3E);  // C

    for(i=0;i<47;i++)
    {
        osd9616_send(0x40,0x00);  // Spaces
    }

    /* Write to page 2 */
    osd9616_send(0x00,0x00);   // Set low column address
    osd9616_send(0x00,0x10);   // Set high column address
    osd9616_send(0x00,0xb0+2); // Set page for page 0 to page 5

    for(i=0;i<20;i++)
    {
        osd9616_send(0x40,0x00);  // Spaces
    }

    printLetter(0x41,0x49,0x49,0x7F);  // E
    printLetter(0x7F,0x30,0x0E,0x7F);  // N
    printLetter(0x3E,0x41,0x41,0x3E);  // O
    printLetter(0x01,0x7F,0x01,0x01);  // T
    printLetter(0x14,0x14,0x14,0x14);  // =
    printLetter(0x47,0x49,0x51,0x62);  // 2
    printLetter(0x7F,0x30,0x30,0x7F);  // W
    printLetter(0x32,0x49,0x49,0x26);  // S
    for(i=0;i<5;i++)
    {
        osd9616_send(0x40,0x00);  // Spaces
    }
    printLetter(0x46,0x29,0x19,0x7F);  // R
    printLetter(0x41,0x49,0x49,0x7F);  // E
    printLetter(0x7F,0x06,0x06,0x7F);  // M
    printLetter(0x00,0x7F,0x00,0x00);  // I
    printLetter(0x01,0x7F,0x01,0x01);  // T
    printLetter(0x14,0x14,0x14,0x14);  // =
    printLetter(0x40,0x7F,0x42,0x00);  // 1
    printLetter(0x7F,0x30,0x30,0x7F);  // W
    printLetter(0x32,0x49,0x49,0x26);  // S

    for(i=0;i<18;i++)
    {
        osd9616_send(0x40,0x00);  // Spaces
    }

    cmd[0] = 0x00;
    cmd[1] = 0x20;       // Set Vertical Scroll Area
    cmd[2] = 0x02;       // Set No. of rows in top fixed area
    osd9616_multiSend( cmd, 3 );

    /* Set vertical and horizontal scrolling */
    cmd[0] = 0x00;
    cmd[1] = 0x29;       // Vertical and Right Horizontal Scroll
    cmd[2] = 0x00;       // Dummy byte
    cmd[3] = 0x00;       // Define start page address
    cmd[4] = 0x03;       // Set time interval between each scroll step
    cmd[5] = 0x02;       // Define end page address
    cmd[6] = 0x01;       // Vertical scrolling offset
    osd9616_multiSend( cmd, 7 );

    osd9616_send(0x00,0x2f);  // Start Scrolling

    /* Keep first 8 rows from vertical scrolling  */
    cmd[0] = 0x00;
    cmd[1] = 0xa3;       // Set Vertical Scroll Area
    cmd[2] = 0x08;       // Set No. of rows in top fixed area
    cmd[3] = 0x10;       // Set No. of rows in scroll area
    osd9616_multiSend( cmd, 4 );

    return 0;
}
