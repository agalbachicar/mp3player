/*
 * PIN.h
 *
 *  Created on: 12/06/2011
 *      Author: Alejandro
 */

#ifndef PIN_H_
#define PIN_H_

void PIN_Init(int pin);

// Macros para codificar los PinNames
#define PN_PIN(n)			(n <<  0)	// 0,1,2,3,4
#define PN_PORT(n)			(n <<  5)	// 5,6,7,8
#define PN_FUNC(n)			(n <<  9)	// 9,10,11
#define PN_MODO_PULL_UP		(0 << 12)	// 12,13,14
#define PN_MODO_PULL_DOWN	(1 << 12)	// 12,13,14
#define PN_MODO_PULL_NONE	(2 << 12)	// 12,13,14
#define PN_MODO_REPEATER	(3 << 12)	// 12,13,14
#define PN_OPEN_DRAIN		(1 << 15)	// 15

// Macros para decodificar los atributos del pin
// Pin: OMMM | FFFp | pppp | PPPP
#define PN_GET_BIT(pin)		((pin >>  0) & 0x001F)	// 5 bits
#define PN_GET_PORT(pin)	((pin >>  5) & 0x000F)	// 4 bits
#define PN_GET_FUNC(pin)	((pin >>  9) & 0x0007)	// 3 bits
#define PN_GET_MODE(pin)	((pin >> 12) & 0x0007)	// 3 bits
#define PN_GET_OD(pin)		((pin >> 15) & 0x0001)	// 1 bits

#endif /* PIN_H_ */
