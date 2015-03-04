/**
 * @author	Agustin Alba Chicar
 * @file	Player.h
 * @brief	Archivo cabecera para el envio de datos al player
 * @date	Febrero/2015
 * @version	1
 * @todo
 * @bug
 */
#ifndef 	PLAYER_H
#define		PLAYER_H

/**
 * @note        Includes
 */

#include <stdint.h>

//FreeRTOS includes
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

//Standard includes
#include "stdint.h"
#include "stdlib.h"

//Includes de nuestra API
#include "appConfig.h"
#include "Network.h"
#include "Protocol.h"
#include "MusicProtocol.h"
#include "SD.h"
#include "sta013_api.h"
#include "LCD_API.h"

/**
 * @note        Defines
 */

#ifndef ERR
        #define ERR     ((int32_t)-1)
#endif

/**
 * @def		PLAYER_SONGBUFFER_SIZE
 * @brief	Tamaño del buffer de las canciones
 */
#define		PLAYER_SONGBUFFER_SIZE			    380

#define		PLAYER_ETHERNET_BUFFER				512

#define		PLAYER_SONGS						4

#define		PLAYER_SONG_NAME_BUFFER				256
/**
 * @def		APP_TASKS_TRANSMIT_DELAY
 * @brief	Numero de ticks a bloquear en la toma del semaforo para la transmision
 */
#define		APP_TASKS_TRANSMIT_DELAY		5
/**
 * @def		PLAYER_STREAM_STACKSIZE
 * @brief	Es el tamaño del stack de la tarea de streaming
 */
#define		PLAYER_STREAM_STACKSIZE			200
/**
 * @def		PLAYER_STREAM_PRIORITY
 * @brief	Es la prioridad de la tarea de streaming
 */
#define		PLAYER_STREAM_PRIORITY			4

/**
 * @note        Tipos de datos, estructuras, typdefs
 */

typedef enum playerEnumType
{
	SD_PLAYER = 0,
	ETHERNET_PLAYER = 1
}playerEType;

typedef struct playerTypeFunction
{
	playerEType type;
	int32_t (*init)();
	int32_t (*getNextSong)();
	int32_t (*getPreviousSong)();
	int32_t (*getNextPlayingSong)();
	int32_t (*getPreviousPlayingSong)();
	int32_t (*prepaireSong)();
	int32_t (*playSong)();
	int32_t (*pauseSong)();
	int32_t (*disposeSong)();
	int32_t (*deinit)();
}playerTFunction;

/**
 * @enum	playerESongState
 * @brief	Son los estados posibles por los que puede pasar una cancion
 */
typedef enum
{
	NO_MORE_SONGS = 2,	    /**< No hay mas canciones. */
	END_OF_FILE = 1,		/**< Fin del archivo. */
	OK = 0,					/**< Estado de lectura ok */
	DISK_INIT_FAIL = -1,	/**< Error en la inicializacion del disco */
	FAT_MOUNT_FAIL = -2,	/**< Error de montaje del sistema de archivos. */
	FILE_OPEN_FAIL = -3,	/**< Error de apertura del archivo. */
	FILE_READ_FAIL = -4,	/**< Error de lectura del archivo. */
	BAD_SOURCE = -5,		/**< Error de seleccion de la fuente del archivo */
	EHERNET_DOWN = -6,		/**< La interfaz de red esta caida */
	TRANSMIT_TIMEOUT = -7,	/**< Se produjo un timeout para poder solicitar el recurso de escritura */
	RECEPTION_TIMEOUT = -8,	/**< Se produjo un timeout en la lectura a traves de Ethernet */
	DMA_ERROR = -9,			/**< Se produjo un error en la copia por DMA de lo recibido por Ethernet */
	RECEPTION_ERROR = -10,	/**< Siempre que lo que se escuche sea distinto de un ACK */
	NO_SONG = -11,			/**< No hay cancion disponible */
	DIR_CHANGED = -12, 		/**< Cambio de diectorio en SD*/
}playerESongState;


//FIXME: Deberia estar en el Private.h
typedef uint32_t log_pos;

/**
 * @struct	playerTSong
 * @brief	Es la estructura que mantiene el estado de lectura de la cancion.
 */
typedef struct
{
	uint32_t id;
	uint32_t size;
	uint32_t index;
	playerEType source;
	FIL *file;
	char *name;
	//TODO: Agregue
	sd_ext type;
	log_pos pos;

}playerTSong;

typedef struct
{
	uint8_t *buffer;
	uint16_t size;
}playerTBuffer;

/**
 * @note        Prototipos de funciones publicas
 */
void playerLoadPointers(playerEType type, playerTFunction *pointers);
void playerStreamTask(void *arguments);

void playerEthernetProtocolReceiveHandler (uint8_t *buffer, uint16_t size, uint8_t cmd, uint32_t ipAdd, uint16_t port);
void playerEthernetErrorHandler(EProtocolError errorType);

void playerEthernetSetServerData(uint8_t *serverIP, uint16_t port);
/**
 * @note        Prototipos de funciones privadas
 */

uint32_t playerSerializeSong(playerTSong *song, char *buffer, uint32_t size);
void playerDeserializeSong(playerTSong *song, char *buffer, uint32_t size);
char * playerGetSongNameBuffer();

/**
 * @note        Variables publicas
 */
extern xSemaphoreHandle playerReceiveSemph;
extern xSemaphoreHandle playerTransmitSemph;
extern xSemaphoreHandle playerStreamingSemaphore;

extern uint8_t playerEthernetTransmitionBuffer[PLAYER_ETHERNET_BUFFER];
extern uint8_t playerEthernetReceptionBuffer[PLAYER_ETHERNET_BUFFER];


#endif	/* PLAYER_H */
