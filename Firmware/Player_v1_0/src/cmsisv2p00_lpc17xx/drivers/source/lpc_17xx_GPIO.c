/*
 * lpc_17xx_DigitalOut.cpp
 *
 *  Created on: 20/06/2011
 *      Author: Alejandro
 */

#include "CAPI_Definitions.h"
#include "GPIO.h"
#include "lpc17xx_gpio.h"

void Direccion(GPIO * gpio, GPIO_Dir dir) {
// Le da la direccion al GPIO
	GPIO_SetDir(gpio->port, 1UL << gpio->bit, dir ? 1 : 0);
}

void Set(GPIO * gpio) {
// Pone el pin +VCC
	GPIO_SetValue(gpio->port, 1UL << gpio->bit);
}

void Clear(GPIO * gpio) {
// Pone el pin GND
	GPIO_ClearValue(gpio->port, 1UL << gpio->bit);
}

bool isActivo(GPIO * gpio) {
	uint32_t x = GPIO_ReadValue(gpio->port);
	x >>= gpio->bit;
	x &= 1UL;
	return (bool)(x == gpio->nivelAct ? ALTO : BAJO);
}
