/*
 * lpc_17xx_PORT.c
 *
 *  Created on: 22/06/2012
 *      Author: Alejandro Celery y Hernan Perez
 */

#include "PIN.h"
#include "PORT.h"
#include "lpc17xx_gpio.h"

#define ANCHO_IO_PORT 32

void PortDireccion ( PORT * port, GPIO_Dir dir )
{
	// Le da la direccion a los GPIO
	GPIO_SetDir( port->ioPort, ( port->mascara_bits << port->bit ), dir ? 1 : 0 );
}

void PortSet ( PORT * port, int valor )
{
	// Pone los pines a +VCC
	valor &= port->mascara_bits;
	GPIO_SetValue( port->ioPort, ( valor << port->bit ) );
}

void PortClear ( PORT * port, int valor )
{
	// Pone los pines a GND
	valor &= port->mascara_bits;
	GPIO_ClearValue( port->ioPort, ( valor << port->bit ) );
}

uint32_t PortLeer ( PORT * port )
{
	uint32_t valorPort = ( GPIO_ReadValue( port->ioPort ) >> port->bit ) & port->mascara_bits;
	return port->nivelAct ? valorPort : ~valorPort;
}

bool PortFits ( PORT * port )
{
	return ( PN_GET_BIT( port->pinNameInicial ) + port->anchoPort ) < ANCHO_IO_PORT;
}
