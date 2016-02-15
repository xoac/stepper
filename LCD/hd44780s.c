/*
 * hd44780s.c
 *
 *  Created on: 3 sty 2015
 *      Author: sylwek
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <stdlib.h>
#include <avr/interrupt.h>

#include "hd44780s.h"


#if USE_RW == 1
static inline uint8_t LCD_readHalf(void)
	{
	#if USE_ONEPORT == 1
			_delay_us(2);
			return (PIN(LCD_PORT) & 0xF0);
	#else
		uint8_t r = 0;
		 if(PIN(LCD_D7PORT) & (1<<LCD_D7))	r |= (1<<7);
		 if(PIN(LCD_D6PORT) & (1<<LCD_D6)) 	r |= (1<<6);
		 if(PIN(LCD_D5PORT) & (1<<LCD_D5))	r |= (1<<5);
		 if(PIN(LCD_D4PORT) & (1<<LCD_D4))	r |= (1<<4);
		return r;
	#endif
	}
#endif

void static inline LCD_sendHalf(uint8_t data) // 1111 0000 intereesjue nas tylko starsza część bitu
	{
	#if USE_ONEPORT == 1
		PORT(LCD_PORT) = (PORT(LCD_PORT) & 0x0F) | (data & 0xF0); //Wysłanie starszej części daty
	#else
		if(data&(1<<4)) PORT(LCD_D4PORT) |= (1<<LCD_D4);
		else 			PORT(LCD_D4PORT) &= ~(1<<LCD_D4);

		if(data&(1<<5)) PORT(LCD_D5PORT) |= (1<<LCD_D5);
		else 			PORT(LCD_D5PORT) &= ~(1<<LCD_D5);

		if(data&(1<<6)) PORT(LCD_D6PORT) |= (1<<LCD_D6);
		else 			PORT(LCD_D6PORT) &= ~(1<<LCD_D6);

		if(data&(1<<7)) PORT(LCD_D7PORT) |= (1<<LCD_D7);
		else 			PORT(LCD_D7PORT) &= ~(1<<LCD_D7);

	#endif
	}

#if USE_RW == 1
uint8_t _LCD_read_byte(void)
	{
		uint8_t result=0;
	#if USE_ONEPORT == 1
		DDR(LCD_PORT) = (DDR(LCD_PORT)& 0x0F); // wszystkie piny jako wejscia
	#else
		DDR(LCD_D7PORT) &= ~(1<<LCD_D7);
		DDR(LCD_D6PORT) &= ~(1<<LCD_D6);
		DDR(LCD_D5PORT) &= ~(1<<LCD_D5);
		DDR(LCD_D4PORT) &= ~(1<<LCD_D4);
	#endif

		SET_RW;
		SET_E;
		result = LCD_readHalf();
		CLR_E;

		SET_E;
		result |= (LCD_readHalf()>>4);
		CLR_E;
		return result;
	}
#endif

#if USE_RW == 1
uint8_t LCD_check_BF(void)
	{
		CLR_RS;
		return _LCD_read_byte();
	}
#endif

void _LCD_write_byte(uint8_t data)
	{
	#if USE_ONEPORT == 1
		DDR(LCD_PORT) |= 0xF0; //ustawienie D7...D4 na wyjścia;
	#else
		DDR(LCD_D7PORT) |= (1<<LCD_D7);
		DDR(LCD_D6PORT) |= (1<<LCD_D6);
		DDR(LCD_D5PORT) |= (1<<LCD_D5);
		DDR(LCD_D4PORT) |= (1<<LCD_D4);
	#endif
	#if USE_RW == 1
		CLR_RW;
	#endif
		SET_E;
		LCD_sendHalf(data);
		CLR_E;

		SET_E;
		LCD_sendHalf(data<<4);
		CLR_E;

		#if USE_RW == 1
				while(LCD_check_BF() & (1<<7));
		#else
				_delay_us(120);
		#endif

	}

void LCD_write_data(uint8_t data)
	{
		SET_RS;
		_LCD_write_byte(data);
	}

void LCD_write_cmd(uint8_t cmd)
	{
		CLR_RS;
		_LCD_write_byte(cmd);
	}

void LCD_Initialize(void)
	{
	#if USE_ONEPORT == 1 //Podpięcie do jednego portu
		DDR(LCD_PORT) |= 0xFF;					 //wyszzystko jako wyjścia
		_delay_ms(15); 							// oczekiwanie na ustalibizowanie siê napiecia zasilajacego
		PORT(LCD_PORT) &= ~LCD_RS | ~LCD_E;		 // wyzerowanie linii RS oraz E
		#if USE_RW == 1
			PORT(LCD_PORT) &= ~LCD_RW;
		#endif

			//BRAK DLASZEJ CZĘŚCI !!!

	#else //Dla dowolnego podpięcia do dowolnych pinów procesora
		DDR(LCD_D7PORT) |= (1<<LCD_D7);			//wyszzystko jako wyjścia
		DDR(LCD_D6PORT) |= (1<<LCD_D6);			//
		DDR(LCD_D5PORT) |= (1<<LCD_D5);			//
		DDR(LCD_D4PORT) |= (1<<LCD_D4);			//

		DDR(LCD_RSPORT) |= (1<<LCD_RS);			//
		#if USE_RW == 1
			DDR(LCD_RWPORT) |= (1<<LCD_RW);		//
		#endif
		DDR(LCD_EPORT) |= (1<<LCD_E);			//
		_delay_ms(15); 							// oczekiwanie na ustalibizowanie siê napiecia zasilajacego
		PORT(LCD_RSPORT) &= ~LCD_RS; // wyzerowanie linii RS
		PORT(LCD_EPORT) &= ~LCD_E;  // wyzerowanie linii E
		#if USE_RW == 1
			PORT(LCD_RWPORT) &= ~LCD_RW;
		#endif
	#endif
			SET_E;
			LCD_sendHalf(0x30);
			CLR_E;
			_delay_ms(4.1);
			SET_E;
			LCD_sendHalf(0x30);
			CLR_E;
			_delay_us(100);
			SET_E;
			LCD_sendHalf(0x30);
			CLR_E;
			_delay_us(120);
			SET_E;
			LCD_sendHalf(HD44780_FUNCTION_SET|HD44780_4_BIT);
			CLR_E;
			#if USE_RW == 1
					while(LCD_check_BF() & (1<<7));
			#else
					_delay_us(120);
			#endif
			LCD_write_cmd(HD44780_FUNCTION_SET|HD44780_TWO_LINE|HD44780_FONT5x7|HD44780_4_BIT);
			LCD_write_cmd(HD44780_DISPLAY_ONOFF|HD44780_DISPLAY_OFF);
			LCD_write_cmd(HD44780_CLEAR);
			LCD_write_cmd(HD44780_ENTRY_MODE| HD44780_EM_SHIFT_CURSOR | HD44780_EM_RIGHT);
			LCD_write_cmd(HD44780_DISPLAY_ONOFF | HD44780_DISPLAY_ON | HD44780_CURSOR_ON | HD44780_CURSOR_BLINK);

	}



void lcd_clr(void)
	{
		LCD_write_cmd(HD44780_CLEAR);
		#if USE_RW == 0
				_delay_ms(1.64);
		#endif
	}

void lcd_cursor_on(void)
	{
		LCD_write_cmd(HD44780_DISPLAY_ONOFF|HD44780_DISPLAY_ON|HD44780_CURSOR_ON);
	}

void lcd_cursor_off(void)
	{
		LCD_write_cmd(HD44780_DISPLAY_ONOFF|HD44780_DISPLAY_ON|HD44780_CURSOR_OFF);
	}

#if USE_LCD_CURSOR_BLINK == 1
	void lcd_blink_on(void)
	{
		LCD_write_cmd(HD44780_DISPLAY_ONOFF|HD44780_DISPLAY_ON|HD44780_CURSOR_ON|HD44780_CURSOR_BLINK);
	}

	void lcd_blink_off(void)
	{
		LCD_write_cmd(HD44780_DISPLAY_ONOFF|HD44780_DISPLAY_ON|HD44780_CURSOR_NOBLINK);
	}
#endif



void LCD_str(char *str)
{
	register char znak;
	while( (znak = *(str++)) )
		(znak >=0x80 && znak<=0x87) ? LCD_write_data(znak & 0x7) : LCD_write_data(znak);

}

#if USE_LCD_STR_P == 1
	void lcd_str_P(char *str)
	{
		register char znak;
		while(znak = pgm_read_byte(str++))
			(znak >=0x80 && znak<=0x87) ? LCD_write_data(znak & 0x7) : LCD_write_data(znak);

	}
#endif

#if USE_LCD_STR_E == 1
	void lcd_str_E(char *str)
	{
		register char znak;
		while(1)
		{
			znak= eeprom_read_byte( (uint8_t *)(str++));
			if(!znak || znak == 0xFF) break;
			else
			(znak >=0x80 && znak<=0x87) ? LCD_write_data(znak & 0x7) : LCD_write_data(znak);
		}

	}
#endif

#if USE_LCD_INT == 1
	void lcd_int(int wartosc)
	{
		char bufor[17];
		LCD_str(itoa(wartosc,bufor,10));
	}
#endif

#if USE_LCD_HEX == 1
	void lcd_hex(int wartosc)
	{
		char bufor[17];
		LCD_str(itoa(wartosc,bufor,16));
	}
#endif

#if USE_LCD_DEFCHAR == 1
	//Definicja własnego znaku na LCD z pamięci RAM
	//
	//argumenty:
	//nr -kod znaku w pamięci CGRAM od 0x80 do 0x87
	//*def_znak -wskaźnik do tablicy 7 bajtów definiujących znak
	void lcd_defchar(uint8_t nr, uint8_t *def_znak)
	{
		register uint8_t i,c;
		LCD_write_cmd( 64+((nr&0x07)*8) );
		for(i=0;i<8;i++)
		{
			c = *(def_znak++);
			LCD_write_data(c);
		}
	}
#endif

#if USE_LCD_DEFCHAR_P == 1
	//Definicja własnego znaku na LCD z pamięci RAM
	//
	//argumenty:
	//nr -kod znaku w pamięci CGRAM od 0x80 do 0x87
	//*def_znak -wskaźnik do tablicy 7 bajtów definiujących znak
	void lcd_defchar_P(uint8_t nr, uint8_t *def_znak)
	{
		register uint8_t i,c;
		LCD_write_cmd( 64+((nr&0x07)*8) );
		for(i=0;i<8;i++)
		{
			c = pgm_read_byte(def_znak++);
			LCD_write_data(c);
		}
	}
#endif

#if USE_LCD_DEFCHAR_E == 1
	//Definicja własnego znaku na LCD z pamięci RAM
	//
	//argumenty:
	//nr -kod znaku w pamięci CGRAM od 0x80 do 0x87
	//*def_znak -wskaźnik do tablicy 7 bajtów definiujących znak
	void lcd_defchar_E(uint8_t nr, uint8_t *def_znak)
	{
		register uint8_t i,c;
		LCD_write_cmd( 64+((nr&0x07)*8) );
		for(i=0;i<8;i++)
		{
			c = eeprom_read_byte(def_znak++);
			LCD_write_data(c);
		}
	}
#endif

#if USE_LCD_LOCATE == 1
	void lcd_locate(uint8_t y, uint8_t x)
	{
		switch(y)
		{
		case 0: y = LCD_LINE1; break;

	#if (LCD_Y>1)
		case 1: y = LCD_LINE2; break;
	#endif
	#if (LCD_Y>2)
		case 2: y = LCD_LINE3; break;
	#endif
	#if (LCD_Y>3)
		case 3: y = LCD_LINE4; break;
	#endif

		}
		LCD_write_cmd( (0x80+y+x) );
	}
#endif
