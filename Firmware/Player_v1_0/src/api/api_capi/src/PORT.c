/*
 * PORT.c
 *
 *  Created on: 22/06/2012
 *      Author: alejandro
 */

#include "PORT.h"
#include "PIN.h"

void PortDireccion	( PORT *, GPIO_Dir ); // Le da la direccion al GPIO
void PortSet		( PORT *, int ); // Pone los bits correspondiente del puerto a +VCC
void PortClear 		( PORT *, int ); // Pone los bits correspondiente del puerto a GND
bool PortFits		( PORT * );

void PortInit ( PORT * port, GPIO_Dir dir, GPIO_NivelActividad nivelAct, int pinNameInicial, int anchoPort )
{
	port->pinNameInicial		= pinNameInicial;
	port->direccion		= dir;
	port->nivelAct		= nivelAct;
	port->ioPort 		= PN_GET_PORT( pinNameInicial );
	port->bit 			= PN_GET_BIT( pinNameInicial );
	port->mascara_bits	= ( 1 << anchoPort ) - 1;


	if( PortFits(port) )
	{
		if( dir == SALIDA )
		{
			PortPoner( port, 0 ); // Lo pasivo antes de darle la direccion para evitar glitches
		}

		for ( ; anchoPort > 0 ; anchoPort-- )
		{
			PIN_Init( ++pinNameInicial );
		}

		PortDireccion( port, dir );
	}

}

void PortPoner ( PORT * port, int valor )
{
	if( port->direccion == SALIDA )
	{
		if( port->nivelAct )
		{
			PortSet		( port, valor );
			PortClear	( port, ~valor );
		}
		else
		{
			PortSet		( port, ~valor );
			PortClear	( port, valor );
		}
	}
}
