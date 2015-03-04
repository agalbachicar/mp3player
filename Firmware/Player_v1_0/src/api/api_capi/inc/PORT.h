/*
 * PORT.h
 *
 *  Created on: 22/06/2012
 *      Author: Alejandro Celery y Hernán Pérez
 *
 *  Puerto de IO de pines contiguos y manejo rápido.
 */

#ifndef PORT_H_
#define PORT_H_

#include "CAPI_Definitions.h"

typedef struct PORTStruct {
	int pinNameInicial;
	int anchoPort;
	GPIO_Dir direccion;
	GPIO_NivelActividad	nivelAct;
	int ioPort;
	int bit;
	int mascara_bits;
}PORT;

void 			PortInit	(PORT * port, GPIO_Dir, GPIO_NivelActividad nivelAct, int pinNameInicial, int anchoPort);
void 			PortPoner 	(PORT * port, int valor);
unsigned int	PortLeer	(PORT * port);
unsigned int	PortInt2Bar	(int numero);

#endif /* PORT_H_ */
