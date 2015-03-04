#ifndef	APP_CONFIG_H
#define	APP_CONFIG_H

//!	@note	Includes

//FreeRTOS Includes
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
//Standard Includes
#include "stdint.h"

//!	@note	Defines
/**
 * @def		APP_RECEPTION_BUFFER_SIZE
 * @brief	El tamaño del buffer de recepcion por el protocolo Ethernet
 */
#define		APP_RECEPTION_BUFFER_SIZE	1024
#define		APP_TRANSMISION_BUFFER_SIZE	1024

/**
 * @def		APP_PORT
 * @brief	Puerto de escucha de requests para el protocolo
 */
#define APP_PORT					15000
/**
 * @def		APP_TIMEOUT
 * @brief	Timeout en ns
 */
#define APP_TIMEOUT					5000

//#define		TEST_MODE
#ifdef		TEST_MODE
#define		TEST_ID					4
#endif

#define		APP_SERIAL_ID			((int)303456)

//!	@note	Variables globales
extern uint8_t appReceptionBuffer[APP_RECEPTION_BUFFER_SIZE];
extern uint8_t appTransmisionBuffer[APP_TRANSMISION_BUFFER_SIZE];

extern xSemaphoreHandle appTransmitSemph;
extern xSemaphoreHandle appReceiveSemph;

#endif
