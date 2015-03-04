#ifndef PROTOCOL_TYPES
#define PROTOCOL_TYPES

//! @note	Includes

//FreeRTOS Includes
#include "FreeRTOS.h"
#include "semphr.h"
//Standard Includes
#include "stdint.h"

//! @note	Defines

#ifndef	NULL
	#define NULL	0
#endif

/**
 * @def	PROTOCOL_HEADER
 * @brief Header de los paquetes
 */
#define PROTOCOL_HEADER		((uint8_t)0xAA)
/**
 * @def	PROTOCOL_FOOTER
 * @brief Footer de los paquetes
 */
#define PROTOCOL_FOOTER		((uint8_t)0xEE)


//! @note	Tipos de dato
/**
 * @enum EProtocolError
 * @brief Tipos de errores
 */
typedef enum EProtocolError
{
    BAD_HEADER_FOOTER = 1,
    UNKNOWN_CMD = 2,
    BAD_PACKAGE_SIZE = 3,
    BAD_CHECKSUM = 4,
    NETWORK = 5,
    UNKNOWN = 6,
}EProtocolError;

/**
 * @enum EProtocolBaseCommands
 * @brief Comandos basicos del protocolo
 */
typedef enum EProtocolBaseCommand
{
	ACK = 1,
	NACK = 2,
    GET_NETWORK_CONFIG = 3,
    REGISTER = 4,
    INITIAL_COMMAND_FOR_APP,
}EProtocolBaseCommand;

/**
 * @struct TProtocolMetadata
 * @brief Metadata de los paquetes. Los datos van entre el size y el chk.
 */
typedef struct TProtocolMetadata
{
	uint8_t header;
	uint8_t command;
	uint16_t size;
	uint8_t chk;
	uint8_t footer;
}TProtocolMetadata;

/**
 * @struct TProtocolConfig
 * @brief Contiene toda la configuracion del protocolo
 */
typedef struct TProtocolConfig
{
	//Callback que se ejecuta cuando llegan datos validos
	void (*receiveCallback)(uint8_t *buffer, uint16_t size, uint8_t cmd, uint32_t ipAdd, uint16_t port);
	//Callback que se ejecuta cuando se produce un error
	void (*errorCallback)(EProtocolError errorType);

	//Buffer de transmision
	uint8_t *transmitBuffer;
	uint16_t transmitBufferSize;
	uint16_t transmitBufferIndex;
	//Buffer de recepcion
	uint8_t *receptionBuffer;
	uint16_t receptionBufferSize;
	uint16_t receptionBufferIndex;
	//Timeout de lectura tras un envio
	uint32_t timeoutTimeInUSec;

	//Socket
	int32_t socket;
	//Puerto de escucha del socket
	int16_t port;
}TProtocolConfig;

#endif
