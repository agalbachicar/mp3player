/*
 * UART.h
 *
 *  Created on: 17/03/2012
 *      Author: alejandro
 */

#ifndef UART_H_
#define UART_H_

#include "CAPI_Definitions.h"
#include "PERIPH.h"
#include "ColaCircular.h"

typedef struct _UART {
	int periphName;
	ColaCircular colaTx;
	ColaCircular colaRx;
	bool TxEnCurso;
} UART;

void	UARTInit			(UART *, int per, long baudRate, int dataBits, char parity, int stopBits);
void 		UARTDeInit		(UART * uart);

//Funciones que acceden a las colas de Tx y Rx
res_t	UARTputs			(UART *, const char * s);
res_t 	UARTgets			(UART *, char * s);
res_t 	UARTputc			(UART * uart, char cola_t);
cola_t 	UARTgetc			(UART * uart);

//Funciones que acceden directamente al hardware
res_t 	UARTputchar			(UART *, char c);
char  	UARTgetchar 		(UART *);
bool	UARTbusy			(UART *);
void 	UART_IntTransmision	(UART *);
void 	UART_IntRecepcion	(UART *);

#if 0 // Falta implementar
int 	UARTwrite			(UART *, const char * buf, int largo);
int 	UARTread			(UART *, char * buf, int largo);
#endif
#endif /* UART_H_ */
