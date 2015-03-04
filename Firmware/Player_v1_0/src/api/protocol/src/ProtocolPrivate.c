#include "ProtocolTypes.h"
#include "ProtocolResources.h"
#include "Protocol.h"

/**
 * @fn protocolComputeChecksum(uint8_t *buffer, uint32_t size)
 * @brief Computa el checksum
 * @param buffer
 * @param size
 * @param chk
 * @return 0 si esta OK, -1 si falla
 */
int32_t protocolCheckChecksum(uint8_t *buffer, uint16_t size, uint8_t chk)
{
	if(chk == protocolComputeChecksum(buffer, size))
		return (int32_t)0;
	return (int32_t)-1;
}

/**
 * @fn protocolComputeChecksum(uint8_t *buffer, uint32_t size)
 * @brief Computa el checksum
 * @param buffer
 * @param size
 * @return checksum
 */
uint8_t protocolComputeChecksum(uint8_t *buffer, uint16_t size)
{
	uint16_t i;
	uint8_t chk = 0;

	//Check del buffer
	if(buffer == NULL || size == 0)
	{
		return (uint8_t)0;
	}

	//Computo el checksum
	for(i = 0; i < size; i++)
	{
		chk ^= buffer[i];
	}

	return chk;
}

/**
 * @fn protocolSendACK(uint32_t ipAdd, uint16_t port)
 * @brief Envia un ACK.
 * @param ipAdd es la ip en un int de 4 bytes, sin signo. El octeto mas significativo en el byte mas significativo.
 * @param port es el puerto al que se desea escribir
 * @return la cantidad de bytes enviados o -1 si se produce un error
 */
int32_t protocolSendACK(uint32_t ipAdd, uint16_t port)
{
	TProtocolMetadata metadata;
	uint16_t i;

	//Cargo la metadata
	metadata.header = PROTOCOL_HEADER;
	metadata.command = (uint8_t) ACK;
	metadata.size = 0;
	metadata.chk = protocolComputeChecksum(NULL, 0);
	metadata.footer = PROTOCOL_FOOTER;

	//Realizo la copia al buffer de transmision
	for(i = 0; i < sizeof(TProtocolMetadata); i++)
		protocolConfig.transmitBuffer[i] = *(((uint8_t *)&metadata) + i);
	protocolConfig.transmitBufferIndex = sizeof(TProtocolMetadata);

	//Envio
	return protocolWriteData(ipAdd, port);
}

/**
 * @fn protocolSendNACK(uint32_t ipAdd, uint16_t port)
 * @brief Envia un NACK.
 * @param ipAdd es la ip en un int de 4 bytes, sin signo. El octeto mas significativo en el byte mas significativo.
 * @param port es el puerto al que se desea escribir
 * @return la cantidad de bytes enviados o -1 si se produce un error
 */
int32_t protocolSendNACK(uint32_t ipAdd, uint16_t port)
{
	TProtocolMetadata metadata;
	uint16_t i;

	//Cargo la metadata
	metadata.header = PROTOCOL_HEADER;
	metadata.command = (uint8_t) NACK;
	metadata.size = 0;
	metadata.chk = protocolComputeChecksum(NULL, 0);
	metadata.footer = PROTOCOL_FOOTER;

	//Realizo la copia al buffer de transmision
	for(i = 0; i < sizeof(TProtocolMetadata); i++)
		protocolConfig.transmitBuffer[i] = *(((uint8_t *)&metadata) + i);
	protocolConfig.transmitBufferIndex = sizeof(TProtocolMetadata);

	//Envio
	return protocolWriteData(ipAdd, port);
}

/**
 * @fn protocolWriteData(uint32_t ipAdd, uint16_t port)
 * @brief Abro el socket, lo bindeo a la direccion de destino, realizo la escritura e inicio la lectura de la respuesta.
 * @param ipAdd a la que quiero escribirle
 * @param port puerto al que deseo escribir
 * @return los bytes escritos, -1 si se produjo un error.
 */
int32_t protocolWriteData(uint32_t ipAdd, uint16_t port)
{
	struct sockaddr_in sLocalAddr, sDestAddr;
	int32_t bytesWritten;

	//Check si el socket estaba abierto
	if(protocolConfig.socket != -1)
	{
		lwip_close(protocolConfig.socket);
	}
	//Abro el socket
	protocolConfig.socket = lwip_socket(AF_INET, SOCK_DGRAM, 0);
	//Check de la apertura del socket
	if(protocolConfig.socket == -1)
	{
		return -1;
	}

	//Le cargo el timeout de lectura al socket
	if(lwip_setsockopt(	protocolConfig.socket,
			 	 	 	SOL_SOCKET,
			 	 	 	SO_RCVTIMEO,
			 	 	 	&(protocolConfig.timeoutTimeInUSec),
			 	 	 	sizeof(int)) == -1)
	{
		return -1;
	}

	//Limpio la memoria de las estructuras
	memset((char*)&sLocalAddr, 0, sizeof(sLocalAddr));
	memset((char*)&sDestAddr, 0, sizeof(sDestAddr));

	//Cargo el destinatario
	sDestAddr.sin_family = AF_INET;
	sDestAddr.sin_len = sizeof(sDestAddr);
	sDestAddr.sin_addr.s_addr = ipAdd;
	sDestAddr.sin_port = htons( ( ( uint16_t ) port ) );

	//Cargo la direccion de origen
	sLocalAddr.sin_family = AF_INET;
	sLocalAddr.sin_len = sizeof(sLocalAddr);
	sLocalAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	sLocalAddr.sin_port = htons( protocolConfig.port );

	//bind de la direccion de origen
	if(lwip_bind(protocolConfig.socket,
				 (struct sockaddr *)&sLocalAddr,
				 sizeof(sLocalAddr)) == -1)
	{
		return -1;
	}

	//Escribo al socket
	bytesWritten = lwip_sendto(	protocolConfig.socket,
								protocolConfig.transmitBuffer,
								protocolConfig.transmitBufferIndex,
								0,
								(struct sockaddr *)&sDestAddr,
								sizeof(sDestAddr));
	//Check de la escritura
	if(bytesWritten < 0)
	{
		return bytesWritten;
	}

	//Inicio la lectura
	xSemaphoreGive( protocolReceptionSemph );

	//Retorno los bytes escritos
	return bytesWritten;
}

/**
 * @fn protocolReceptionTask(void *arguments)
 * @brief Tarea de recepcion del protocolo
 * @param arguments	son los parametros de la tarea
 */
void protocolReceptionTask(void *arguments)
{
	int32_t bytesReceived;
	TProtocolMetadata *pProtocolMetadata;

	//Tomo el semaforo
	xSemaphoreTake( protocolReceptionSemph, portMAX_DELAY );

	while(1)
	{
		//Tomo el semaforo
		xSemaphoreTake( protocolReceptionSemph, portMAX_DELAY );

		//Leo por el socket
		bytesReceived = lwip_recvfrom(	protocolConfig.socket,
										protocolConfig.receptionBuffer,
										protocolConfig.receptionBufferSize,
										0,
										NULL,
										NULL);
		//Evaluo el resultado de la lectura
		if(bytesReceived < (int32_t)sizeof(TProtocolMetadata))
		{
			//Recibi menos bytes que la metadata, es un error!
			if(protocolConfig.errorCallback != NULL)
			{
				protocolConfig.errorCallback(BAD_PACKAGE_SIZE);
			}
			continue;
		}
		//Proceso el header
		pProtocolMetadata = (TProtocolMetadata *)protocolConfig.receptionBuffer;
		if(pProtocolMetadata->header != PROTOCOL_HEADER)
		{
			//Mal header, es un error!
			if(protocolConfig.errorCallback != NULL)
			{
				protocolConfig.errorCallback(BAD_PACKAGE_SIZE);
			}
			continue;
		}
		//Evaluo el comando
		if(pProtocolMetadata->command == 0)
		{
			//Error de comando
			if(protocolConfig.errorCallback != NULL)
			{
				protocolConfig.errorCallback(UNKNOWN_CMD);
			}
			continue;
		}
		//Evaluo si tengo datos y el checksum
		if(pProtocolMetadata->size == 0)
		{
			//Evaluo el checksum
			if(pProtocolMetadata->chk != protocolComputeChecksum(NULL, 0))
			{
				//Error de checksum
				if(protocolConfig.errorCallback != NULL)
				{
					protocolConfig.errorCallback(BAD_CHECKSUM);
				}
				continue;
			}
		}
		else
		{
			//Evaluo el checksum
			if(protocolConfig.receptionBuffer[4 + pProtocolMetadata->size] !=
			   protocolComputeChecksum(&(protocolConfig.receptionBuffer[4]), pProtocolMetadata->size))
			{
				//Error de checksum
				if(protocolConfig.errorCallback != NULL)
				{
					protocolConfig.errorCallback(BAD_CHECKSUM);
				}
				continue;
			}
		}
		//Cargo el tamaño en el indice de recepcion
		protocolConfig.receptionBufferIndex = pProtocolMetadata->size;

		//Emito el handler de fin de recepcion
		if(protocolConfig.receiveCallback != NULL)
		{
			protocolConfig.receiveCallback(	&(protocolConfig.receptionBuffer[4]),
											protocolConfig.receptionBufferIndex,
											pProtocolMetadata->command,
											0,
											protocolConfig.port);
		}
	}
}
