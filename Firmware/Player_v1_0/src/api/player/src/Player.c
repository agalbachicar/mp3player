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
 * @var		playerReceiveSemph
 * @brief	El semaforo de recepcion para las transmisiones por Ethernet
 */
xSemaphoreHandle playerReceiveSemph = NULL;
/**
 * @var		playerTransmitSemph
 * @brief	El semaforo de transmision para las transmisiones por Ethernet
 */
xSemaphoreHandle playerTransmitSemph = NULL;
/**
 * @var		playerStreamingSemaphore
 * @brief	El semaforo para sincronizar el envio de datos al STA013
 */
xSemaphoreHandle playerStreamingSemaphore = NULL;
/**
 * @var		playerState
 * @brief	Estado del player a medida que ejecuta las distintas acciones, toma un valor del tipo playerESongState
 */
int32_t playerState;
/**
 * @var		appSongBuffer
 * @brief	Buffer que se utiliza para el streaming de una cancion y para el envio de datos al STA013
 */
uint8_t playerSongBuffer[PLAYER_SONGBUFFER_SIZE];
/**
 * @var		playerBuffer
 * @brief	Puntero y size del buffer a enviar el
 */
volatile playerTBuffer playerBuffer;
/**
 * @var		playerAuxSong
 * @brief	Es un puntero a una estructura del tipo song para que se cargue al recibir por Etherent en caso de ser necesario.
 */
playerTSong *playerAuxSong = NULL;
/**
 * @var		playerReadBytes
 * @brief	Cantidad de bytes leidos por Ethernet o por la SD
 */
volatile uint16_t playerReadBytes = 0;

uint8_t playerEthernetTransmitionBuffer[PLAYER_ETHERNET_BUFFER];

uint8_t playerEthernetReceptionBuffer[PLAYER_ETHERNET_BUFFER];

char playerSongNameBuffer[PLAYER_SONGS][PLAYER_SONG_NAME_BUFFER];

/**
 * @note Funciones publicas
 */
/**
 * @fn		playerLoadPointers(playerEType type, playerTFunction *pointers)
 * @brief	Cargo los punteros segun el tipo de player solicitado
 * @param	type		es el tipo de player
 * @param	pointers 	es el puntero a una estructura con los punteros a funcion de las acciones
 */
void playerLoadPointers(playerEType type, playerTFunction *pointers)
{
	pointers->type = type;

	if(type == SD_PLAYER)
	{
		pointers->init = playerSDInit;
		pointers->getNextSong = playerSDGetNextSong;
		pointers->getNextPlayingSong = playerSDGetNextPlaying;
		pointers->getPreviousSong = playerSDGetPreviousSong;
		pointers->getPreviousPlayingSong = playerSDGetPreviousPlaying;
		pointers->playSong = playerSDPlaySong;
		pointers->pauseSong = playerSDPauseSong;
		pointers->deinit = playerSDDeinit;
		pointers->prepaireSong = playerSDPrepaireSong;
		pointers->disposeSong = playerSDDisposeSong;
	}
	else if(type == ETHERNET_PLAYER)
	{
		pointers->init = playerEthernetInit;
		pointers->getNextSong = playerEthernetGetNextSong;
		pointers->getNextPlayingSong = playerEthernetGetNextSong;
		pointers->getPreviousSong = playerEthernetGetPreviousSong;
		pointers->getPreviousPlayingSong = playerEthernetGetPreviousSong;
		pointers->playSong = playerEthernetPlaySong;
		pointers->pauseSong = playerEthernetPauseSong;
		pointers->deinit = playerEthernetDeinit;
		pointers->prepaireSong = playerEthernetPrepaireSong;
		pointers->disposeSong = playerEthernetDisposeSong;
	}
}

/**
 * @fn		playerStreamTask(void *arguments)
 * @brief	Tarea de streaming da datos al STA013
 * @param	arguments	no se utilizan
 */
void playerStreamTask(void *arguments)
{
	int32_t i;

	uint8_t *buffer = NULL;
	uint32_t bufferSize = 0;

	//Tomo el semaforo de envio
	xSemaphoreTake( playerStreamingSemaphore, 0 );

	while(1)
	{
		//Tomo el semaforo
		if(xSemaphoreTake( playerStreamingSemaphore, portMAX_DELAY ) == pdTRUE)
		{
			//Copio los datos del buffer
			buffer = playerBuffer.buffer;
			bufferSize = playerBuffer.size;

			//Itero en el buffer de datos para enviarle al sta los bytes
			for(i = 0; i < bufferSize; i++)
			{
				while(1)
				{
					//Me quedo intentando de enviarle los datos al sta, en caso que
					//retorne con OP_BUSY vuelvo a intentar de enviarle
					if(sta013_Stream (&(buffer[i]), 1) == OP_END)
					{
					break;
					}
				}
			}
		}
	}
}

/**
 * @note Funciones Privadas
 */

/**
 * @fn		playerSerializeSong(playerTSong *song, char *buffer, uint32_t size)
 * @brief	Serializo una cancion como el servidor la necesita en un buffer
 * @param	song		es el puntero a una estructura playerTSong
 * @param	buffer		es el puntero al buffer donde se guarda el string de la cancion
 * @param	size		es el tamaño del buffer
 * @return	el tamaño final del buffer
 */
uint32_t playerSerializeSong(playerTSong *song, char *buffer, uint32_t size)
{
	char uintToStrBuffer[15];
	int32_t n;

	//Inicializo la variable del tamaño en 0
	*buffer = '\0';

	//Comienzo a copiar el nombre del mensaje
	//Token de id de cancion
	strcat(buffer, APP_TASK_ID);
	//Id de la cancion
	n = sprintf(uintToStrBuffer, "%u", song->id);
	strncat(buffer, uintToStrBuffer, n);
	//Separador
	strcat(buffer, APP_TASK_SEPARATOR);
	//Token del indice
	strcat(buffer, APP_TASK_INDEX);
	//El indice
	n = sprintf(uintToStrBuffer, "%u", song->index);
	strncat(buffer, uintToStrBuffer, n);
	//Separador
	strcat(buffer, APP_TASK_SEPARATOR);
	//Token de bytes a leer
	strcat(buffer, APP_TASK_BYTES_TO_READ);
	//Los bytes a leer
	n = sprintf(uintToStrBuffer, "%u", size);
	strncat(buffer, uintToStrBuffer, n);

	return strlen(buffer);
}

void playerDeserializeSong(playerTSong *song, char *buffer, uint32_t size)
{
	char *pLabel;
	int32_t aux;

	//Obtengo el string del "id="
	pLabel = strstr(buffer, APP_TASK_ID);
	if(pLabel != NULL)
	{
		//Encontro el string, lo convierto a int
		song->id = atoi(pLabel + strlen(APP_TASK_ID));
	}

	//Obtengo el string del "bytesToRead="
	pLabel = strstr(buffer, APP_TASK_BYTES_TO_READ);
	if(pLabel != NULL)
	{
		//Encontro el string, lo convierto a int
		song->size = atoi(pLabel + strlen(APP_TASK_BYTES_TO_READ));
	}

	//Obtengo el string de los temas
	pLabel = strstr(buffer, APP_TASK_NAME);
	if(pLabel != NULL)
	{
		aux = strlen(APP_TASK_NAME);
		strncpy(song->name, pLabel + aux, buffer + size - pLabel - aux);
		song->name[buffer + size - pLabel - aux] = '\0';
	}

	song->index = 0;
}

char * playerGetSongNameBuffer()
{
	static int32_t index = -1;

	index++;
	if(index == PLAYER_SONGS)
	{
		index = 0;
	}
	return &(playerSongNameBuffer[index][0]);
}


