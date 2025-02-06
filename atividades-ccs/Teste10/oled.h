/*
 * oled.h
 *
 *  Created on: 26 de out de 2024
 *      Author: Heverton Reis
 */

#ifndef OLED_H_
#define OLED_H_

void printChar(char c);
void printString(const char* str);
void writeOnPage(Uint16 page);
void clearPage(Uint16 line);
void printSpace(Uint16 spaces);
void message(Uint16 page, const char* text);
void initOLED(void);

#endif /* OLED_H_ */
