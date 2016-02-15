/*
 * klawiatura4x4.h
 *
 *  Created on: 19 lut 2015
 *      Author: sylwek
 */

#ifndef KLAWIATURA4X4_H_
#define KLAWIATURA4X4_H_
#include <avr/io.h>

extern volatile int32_t kroczki;
volatile uint8_t temp_OCR2;
volatile uint8_t temp_OCR2_z_wyswietlacza;

#define PRZY1 0b01110111
#define PRZY4 0b10110111
#define PRZY7 0b11010111
#define PRZYG 0b11100111

#define PRZY2 0b01111011
#define PRZY5 0b10111011
#define PRZY8 0b11011011
#define PRZY0 0b11101011

#define PRZY3 0b01111101
#define PRZY6 0b10111101
#define PRZY9 0b11011101
#define PRZYP 0b11101101

#define PRZYA 0b01111110
#define PRZYB 0b10111110
#define PRZYC 0b11011110 //PRZY#
#define PRZYD 0b11101110


#define KLAW_DDR DDRD
#define KLAW_PORT PORTD
#define KLAW_PIN PIND

void Klawiatura4x4_Init(void);
uint8_t check_key(void);
uint8_t number(uint8_t key);
char znak(uint8_t key);
char convert_int_to_char(uint8_t number);
void klawiatura_lcd(void);
#endif /* KLAWIATURA4X4_H_ */
