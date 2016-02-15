/*
 * main.c
 *
 *  Created on: 15 lut 2015
 *      Author: sylwek
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

#include "BIengine.h"
#include "KLAWIATURA4x4/klawiatura4x4.h"
#include "LCD/hd44780s.h"
#include "Joystick/joystick.h"

extern volatile int32_t kroczki;
extern uint32_t wysokosc;
extern uint8_t doganiacz;

int main(int argc, char **argv) {

	LCD_Initialize();
	Klawiatura4x4_Init();
	BIengine_Init();
	ADC_Init();
		sei();
		uint16_t x;
start();
	while(1)
		{

		klawiatura_lcd();

	     x=readadc(0);      //READ ADC VALUE FROM PA.0


			if(x<=406 && kroczki <20 )
			{
				if(wysokosc<MAX_WYS)
				{
					kroczki+=KROKOW_NA_MM;
					wysokosc++;
					temp_OCR2= PREDKOSC_MAKSYMALNA + ((x+2)/2);
				}
			}
			else if(x>=619 && kroczki > -20)
			{
				if(wysokosc > 0)
				{
					kroczki-=KROKOW_NA_MM;
					wysokosc--;
					temp_OCR2 = PREDKOSC_MAKSYMALNA + (uint32_t)((1025-x)/2);
				}
			}
			else if(kroczki == 0 && x > 414 && x < 609)
			{
				doganiacz=PREDKOSC_MINIMALNA;
			}

		}


}

/*
 * while(1)
	{
		//Koniecznie inny pomysł!
	//	 y=readadc(0);      //READ ADC VALUE FROM PA.0
		 x=readadc(1);      //READ ADC VALUE FROM PA.1



	 klawiatura_lcd();
		if(kroczki == 0)
		{
			 if( Tms_f && ( (MAX_X_VALUE-200) < x ) )
			 {
				 if(x>MAX_X_VALUE)
					 DODATKOWY_DZILNIK = 55;
				 else
					 DODATKOWY_DZILNIK = MAX_X_VALUE - x +55;
				 obrot(1);
				 Tms_f=0;
			 }
			 else if(Tms_f && (MIN_X_VALUE +200) > x)
			 {
				 if(x<MIN_X_VALUE)
					 DODATKOWY_DZILNIK = 55;
				 else
					 DODATKOWY_DZILNIK = x - MIN_X_VALUE + 55;
				 obrot(2);
				 Tms_f=0;
			 }
		}
		 else if(Tms_f )
		 		{
		 			//Potrzeba jeszcze zpisywać prędkość z klawiatury bo
		 			//joystick zmienia jej wartość :(
		 			steps( (&kroczki) );
		 			Tms_f=0;
		 		}


	}
 */
