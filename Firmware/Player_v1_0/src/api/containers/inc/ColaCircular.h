/*
 * ColaCircular.h
 *
 *  Created on: 18/03/2012
 *      Author: Alejandro
 */

#ifndef COLACIRCULAR_H_
#define COLACIRCULAR_H_

#include "CAPI_Definitions.h"

typedef struct ColaCircularStruct {
	cola_t *pOrigen;
	cola_t *pIn;
	cola_t *pOut;
	int cantidadEnCola;
	int capacidad;
}ColaCircular;

void 	ColaCircular_Init 	(ColaCircular * cola, cola_t * vector, int capacidad);
res_t 	ColaPoner 			(ColaCircular * cola, cola_t dato);
cola_t	ColaSacar 			(ColaCircular * cola);
int 	ColaCantidad		(ColaCircular * cola);
int		ColaDisponible		(ColaCircular * cola);
bool 	ColaVacia 			(ColaCircular * cola);
bool 	ColaLlena 			(ColaCircular * cola);

#endif /* COLACIRCULAR_H_ */
