/*
 * timer.h
 *
 *  Created on: 13 de out de 2024
 *      Author: Heverton Reis
 */

#ifndef TIMER_H_
#define TIMER_H_

void initTimer0(void);
void startTimer0(void);
interrupt void gpt0Isr(void);
void changeTimer(void);

#endif /* TIMER_H_ */
