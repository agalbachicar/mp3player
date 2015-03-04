/**
 * @author
 * @file
 * @brief
 * @date
 * @version
 * @todo
 * @bug
 */

/**
 * @note Includes
 */
#include "Player.h"
#include "PlayerPrivate.h"

/**
 * @note Variables globales
 */

/**
 * @var		playerEthernetServerAddress
 * @brief	Direccion del servidor (IP)
 */
static uint32_t playerEthernetServerAddress = 0x402A8C0;
/**
 * @var		playerEthernetServerPort
 * @brief	Puerto al que se debe escribir del servidor
 */
static uint32_t playerEthernetServerPort = 15000;

/**
 * @var		playerReceiveCallback
 * @brief	Puntero a funcion a ejecutar cuando se reciben datos por el protocolo
 */
void (*playerReceiveCallback)(uint8_t *buffer, uint32_t size) = NULL;

/**
 * @var		playerRequestBuffer
 * @brief	Es el buffer para las requests por ethernet
 */
char playerRequestBuffer[PLAYER_REQUEST_BUFFER_SIZE];

/**
 * @note Funciones publicas
 */
int32_t playerEthernetInit()
{
	uint32_t i = 0;
	//Evaluo el estado de la interfaz de red

	while(1)
	{
		//Evaluo el estado de la interfaz de red
		if(networkGetInterfaceStatus() == INTERFACE_DOWN)
		{
			i++;
			if(i == 30)
			{
				return EHERNET_DOWN;
			}
			//Genero una demora de 100 ticks
			vTaskDelay( 100 );
		}
		else
		{
			break;
		}
	}
	//Seteo los callback del protocolo
	playerReceiveCallback = NULL;

	//Tomo el semaforo para la transmision
	if(xSemaphoreTake( playerTransmitSemph, APP_TASKS_TRANSMIT_DELAY) != pdPASS)
	{
		return TRANSMIT_TIMEOUT;
	}

	//Envio un ACK al servidor para saber si esta activo
	protocolSendData(NULL, 0, AACK, playerEthernetServerAddress, playerEthernetServerPort);

	//Tomo el semaforo para la lectura
	xSemaphoreTake( playerReceiveSemph, portMAX_DELAY );

	//Libero el semaforo de la transmision
	xSemaphoreGive( playerTransmitSemph );

	return playerState;
}

int32_t playerEthernetGetNextSong(playerTSong *actualSong, playerTSong *nextSong)
{
	uint32_t bufferSize;
	uint8_t *buffer;

	//Copio el puntero a la cancion proxima
	playerAuxSong = nextSong;

	//Creo un mensaje para consultar la proxima cancion
	if(actualSong == NULL)
	{
		bufferSize = 0;
		buffer = NULL;
	}
	else
	{
		bufferSize = playerSerializeSong(actualSong, (char *)playerSongBuffer, PLAYER_SONGBUFFER_SIZE);
		buffer = playerSongBuffer;
	}
	//Seteo los callback del protocolo
	playerReceiveCallback = playerEthernetProtocolReceiveSongDataHandler;
	//Tomo el semaforo para la transmision
	if(xSemaphoreTake( playerTransmitSemph, APP_TASKS_TRANSMIT_DELAY) != pdPASS)
	{
		return TRANSMIT_TIMEOUT;
	}

	//Envio el mensaje para que me diga los datos de la siguiente cancion
	protocolSendData(buffer, bufferSize, GET_NEXT_SONG_DATA, playerEthernetServerAddress, playerEthernetServerPort);

	//Tomo el semaforo para la lectura
	xSemaphoreTake( playerReceiveSemph, portMAX_DELAY );

	//Libero el semaforo de la transmision
	xSemaphoreGive( playerTransmitSemph );

	return playerState;
}
int32_t playerEthernetGetPreviousSong(playerTSong *actualSong, playerTSong *nextSong)
{
	uint32_t bufferSize;

	//Copio el puntero a la cancion proxima
	playerAuxSong = nextSong;

	//Creo un mensaje para consultar la proxima cancion
	if(actualSong == NULL)
	{
		bufferSize = 0;
	}
	else
	{
		bufferSize = playerSerializeSong(actualSong, (char *)playerSongBuffer, PLAYER_SONGBUFFER_SIZE);
	}
	//Seteo los callback del protocolo
	playerReceiveCallback = playerEthernetProtocolReceiveSongDataHandler;
	//Tomo el semaforo para la transmision
	if(xSemaphoreTake( playerTransmitSemph, APP_TASKS_TRANSMIT_DELAY) != pdPASS)
	{
		return TRANSMIT_TIMEOUT;
	}

	//Envio el mensaje para que me diga los datos de la siguiente cancion
	protocolSendData(playerSongBuffer, bufferSize, GET_PREVIOUS_SONG_DATA, playerEthernetServerAddress, playerEthernetServerPort);

	//Tomo el semaforo para la lectura
	xSemaphoreTake( playerReceiveSemph, portMAX_DELAY );

	//Libero el semaforo de la transmision
	xSemaphoreGive( playerTransmitSemph );

	return playerState;
}
int32_t playerEthernetPlaySong(playerTSong *song)
{
	uint32_t bufferSize;

	//Creo el buffer para el pedazo de cancion
	bufferSize = playerSerializeSong(song, (char *)playerSongBuffer, PLAYER_SONGBUFFER_SIZE);

	//Seteo los callback del protocolo
	playerReceiveCallback = playerEthernetProtocolReceiveSongHandler;
	//Tomo el semaforo para la transmision
	if(xSemaphoreTake( playerTransmitSemph, APP_TASKS_TRANSMIT_DELAY) != pdPASS)
	{
		return TRANSMIT_TIMEOUT;
	}

	//Envio el mensaje para que me diga los datos de la siguiente cancion
	protocolSendData(playerSongBuffer, bufferSize, DOWNLOAD_SONG, playerEthernetServerAddress, playerEthernetServerPort);

	//Tomo el semaforo para la lectura
	xSemaphoreTake( playerReceiveSemph, portMAX_DELAY );

	//Libero el semaforo de la transmision
	xSemaphoreGive( playerTransmitSemph );

	//Evaluo el estado de error de la recepcion
	if(playerState != OK)
	{
		//Se produjo un error
		return RECEPTION_ERROR;
	}

	//Incremento la cantidad de bytes leidos
	song->index += playerReadBytes;
	//Cargo los datos del buffer de recepcion
	playerBuffer.size = playerReadBytes;
	playerBuffer.buffer = playerSongBuffer;

	//Envio el mensaje para que comience el envio de datos hacia el STA
	xSemaphoreGive( playerStreamingSemaphore );

	//Evaluo si lo que recibi es lo que pedi
	if(song->index >= song->size)
	{
		//Asumo que recibi el archivo completo
		playerState = END_OF_FILE;
	}

	return playerState;
}
int32_t playerEthernetPauseSong()
{
	return 0;
}
int32_t playerEthernetPrepaireSong(playerTSong *song)
{
	uint32_t bufferSize;

	//Creo el mensaje de la cancion
	bufferSize = playerSerializeSong(song, (char *)playerSongBuffer, PLAYER_SONGBUFFER_SIZE);
	//Seteo los callback del protocolo
	playerReceiveCallback = NULL;
	//Tomo el semaforo para la transmision
	if(xSemaphoreTake( playerTransmitSemph, APP_TASKS_TRANSMIT_DELAY) != pdPASS)
	{
		return TRANSMIT_TIMEOUT;
	}

	//Envio el mensaje para que me diga los datos de la siguiente cancion
	protocolSendData(playerSongBuffer, bufferSize, PREPAIRE_SONG, playerEthernetServerAddress, playerEthernetServerPort);

	//Tomo el semaforo para la lectura
	xSemaphoreTake( playerReceiveSemph, portMAX_DELAY );

	//Libero el semaforo de la transmision
	xSemaphoreGive( playerTransmitSemph );

	return playerState;
}
int32_t playerEthernetDisposeSong(playerTSong *song)
{
	uint32_t bufferSize;

	//Creo el mensaje de la cancion
	bufferSize = playerSerializeSong(song, (char *)playerSongBuffer, PLAYER_SONGBUFFER_SIZE);
	//Seteo los callback del protocolo
	playerReceiveCallback = NULL;
	//Tomo el semaforo para la transmision
	if(xSemaphoreTake( playerTransmitSemph, APP_TASKS_TRANSMIT_DELAY) != pdPASS)
	{
		return TRANSMIT_TIMEOUT;
	}

	//Envio el mensaje para que me diga los datos de la siguiente cancion
	protocolSendData(playerSongBuffer, bufferSize, DISPOSE_SONG, playerEthernetServerAddress, playerEthernetServerPort);

	//Tomo el semaforo para la lectura
	xSemaphoreTake( playerReceiveSemph, portMAX_DELAY );

	//Libero el semaforo de la transmision
	xSemaphoreGive( playerTransmitSemph );

	return playerState;
}

int32_t playerEthernetDeinit()
{
	return 0;
}

/**
 * @note Funciones Privadas
 */

void playerEthernetProtocolReceiveHandler (uint8_t *buffer, uint16_t size, uint8_t cmd, uint32_t ipAdd, uint16_t port)
{
	//Evaluo el comando
	switch(cmd)
	{
		case ACK:
			if(playerReceiveCallback != NULL)
			{
				playerReceiveCallback(buffer, size);
			}
			else
			{
				playerState = OK;
			}
			break;
		default:
			//Seteo el estado de error
			playerState = RECEPTION_ERROR;
			break;
	}
	//Libero el semaforo de recepcion
	xSemaphoreGive( playerReceiveSemph );
}

void playerEthernetErrorHandler(EProtocolError errorType)
{
	//Seteo el estado de error
	playerState = RECEPTION_ERROR;
	//Libero el semaforo para recibir
	xSemaphoreGive( playerReceiveSemph );
}

void playerEthernetSetServerData(uint8_t *serverIP, uint16_t port)
{
	playerEthernetServerAddress = (uint32_t)serverIP[0] |
			((uint32_t)serverIP[1] << 8)  |
			((uint32_t)serverIP[2] << 16)  |
			((uint32_t)serverIP[3] << 24);

	playerEthernetServerPort = port;
}

/**
 * @fn		playerEthernetProtocolReceiveSongDataHandler(uint8_t *buffer, uint32_t size)
 * @brief	Evaluo la estructura de la cancion que recibo y la cargo en el buffer de datos
 * @param	buffer	inicio de memoria a mover
 * @param	size	tamaño del buffer en bytes
 */
void playerEthernetProtocolReceiveSongDataHandler(uint8_t *buffer, uint16_t size)
{
	if(size == 0)
	{
		playerState = NO_SONG;
		return;
	}
	//Obtengo la cancion
	playerAuxSong->name = playerGetSongNameBuffer();
	//Cargo la informacion del servidor a la estructura de la cancion
	playerDeserializeSong(playerAuxSong, (char *)buffer, size);

	playerState = OK;
}


/**
 * @fn		playerEthernetProtocolReceiveSongHandler(uint8_t *buffer, uint32_t size)
 * @brief	Muevo la memoria del buffer de recepcion al de la cancion
 * @param	buffer	inicio de memoria a mover
 * @param	size	tamaño del buffer en bytes
 */
void playerEthernetProtocolReceiveSongHandler(uint8_t *buffer, uint16_t size)
{
	//Cargo la cantidad de bytes leidos
	playerReadBytes = size;
	//Copio la memoria
	memcpy((void *)playerSongBuffer, (void *)buffer, size);

	playerState = OK;
}



