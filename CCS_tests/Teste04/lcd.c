/*
 * lcd.c
 *
 *  Created on: 13 de out de 2024
 *      Author: Heverton Reis
 */
#include "lcd.h"

Int16 osd9616_send(Uint16 comdat, Uint16 data)
{
    Uint16 cmd[2];
    cmd[0] = comdat & 0x00FF;     // Specifies whether data is Command or Data
    cmd[1] = data;                // Command / Data

    /* Write to OSD9616 */
    return EZDSP5502_I2C_write(OSD9616_I2C_ADDR, cmd, 2);
}

Int16 osd9616_multiSend(Uint16* data, Uint16 len)
{
    Uint16 x;
    Uint16 cmd[10];
    for(x=0;x<len;x++)               // Command / Data
    {
        cmd[x] = data[x];
    }

    /* Write len bytes to OSD9616 */
    return EZDSP5502_I2C_write(OSD9616_I2C_ADDR, cmd, len);
}

Int16 osd9616_init(void){

    Uint16 cmd[10];    // For multibyte commands

    /* Initialize LCD power */
    EZDSP5502_GPIO_init( GPIO_GPIO_PIN1 );          // Enable GPIO pin
    EZDSP5502_GPIO_setDirection( GPIO_GPIO_PIN1, GPIO_GPIO_PIN1_OUTPUT );  // Output
    EZDSP5502_GPIO_setOutput( GPIO_GPIO_PIN1, 1 );  // Enable 13V

    /* Initialize osd9616 display */
    osd9616_send(0x00,0x00);  // Set low column address
    osd9616_send(0x00,0x10);  // Set high column address
    osd9616_send(0x00,0x40);  // Set start line address

    /* Set contrast control register */
    cmd[0] = 0x00 & 0x00FF;
    cmd[1] = 0x81;
    cmd[2] = 0x7f;
    osd9616_multiSend( cmd, 3 );

    osd9616_send(0x00,0xa1);  // Set segment re-map 95 to 0
    osd9616_send(0x00,0xa6);  // Set normal display

    /* Set multiplex ratio(1 to 16) */
    cmd[0] = 0x00 & 0x00FF;
    cmd[1] = 0xa8;
    cmd[2] = 0x0f;
    osd9616_multiSend( cmd, 3 );

    osd9616_send(0x00,0xd3);  // Set display offset
    osd9616_send(0x00,0x00);  // Not offset
    osd9616_send(0x00,0xd5);  // Set display clock divide ratio/oscillator frequency
    osd9616_send(0x00,0xf0);  // Set divide ratio

    /* Set pre-charge period */
    cmd[0] = 0x00 & 0x00FF;
    cmd[1] = 0xd9;
    cmd[2] = 0x22;
    osd9616_multiSend( cmd, 3 );

    /* Set com pins hardware configuration */
    cmd[0] = 0x00 & 0x00FF;
    cmd[1] = 0xda;
    cmd[2] = 0x02;
    osd9616_multiSend( cmd, 3 );

    osd9616_send(0x00,0xdb);  // Set vcomh
    osd9616_send(0x00,0x49);  // 0.83*vref

    /* Set DC-DC enable */
    cmd[0] = 0x00 & 0x00FF;
    cmd[1] = 0x8d;
    cmd[2] = 0x14;
    osd9616_multiSend( cmd, 3 );

    osd9616_send(0x00,0xaf); // Turn on oled panel

    return 0;
}

Int16 printLetter(Uint16 c4,Uint16 c3,Uint16 c2,Uint16 c1)
{
    osd9616_send(0x40, c4);    // Column 4
    osd9616_send(0x40, c3);    // Column 3
    osd9616_send(0x40, c2);    // Column 2
    osd9616_send(0x40, c1);    // Column 1
    osd9616_send(0x40, 0x00);

    return 0;
}
