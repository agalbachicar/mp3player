/*
 * BusOut.h
 *
 *  Created on: 07/09/2010
 *      Author: Trabajando
 */

#ifndef BUSOUT_H_
#define BUSOUT_H_

#include "GPIO.h"

#define MAX_PINS 8  // puedo manejar hasta los 32 bits de un puerto
	// Lo capeo en 8 porque cada BUSStruct lleva un GPIO y es mucha memoria!

typedef struct BUSStruct {
	int	n_pins;
	GPIO busPin[MAX_PINS];
	int nivelAct;
	int direccion;
}BUS;

void	BusInit				(BUS * bus, GPIO_Dir, GPIO_NivelActividad);
res_t 	BusAgregarPin 		(BUS * bus, int pinName);
void 	BusPoner			(BUS * bus, int valor);
long 	BusLeer 			(BUS * bus);
int 	BusGetCantidadPines (BUS * bus);

#endif /* BUSOUT_H_ */
