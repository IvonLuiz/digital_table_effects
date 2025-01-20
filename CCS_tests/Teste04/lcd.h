/*
 * lcd.h
 *
 *  Created on: 13 de out de 2024
 *      Author: Heverton Reis
 */
#ifndef LCD_H_
#define LCD_H_

#include"ezdsp5502_i2c.h"
#include"ezdsp5502_gpio.h"
#include "csl_gpio.h"

#define OSD9616_I2C_ADDR 0x3C    // Endereco I2C OSD9616

Int16 osd9616_init(void);
Int16 osd9616_send(Uint16 comdat, Uint16 data);
Int16 osd9616_multiSend(Uint16* data, Uint16 len);
Int16 printLetter(Uint16 l1,Uint16 l2,Uint16 l3,Uint16 l4);

#endif /* LCD_H_ */
