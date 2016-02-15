/*
 * BIengine.h
 *
 *  Created on: 15 lut 2015
 *      Author: sylwek
 */

#ifndef BIENGINE_H_
#define BIENGINE_H_

#include <avr/io.h>
#include <avr/interrupt.h>

 uint32_t wysokosc;
#define MAX_WYS 2000
#define PREDKOSC_MINIMALNA 220
#define PREDKOSC_MAKSYMALNA 48

#define ENA PC5
#define DIR PC4
#define CLK PC3
#define ZAB PC2

#define KROKOW_NA_MM  80

#define TIM2_PREKSLER1024	 			TCCR2 |= (1<<CS22)   //64
#define TIM2_CNC_MODE 					TCCR2 |= (1<<WGM21)
#define TIM2_CNC_INTERRUPT_ENABLE 		TIMSK |= ( 1<<OCIE2 )
#define DODATKOWY_DZILNIK				OCR2

void	BIengine_Init(void);
void steps(int32_t *ile_krokow);
void obrot(uint8_t krowa);
void start(void);
void na_wysokosc(uint32_t zadana_wysokosc);
#endif /* BIENGINE_H_ */
