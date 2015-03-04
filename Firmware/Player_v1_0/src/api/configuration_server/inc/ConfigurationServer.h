/**
 * @author	Agustin Alba Chicar
 * @file	ConfigurationServer.h
 * @brief	Archivo header de la API de configuracion de la interfaz de red y demas propiedades del dispositivo
 * @date	Febrero, 2015
 * @version	1.0
 */
#ifndef CONFIGURATIONSERVER_H_
#define CONFIGURATIONSERVER_H_

//!	@note Includes

//FreeRTOS includes
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "Network.h"
#include "LCD_API.h"
#include "debFlashMemory.h"

//lwIP core includes
#include "lwip/opt.h"
#include "lwip/tcpip.h"
#include "netif/etharp.h"
#include "lwip/opt.h"
#include "lwip/sockets.h"

//Standard Includes
#include "stdlib.h"
#include "string.h"
#include "stdint.h"

//!	@note Defines

/**
 * @def		CONFIGURATION_SERVER_PORT
 * @brief	Es la IP de la IP de configuracion del servidor
 */
#define		CONFIGURATION_SERVER_PORT					15001
/**
 * @def		CONFIGURATION_SERVER_STACKSIZE
 * @brief	Es el tamaño en bytes del stack de la tarea de configuracion
 */
#define		CONFIGURATION_SERVER_STACKSIZE				1000
/**
 * @def		CONFIGURATION_SERVER_PRIORITY
 * @brief	Es la prioridad de la tarea de configuracion
 */
#define		CONFIGURATION_SERVER_PRIORITY				2

#define		CONFIGURATIONSERVER_FLASH_SECTOR			23

//!	@note	Tipos de dato
typedef struct ConfigurationSeverType
{
	NetworkTInterface interface;
	uint16_t port;
	//uint32_t id;
	uint8_t serverIP[4];
}ConfigurationSeverT;

//!	@note Prototipos de funciones privadas

void printRaisingUpInterface();
void printWaitingForIncommingConfiguration();
void printAckConfiguration();
void printNAckConfiguration();

//!	@note Prototipos de funciones publicas

void ConfigurationServerTask(void *arguments);
void ConfigurationServerSaveConfiguration(ConfigurationSeverT *configuration);
int32_t ConfigurationServerLoadConfiguration(ConfigurationSeverT *configuration);

//!	@note Variables externas

#endif /* CONFIGURATIONSERVER_H_ */

