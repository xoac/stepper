/*
 * joystick.h
 *
 *  Created on: 22 lut 2015
 *      Author: sylwek
 */

#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#define MAX_X_VALUE 880
#define MIN_X_VALUE 165


void ADC_Init(void);
uint16_t readadc(uint8_t ch);
#endif /* JOYSTICK_H_ */
