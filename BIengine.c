/*
 * BIengine.c
 *
 *  Created on: 15 lut 2015
 *      Author: sylwek
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "BIengine.h"
#include "KLAWIATURA4x4/klawiatura4x4.h"
#include "LCD/hd44780s.h"

extern volatile uint8_t temp_OCR2;
extern volatile uint8_t temp_OCR2_z_wyswietlacza;
volatile int32_t kroczki;
extern uint32_t wysokosc;
uint8_t doganiacz;

void	BIengine_Init(void)
{
	//Ustawienia timera do odmierzania czasu
	//dla TIMERA1 w M8 od CNC
TIM2_PREKSLER1024;
TIM2_CNC_MODE;
TIM2_CNC_INTERRUPT_ENABLE;
DODATKOWY_DZILNIK = temp_OCR2 = PREDKOSC_MAKSYMALNA; //temp_OCR2 służy do zapamiętania prędkości z klawiatury ponieważ prędkość będzie się
									//zmieniać przy joysticku
temp_OCR2_z_wyswietlacza=PREDKOSC_MAKSYMALNA;
doganiacz = PREDKOSC_MINIMALNA;

DDRC |= (1<<ENA) | (1<<CLK) | (1<<DIR);
DDRC &= ~(1<<ZAB);
PORTC |= (1<<ENA) | (1<<ZAB);
}

void start(void)
{
	LCD_str("SZUKAM POCZATKU");
	while( !(PINC & (1<<ZAB)) )
		kroczki=-1;
	lcd_clr();
}

void na_wysokosc(uint32_t zadana_wysokosc)
{
	while(zadana_wysokosc != wysokosc)
	{
		if( (zadana_wysokosc > wysokosc) && kroczki < +5 )
		{
			kroczki+= KROKOW_NA_MM;
			wysokosc++;
			if(zadana_wysokosc - wysokosc<10)
				temp_OCR2=PREDKOSC_MINIMALNA;
		}
		else if(zadana_wysokosc < wysokosc && kroczki > -5)
		{
			kroczki -= KROKOW_NA_MM;
			wysokosc--;
			if(wysokosc - zadana_wysokosc<10)
				temp_OCR2=PREDKOSC_MINIMALNA;
		}
		klawiatura_lcd();
	}
	doganiacz=PREDKOSC_MINIMALNA;

}

ISR(TIMER2_COMP_vect)
{
	static uint8_t cnt;
	static uint8_t flaga;
	if(cnt++>7)
	{	cnt=0;
		//płynne ? przyśpieszanie, zwalnianie silnika
			if(temp_OCR2>doganiacz)
				OCR2=++doganiacz;
			else if(temp_OCR2<doganiacz)
				OCR2=--doganiacz;
	}

		if(kroczki>0 )
		{
			PORTC &= ~( (1<<ENA) | (1<<DIR) );
			if(flaga)
				{PORTC |= (1<<CLK); flaga=0;}

			else
				{PORTC &= ~(1<<CLK); flaga=1; kroczki--; }
		}
		else if(kroczki<0)
		{
			PORTC |= (1<<DIR);
			PORTC &= ~( (1<<ENA) );
			if(flaga)
				{PORTC |= (1<<CLK); flaga=0;}
			else
				{PORTC &= ~(1<<CLK); flaga=1; kroczki++; }
		}
		else
		{
			PORTC |= (1<<ENA);
			temp_OCR2=temp_OCR2_z_wyswietlacza;
			cnt=8;
		}

}

