/*
 * klawiatura4x4.c
 *
 *  Created on: 19 lut 2015
 *      Author: sylwek
 */

#include <avr/io.h>
#include <util/delay.h> // Trzeba się pozbyć tego znaku nieumiejętnośći programowania
#include "klawiatura4x4.h"
#include "../LCD/hd44780s.h"
#include "../BIengine.h"
void Klawiatura4x4_Init(void)
{
	KLAW_DDR = 0xF0; // Pół jako wyjścia a pół jako wejścia
	KLAW_PORT = 0xFF;
}

uint8_t check_key(void)
{
	for(uint8_t i=0;i<4;i++)
	{
		KLAW_PORT = ~(1<<(i+4) );
			_delay_us(1);

		if( (KLAW_PIN & 0x0F) != 0x0F )
		{
			_delay_ms(20);
			if( (KLAW_PIN & 0x0F) != 0x0F )
				return KLAW_PIN;
		}
	}
	return 0;  //zwraca 0 jeśli nie udało się odczytać niczego z pinów
}

uint8_t number(uint8_t key)
{
	switch(key)
	{
	case(PRZY0) : return 0; break;
	case(PRZY1) : return 1; break;
	case(PRZY2) : return 2; break;
	case(PRZY3) : return 3; break;
	case(PRZY4) : return 4; break;
	case(PRZY5) : return 5; break;
	case(PRZY6) : return 6; break;
	case(PRZY7) : return 7; break;
	case(PRZY8) : return 8; break;
	case(PRZY9) : return 9; break;
	//Jeśli liczba zwrócona jest większa od 9
	case(PRZYA) : return 'A'; break;
	case(PRZYB) : return 'B'; break;
	case(PRZYC) : return 'C'; break;
	case(PRZYD) : return 'D'; break;
	case(PRZYP) : return '#'; break;
	case(PRZYG) : return '*'; break;
	default		: return 255; break;
	}
}


void klawiatura_lcd(void)
{
	static char buff[20];
	static uint8_t wsk_buff=1; //Domyślna wartość bo w buff[0] sterujemy znakiem , prawo lewo.
	static int32_t wynik=0;
	uint8_t temp;
	if(kroczki != 0)
	{
		lcd_locate(0,0);
		LCD_str("Working   ");
		lcd_locate(1,0);
	}
	else
	{
		if( (check_key()!=0) && (temp = check_key()) ) //czy klawisz znostał naciśnięty
				{
					if( (number(temp) < 10) )
					{
						wynik = wynik*10 + number(temp);
						if(wynik<0)
							buff[0]='-';
						else
							buff[0]='+';

						buff[wsk_buff]=convert_int_to_char(number(temp));
						buff[++wsk_buff]=0;
						LCD_str("           ");
						lcd_locate(1,0);
						LCD_str(buff);
					}
					else if (number(temp) == 'D' )
					{
						na_wysokosc(wynik);
						wynik=0;
						wsk_buff=1;
						lcd_locate(0,0);
						LCD_str("           ");
						lcd_locate(1,0);
						LCD_str("Sent");
					}
					else if(number(temp) == 'C')
					{
						wynik=0;
						wsk_buff=1;
						buff[19] = 0 ; buff[1]=0;
						lcd_clr();
						lcd_locate(0,0);
						LCD_str("Press ");
						lcd_locate(1,0);
					}
					else if(number(temp) == 'B')
					{
						start();
						/*wynik=wynik*(-1);
						if(wynik<0)
							buff[0]='-';
						else if(wynik ==0)
						{
							if(buff[0] == '-')
								buff[0]='+';
							else
								buff[0]='-';
						}
						else
							buff[0]='+';
						lcd_locate(1,0);
						LCD_str(buff);*/
					}
					else if(number(temp) == 'A')
					{
						if(wynik>=0 && wynik<=213)
						{
							temp_OCR2_z_wyswietlacza = (255 - wynik);
							lcd_clr();
							LCD_str("PREDKOSC");
							lcd_locate(1,0);
							lcd_int(wynik);
							wynik=0;
							wsk_buff=1;
							buff[1]=0;
							_delay_ms(2000);
							lcd_clr();
						}
						else
						{
							lcd_clr();
							LCD_str("Error 1");
							wynik=0;
							wsk_buff=1;
							buff[1]=0;
							_delay_ms(2000);
							lcd_clr();
						}
					}
					_delay_ms(250);
				}
		else
		{
			lcd_locate(0,0);
			LCD_str("Press     ");
			lcd_locate(1,0);
		}
	}
}

char convert_int_to_char(uint8_t number)
{
	if(number<10)
		return (number + '0');
	else return 0;
}
