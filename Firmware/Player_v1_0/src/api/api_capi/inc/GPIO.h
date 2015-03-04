/*
 * GPIO.h
 *
 *  Created on: 15/02/2012
 *      Author: alejandro
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "CAPI_Definitions.h"

typedef struct _GPIO {
	int pinName;
	GPIO_Dir direccion;
	GPIO_NivelActividad	nivelAct;
	int port;
	int bit;
}GPIO;

void GPIO_Init			(GPIO *, GPIO_Dir, GPIO_NivelActividad nivelAct, int pinName);

void Activar			(GPIO *);
void Pasivar			(GPIO *);
void AsignarNivel		(GPIO *, GPIO_NivelActividad nivel);
bool isActivo			(GPIO *);

unsigned int Int2Bar (int numero);

#endif /* GPIO_H_ */
