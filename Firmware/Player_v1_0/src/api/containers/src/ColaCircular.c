/*
 * ColaCircular.c
 *
 *  Created on: 18/03/2012
 *      Author: Alejandro
 */

#include "CAPI_Definitions.h"
#include "ColaCircular.h"

typedef enum {
	P_IN,
	P_OUT
} punteroCola_t;

static void ColaReponerPuntero (ColaCircular * cola, cola_t **ptr);

void ColaCircular_Init (ColaCircular * cola, cola_t * vector, int capacidad)
{
	cola->pOrigen	= vector;
	cola->pIn 		= vector;
	cola->pOut 		= vector;
	cola->cantidadEnCola = 0;
	cola->capacidad = capacidad;
}

res_t ColaPoner (ColaCircular * cola, cola_t dato){

	if(ColaLlena(cola))
	{
		return CAPI_ERROR;
	}
	else
	{
		*(cola->pIn) = dato;
		cola->cantidadEnCola++;
		cola->pIn++;
		ColaReponerPuntero(cola, &cola->pIn);
		return CAPI_EXITO;
	}
}

cola_t ColaSacar (ColaCircular * cola){
//TODO: Rediseniar para que salga por puntero el valor, asi puedo devolver res_t
	cola_t dato;
	if(ColaVacia(cola))
	{
		return CAPI_ERROR;
	}
	else
	{
		dato = *(cola->pOut);
		cola->cantidadEnCola--;
		cola->pOut++;
		ColaReponerPuntero(cola, &cola->pOut);
		return dato;
	}
}

int ColaCantidad(ColaCircular * cola){
	return cola->cantidadEnCola;
}

int ColaDisponible(ColaCircular * cola){
	return cola->capacidad - cola->cantidadEnCola;
}

bool ColaLlena (ColaCircular * cola){
	return cola->cantidadEnCola >= cola->capacidad;
}

bool ColaVacia (ColaCircular * cola){
	return !cola->cantidadEnCola;
}

static void ColaReponerPuntero (ColaCircular * cola, cola_t **ptr){
	if(*ptr >= cola->pOrigen + cola->capacidad) {
		*ptr = cola->pOrigen;
	}
}
