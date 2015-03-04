/*
 * PIN.cpp
 *
 *  Created on: 12/06/2011
 *      Author: Alejandro
 */

#include "PIN.h"
#include "lpc17xx_pinsel.h"

// La unica funcion que me interesa de un PIN es el constructor
void PIN_Init(int pin){
	PINSEL_CFG_Type PinCfg;
	PinCfg.Portnum		= PN_GET_PORT(pin);
	PinCfg.Pinnum		= PN_GET_BIT (pin);
	PinCfg.Funcnum		= PN_GET_FUNC(pin);
	PinCfg.Pinmode		= PN_GET_MODE(pin);
	PinCfg.OpenDrain	= PN_GET_OD  (pin);
	PINSEL_ConfigPin(&PinCfg);
}
