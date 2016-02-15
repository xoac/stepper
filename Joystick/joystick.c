/*
 * joystick.c
 *
 *  Created on: 22 lut 2015
 *      Author: sylwek
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "joystick.h"
#include "../LCD/hd44780s.h"

void ADC_Init(void)
{
    ADMUX |= (1<<REFS0);
    ADCSRA|=(1<<ADEN)|(1<<ADPS1)|(1<<ADPS2); //ENABLE ADC, PRESCALER 128
}
uint16_t readadc(uint8_t ch)
{
    ch &= 0b00000111;         //ANDing to limit input to 7
    ADMUX = (ADMUX & 0xf8) | ch;  //Clear last 3 bits of ADMUX, OR with ch
    ADCSRA|=(1<<ADSC);        //START CONVERSION
    while((ADCSRA)&(1<<ADSC));    //WAIT UNTIL CONVERSION IS COMPLETE
    return(ADC);        //RETURN ADC VALUE
}
