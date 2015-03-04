/*
 * GPIO.c
 *
 *  Created on: 20/06/2011
 *      Author: Alejandro
 */

#include "CAPI_Definitions.h"
#include "PIN.h"
#include "GPIO.h"

void Direccion			(GPIO *, GPIO_Dir);	// Le da la direccion al GPIO
void Set				(GPIO *);			// Pone el pin +VCC
void Clear				(GPIO *);			// Pone el pin GND

void GPIO_Init(GPIO * gpio, GPIO_Dir dir, GPIO_NivelActividad nivelAct, int pinName) {
	gpio->pinName 	= pinName;
	gpio->direccion = dir;
	gpio->nivelAct	= nivelAct;
	gpio->port		= PN_GET_PORT(pinName);
	gpio->bit		= PN_GET_BIT(pinName);

	PIN_Init(gpio->pinName);

	if(dir == SALIDA) {
		Pasivar(gpio);	// Lo pasivo antes de darle la direccion para evitar glitches
	}
	Direccion(gpio, dir);
}

void Activar(GPIO * gpio) {
	if(gpio->direccion == SALIDA) {
		gpio->nivelAct ? Set(gpio) : Clear(gpio);
	}
}

void Pasivar(GPIO * gpio) {
	if(gpio->direccion == SALIDA) {
		gpio->nivelAct ? Clear(gpio) : Set(gpio);
	}
}

void AsignarNivel(GPIO * gpio, GPIO_NivelActividad nivel) {
	if(gpio->direccion == SALIDA) {
		nivel ? Activar(gpio) : Pasivar(gpio);
	}
}

unsigned int Int2Bar (int numero)
{
	return (1UL << numero) - 1;
}

