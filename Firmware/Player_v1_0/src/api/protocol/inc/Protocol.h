/*
 * Protocol.h
 *
 *  Created on: Jul 18, 2014
 *      Author: agustin
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

//! @note	Includes

//lwIP core includes
#include "lwip/opt.h"
#include "lwip/tcpip.h"
//lwIP netif includes
#include "netif/etharp.h"
#include "lwip/opt.h"
#include "lwip/sockets.h"

//FreeRTOS Includes
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

//Standard includes
#include "stdlib.h"
#include "string.h"
#include "stdint.h"

//Includes de nuestra API
#include "ProtocolTypes.h"


//! @note	Defines
#ifndef NULL
	#define	NULL	0
#endif

/**
 * @def	PROTOCOL_RECEPTION_TASK_STACK_SIZE
 * @brief Stack de la tarea de recepcion
 */
#define	PROTOCOL_RECEPTION_TASK_STACK_SIZE		1000
/**
 * @def	PROTOCOL_RECEPTION_TASK_PRIORITY
 * @brief Prioridad de la tarea de recepcion
 */
#define	PROTOCOL_RECEPTION_TASK_PRIORITY		(configMAX_PRIORITIES - 1)

//! @note	Funciones privadas
int32_t protocolCheckChecksum(uint8_t *buffer, uint16_t size, uint8_t chk);
uint8_t protocolComputeChecksum(uint8_t *buffer, uint16_t size);
int32_t protocolSendACK(uint32_t ipAdd, uint16_t port);
int32_t protocolSendNACK(uint32_t ipAdd, uint16_t port);
int32_t protocolWriteData(uint32_t ipAdd, uint16_t port);
void protocolReceptionTask(void *arguments);

//! @note	Funciones publicas
int32_t protocolSetConfig(TProtocolConfig *config);
inline void protocolSetReceptionHandler(void (*receiveCallback)(uint8_t *buffer, uint16_t size, uint8_t cmd, uint32_t ipAdd, uint16_t port));
inline void protocolSetErrorHandler(void (*errorCallback)(EProtocolError errorType));
int32_t protocolSendData(uint8_t *buffer, uint16_t size, uint8_t cmd, uint32_t ipAdd, uint16_t port);

#endif /* PROTOCOL_H_ */
