/*
 * hd44780_S.h
 *
 *  Created on: 2 sty 2015
 *      Author: Sylwek
 *      na podstawie materiałów mirek36
 */

#ifndef HD44780S_H_
#define HD44780S_H_


//==============KONFIGURACJA KONIECZNA=============//
#define LCD_X 		8
#define LCD_Y 		2
#define USE_ONEPORT 	1	//Cały wyświetlacz jest podpięty do jednego portu w tym dane przesyłane są przez starszą część portu
#define USE_RW 			1		// RW podbięte do procesora 1. RW podpięte do GND 0
#if USE_ONEPORT == 		1
	#define LCD_PORT 	B //Jeśli używasz jednego portu edytuj tylko to
	#define LCD_D7 		7 //Tego nie wolno zmieniać
	#define LCD_D6		6 //
	#define LCD_D5 		5 //
	#define LCD_D4 		4 //

	#define LCD_RS 		2 //
	#if USE_RW == 1
		#define LCD_RW 	1 //
	#endif
	#define LCD_E 0 //
#else
	#define LCD_D7PORT	D
	#define LCD_D7 		7
	#define LCD_D6PORT 	D
	#define LCD_D6 		6
	#define LCD_D5PORT 	D
	#define LCD_D5 		5
	#define	LCD_D4PORT 	D
	#define	LCD_D4 		4
	//linie komunikacyje (trybu pracy)
	#define LCD_RSPORT 	D
	#define LCD_RS 		2 //	Register Select signal
	#if USE_RW == 1
		#define LCD_RWPORT 	D
		#define LCD_RW  	1 // Data read / write
	#endif
	#define LCD_EPORT 	D
	#define LCD_E  		0//Enable Signal

#endif
//=================== KONIEC KONFIGURACJI KONIECZNEJ===========================//
//=============================================================================//
//===================KONFIGURACJA ZAAWANSOWANA ================================//
#define USE_LCD_LOCATE  1   //1                    // ustawia kursor na wybranej pozycji Y,X (Y=0-3, X=0-n)
#define USE_LCD_CHAR    0 	//1                // wysyła pojedynczy znak jako argument funkcji

#define USE_LCD_STR_P   0                      // wysyła string umieszczony w pamiêci FLASH
#define USE_LCD_STR_E   0                       // wysyła string umieszczony w pamiêci FLASH

#define USE_LCD_INT     1    //1                  // wyświetla liczbê dziesietn¹ na LCD
#define USE_LCD_HEX     0                       // wyświetla liczbê szesnastkow¹ na LCD

#define USE_LCD_DEFCHAR         0  //1          // wysyła zdefiniowany znak z pamiêci RAM
#define USE_LCD_DEFCHAR_P       0               // wysyła zdefiniowany znak z pamiêci FLASH
#define USE_LCD_DEFCHAR_E       0               // wysyła zdefiniowany znak z pamiêci EEPROM

#define USE_LCD_CURSOR_ON       1       // obsługa włczania/wyłczania kursora
#define USE_LCD_CURSOR_BLINK    0      // obsługa włczania/wyłczania migania kursora
#define USE_LCD_CURSOR_HOME     0       // ustawia kursor na pozycji pocz¹tkowej

//===================KONIEC KONFIGURACJI ZAAWANSOWANEJ ========================//

#if ( (LCD_Y==4) && (LCD_X==20) )
#define LCD_LINE1 0x00  //adres 1 znaku 1 wiersza
#define LCD_LINE2 0x28  //adres 1 znaku 2 wiersza
#define LCD_LINE3 0x14  //adres 1 znaku 3 wiersza
#define LCD_LINE4 0x54  //adres 1 znaku 4 wiersza
#else
#define LCD_LINE1 0x00  //adres 1 znaku 1 wiersza
#define LCD_LINE2 0x40  //adres 1 znaku 2 wiersza
#define LCD_LINE3 0x10  //adres 1 znaku 3 wiersza
#define LCD_LINE4 0x50  //adres 1 znaku 4 wiersza
#endif

#define PORT(x) SPORT(x)
#define SPORT(x) (PORT##x)
#define PIN(x) SPIN(x)
#define SPIN(x) (PIN##x)
#define DDR(x) SDDR(x)
#define SDDR(x) (DDR##x)

//***********************************************************
#define HD44780_CLEAR					1		//0x01		*
														  //*
#define HD44780_HOME					(1<<1)	//0x02		*

#define HD44780_ENTRY_MODE				(1<<2)	//0x04
	#define HD44780_EM_SHIFT_CURSOR		0
	#define HD44780_EM_SHIFT_DISPLAY	1
	#define HD44780_EM_RIGHT			(1<<1)
	#define HD44780_EM_LEFT				0

#define HD44780_DISPLAY_ONOFF			(1<<3)	//0x08
	#define HD44780_DISPLAY_OFF			0		//0
	#define HD44780_DISPLAY_ON			(1<<2)	//4
	#define HD44780_CURSOR_OFF			0		//0
	#define HD44780_CURSOR_ON			(1<<1)	//2
	#define HD44780_CURSOR_NOBLINK		0		//0
	#define HD44780_CURSOR_BLINK		1		//1

#define HD44780_DISPLAY_CURSOR_SHIFT	(1<<4)	//0x10
	#define HD44780_SHIFT_CURSOR		0
	#define HD44780_SHIFT_DISPLAY		(1<<3)	//8
	#define HD44780_SHIFT_LEFT			0
	#define HD44780_SHIFT_RIGHT			(1<<2)	//4

#define HD44780_FUNCTION_SET			(1<<5)	//0x20
	#define HD44780_FONT5x7				0
	#define HD44780_FONT5x10			(1<<2)	//4
	#define HD44780_ONE_LINE			0
	#define HD44780_TWO_LINE			(1<<3)	//8
	#define HD44780_4_BIT				0
	#define HD44780_8_BIT				(1<<4)	//16

#define HD44780_CGRAM_SET				(1<<6)	//0x40		*
														  //*
#define HD44780_DDRAM_SET				(1<<7)	//0x80		*
//***********************************************************



//========================Operacje=======S================
#if USE_ONEPORT == 1
	#define SET_RS PORT(LCD_PORT) |= (1<<LCD_RS) //high state
	#define CLR_RS PORT(LCD_PORT) &= ~(1<<LCD_RS) //low state
			#if USE_RW == 1
	#define SET_RW PORT(LCD_PORT) |= (1<<LCD_RW) //high read
	#define CLR_RW PORT(LCD_PORT) &= ~(1<<LCD_RW) //low read
			#endif
	#define SET_E PORT(LCD_PORT) |= (1<<LCD_E)
	#define CLR_E PORT(LCD_PORT) &= ~(1<<LCD_E)
#else
	#define SET_RS PORT(LCD_RSPORT) |= (1<<LCD_RS) //high state
	#define CLR_RS PORT(LCD_RSPORT) &= ~(1<<LCD_RS) //low state
			#if USE_RW == 1
	#define SET_RW PORT(LCD_RWPORT) |= (1<<LCD_RW) //high read
	#define CLR_RW PORT(LCD_RWPORT) &= ~(1<<LCD_RW) //low read
			#endif
	#define SET_E PORT(LCD_EPORT) |= (1<<LCD_E)
	#define CLR_E PORT(LCD_EPORT) &= ~(1<<LCD_E)
#endif
//========================================================

void LCD_Initialize(void);
void LCD_write_cmd(uint8_t cmd);
void LCD_write_data(uint8_t data);

void lcd_int(int wartosc);
void lcd_clr(void);
void lcd_blink_on(void);
void lcd_cursor_on(void);
void LCD_str(char *str);
void lcd_str_P(char *str);
void lcd_str_E(char *str);
void LCD_str_move(char *str);


void lcd_locate(uint8_t y, uint8_t x);

#endif /* HD44780S_H_ */
