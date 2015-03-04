/*
 * UART.c
 *
 *  Created on: 21/06/2011
 *      Author: Alejandro
 */

#include "CAPI_Definitions.h"
#include "UART.h"
#include "ColaCircular.h"
#include "string.h"

res_t UARTputc(UART * uart, cola_t c)
{
	ColaCircular * cola = &uart->colaTx;

	if(ColaLlena(cola))
		return CAPI_ERROR;

	ColaPoner(cola, c);

	if(!uart->TxEnCurso){
		UART_IntTransmision(uart);	// Pongo en marcha la transmision
	}

	return CAPI_EXITO;
}

cola_t UARTgetc(UART * uart)
{
	ColaCircular * cola = &uart->colaRx;

	if(ColaVacia(cola))
		return CAPI_ERROR;

	return ColaSacar(cola);
}

res_t UARTputs(UART * uart, const char * s)
{
	ColaCircular * cola = &uart->colaTx;

	if(ColaDisponible(cola) < strlen(s))
		return CAPI_ERROR;

	while(*s)
		ColaPoner(cola, *s++);

	if(!uart->TxEnCurso){
		UART_IntTransmision(uart);	// Pongo en marcha la transmision
	}

	return CAPI_EXITO;
}

res_t UARTgets(UART * uart, char * s)
{
	//FIXME: No se chequea que el buffer pueda contener el string que haya en la cola,
	//si la misma no contiene un \0 va a pisar el buffer que recibio
	ColaCircular * cola = &uart->colaRx;
	*s = ColaSacar(cola);
	while(*s){
		s++;
		*s = ColaSacar(cola);
	}
	return CAPI_EXITO;
}
