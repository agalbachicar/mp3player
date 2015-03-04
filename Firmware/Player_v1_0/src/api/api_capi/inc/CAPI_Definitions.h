/*
 * CAPI_Definitions.h
 *
 *  Created on: 15/02/2012
 *      Author: alejandro
 */

#ifndef CAPI_DEFINITIONS_H_
#define CAPI_DEFINITIONS_H_

typedef unsigned char bool;
typedef char cola_t;	// Hago las colas de a bytes

typedef enum {
	CAPI_ERROR = 0,
	CAPI_EXITO
} res_t;

typedef enum _direction {
	ENTRADA = 0,
	SALIDA = 1,
}GPIO_Dir;

typedef enum _nivelActividad {
	BAJO = 0,
	ALTO = 1,
}GPIO_NivelActividad;

enum {
	false = 0,
	true = !false
};

#define NIBBLE_ALTO(x) 		( (x & 0x000000F0) >> 4)
#define NIBBLE_BAJO(x) 		( (x & 0x0000000F) >> 0)
#define MAKE_WORD_HL(h,l)	( (h<<8) | l )

#define BIT_POS(x,bit)	(x & (1UL<<bit))

#endif /* CAPI_DEFINITIONS_H_ */
