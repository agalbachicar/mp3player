#include "ProtocolTypes.h"
#include "ProtocolResources.h"
#include "Protocol.h"

//! @note	Funciones

/**
 * @fn protocolSetConfig(TProtocolConfig *config)
 * @brief Cargo la estructura de configuracion. Creo los recursos de sistema necesarios.
 * 		  Debe ser llamada solo una vez en la vida util del programa.
 * @param config puntero a una estructura de configuracion
 * @return -1 si el puntero es erroneo, 0 si se realizo el cargado existosamente
 */
int32_t protocolSetConfig(TProtocolConfig *config)
{
	//Check del puntero
	if(config == NULL)
	{
		return -1;
	}
	//Realizo la copia de las estructuras
	protocolConfig = *config;

	//Creo el semaforo de recepcion
	vSemaphoreCreateBinary( protocolReceptionSemph );
	if(protocolReceptionSemph == NULL)
	{
		return -1;
	}
	//Creo la tarea de recepcion
	if( xTaskCreate( protocolReceptionTask,
					 NULL,
					 PROTOCOL_RECEPTION_TASK_STACK_SIZE,
					 NULL,
					 PROTOCOL_RECEPTION_TASK_PRIORITY,
					 NULL ) != pdPASS)
	{
		return -1;
	}
	//Retorno satisfactoriamente
	return 0;
}

/**
 * @fn protocolSendData(uint8_t *buffer, uint16_t size, uint8_t cmd, uint32_t ipAdd, uint16_t port)
 * @brief Envia datos a una ip y puerto determinados
 * @param buffer de datos a enviar
 * @param size tamaño del buffer de datos a enviar.
 * @param cmd comando a enviar. No se debe enviar ni ACK ni NACK.
 * @param ipAdd es la ip en un int de 4 bytes, sin signo. El octeto mas significativo en el byte mas significativo.
 * @param port es el puerto al que se desea escribir
 * @return La cantidad de bytes escritos, -1 si hay error.
 */
int32_t protocolSendData(uint8_t *buffer, uint16_t size, uint8_t cmd, uint32_t ipAdd, uint16_t port)
{
	TProtocolMetadata metadata;
	uint16_t i, j;

	//Valido el buffer
	if((buffer == NULL && size != 0) || (buffer != NULL && size == 0))
	{
		return -1;
	}

	//Valido el comando
	if(cmd < (uint8_t)INITIAL_COMMAND_FOR_APP)
	{
		return -1;
	}

	//Valido que el buffer de transmision sea distinto de NULL
	if(protocolConfig.transmitBuffer == NULL)
	{
		return -1;
	}

	//Cargo la metadata
	metadata.header = PROTOCOL_HEADER;
	metadata.command = cmd;
	metadata.size = size;
	metadata.chk = protocolComputeChecksum(buffer, size);
	metadata.footer = PROTOCOL_FOOTER;

	//Realizo la copia al buffer de transmision
	for(i = 0; i < 4; i++)
		protocolConfig.transmitBuffer[i] = *(((uint8_t *)&metadata) + i);
	for(j = 0; j < size; i++, j++)
		protocolConfig.transmitBuffer[i] = buffer[j];
	for(j = 4; j < sizeof(TProtocolMetadata); i++, j++)
		protocolConfig.transmitBuffer[i] = *(((uint8_t *)&metadata) + j);
	//Cargo el tamaño de bytes a enviar en el buffer de transmision
	protocolConfig.transmitBufferIndex = sizeof(TProtocolMetadata) + size;

	//Realizo la transmision
	return protocolWriteData(ipAdd, port);
}



