/*
 * BUS.cpp
 *
 *  Created on: 07/09/2010
 *      Author: Trabajando
 */

#include "PIN.h"
#include "GPIO.h"
#include "BUS.h"
#include "CAPI_Definitions.h"

void BusInit (BUS * bus, GPIO_Dir dir, GPIO_NivelActividad nivelAct)
{
	bus->n_pins 	= 0;
	bus->nivelAct 	= nivelAct;
	bus->direccion	= dir;
}

res_t BusAgregarPin(BUS * bus, int pin)
{
	if(bus->n_pins >= MAX_PINS)
		return CAPI_ERROR;

	GPIO_Init( &(bus->busPin[bus->n_pins]) , bus->direccion, bus->nivelAct, pin);

	bus->n_pins++;
	return CAPI_EXITO;
}

void BusPoner (BUS * bus, int valor)
{
	int i;
	for (i = 0; i < bus->n_pins; ++i) {
		AsignarNivel(&bus->busPin[i], BIT_POS(valor, i));
	}
}

long BusLeer 	(BUS * bus)
{
	long valor = 0;
	int i;
	for (i = 0; i < bus->n_pins; ++i) {
		if(isActivo(&bus->busPin[i])){
			valor |= (1UL << i);
		}
	}
	return valor;
}

int BusGetCantidadPines (BUS * bus)
{
	return bus->n_pins;
}
