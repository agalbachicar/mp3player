/**
 * @author
 * @file
 * @brief
 * @date
 * @version
 * @todo
 * @bug
 */
#ifndef PLAYERPRIVATE_H_
	#define PLAYERPRIVATE_H_

/**
 * @note        Includes
 */

#include <stdint.h>

#include "sta013_driver.h"

/**
 * @note        Defines
 */
#ifndef ERR
        #define ERR     ((int32_t)-1)
#endif

/**
 * @def		APP_TASK_ID
 * @brief	Es el id de la cancion a requerir
 */
#define		APP_TASK_ID						("id=")
/**
 * @def		APP_TASK_NAME
 * @brief	Es el token del nombre del archivo a seleccionar
 */
#define		APP_TASK_NAME				("name=")
/**
 * @def		APP_TASK_INDEX
 * @brief	Es el token del indice desde donde leer
 */
#define		APP_TASK_INDEX					("index=")
/**
 * @def		APP_TASK_BYTES_TO_READ
 * @brief	Es el token de la cantidad de bytes a leer del archivo
 */
#define		APP_TASK_BYTES_TO_READ			("bytesToRead=")
/**
 * @def		APP_TASK_SEPARATOR
 * @brief	Es el separador entre variables entre los datos del mensaje
 */
#define		APP_TASK_SEPARATOR				("&&")
/**
 * @def		APP_REQUEST_BUFFER_SIZE
 * @brief	El tamaño del buffer para las requests por ethernet
 */
#define		PLAYER_REQUEST_BUFFER_SIZE		512


/*
 *@def		LOG_SECTOR_SIZE
 *@brief	Cantidad de bytes que ocupa cada item en el .log
 */
#define 	LOG_SECTOR_SIZE		300 //300 bytes


/*
 * @def		CANT_LOG_ITEMS
 * @brief	Cantidad de elemento a guardar en el .log
 */
#define 	CANT_LOG_ITEMS		100 //(100*LOG_SECTOR_SIZE) bytes

/*
 * @def		_MAX_LOG_EXTRA
 * @brief	Cantidad de caracteres de para informacion extra en LogFile.log
 */
#define		_MAX_LOG_EXTRA		10


/**
 * @note        Tipos de datos, estructuras, typdefs
 */

/*
 * *****************
 * *******SD********
 * *****************
 */

typedef enum{
	ENABLED,
	NOT_ENABLED,
}back_op;

/*
 * @struct	log_type
 * @brief	Estructura para mantener putero al archivo .log
 */
typedef struct{
	log_pos actual_pos;
	log_pos end_pos;
	log_pos org_pos;
	FIL		acces; //Archivo FIL del Log_File
	uint8_t new_dir; //Indica con 0x01 si se ha movido a un directorio distinto
}log_type;

/**
 * @note        Prototipos de funciones publicas
 */
int32_t playerEthernetInit();
int32_t playerEthernetGetNextSong(playerTSong *actualSong, playerTSong *nextSong);
int32_t playerEthernetGetPreviousSong(playerTSong *actualSong, playerTSong *nextSong);
int32_t playerEthernetPlaySong(playerTSong *song);
int32_t playerEthernetPauseSong();
int32_t playerEthernetPrepaireSong(playerTSong *song);
int32_t playerEthernetDisposeSong(playerTSong *song);
int32_t playerEthernetDeinit();

int32_t playerSDInit();
int32_t playerSDGetNextSong(playerTSong *actualSong, playerTSong *nextSong);
int32_t playerSDGetNextPlaying(playerTSong *actualSong, playerTSong *nextSong);
int32_t playerSDGetPreviousSong(playerTSong *actualSong, playerTSong *nextSong);
int32_t playerSDGetPreviousPlaying(playerTSong *actualSong, playerTSong *nextSong);
int32_t playerSDPlaySong(playerTSong *song);
int32_t playerSDPauseSong();
int32_t playerSDPrepaireSong(playerTSong *song);
int32_t playerSDDisposeSong(playerTSong *song);
int32_t playerSDDeinit();


/**
 * @note        Prototipos de funciones privadas
 */
/*****************
 *****ETHERNET****
 *****************
 */
void playerEthernetProtocolReceiveSongDataHandler(uint8_t *buffer, uint16_t size);
void playerEthernetProtocolReceiveSongHandler(uint8_t *buffer, uint16_t size);

/*****************
 ********SD*******
 *****************
 */
sd_ext 		item_filter(FILINFO info, playerTSong *item);
dir_stat	dir_peek(DIR *carpeta, playerTSong *item);
uint8_t		log_puts(playerTSong *item);
uint8_t 	log_gets( log_pos *pos, playerTSong *item);
void 		log_filter( playerTSong *item);
void		ret_void(void);

/**
 * @note        Variables publicas
 */
extern volatile playerTBuffer playerBuffer;
extern int32_t playerState;
extern playerTSong *playerAuxSong;
extern uint8_t playerSongBuffer[PLAYER_SONGBUFFER_SIZE];
extern volatile uint16_t playerReadBytes;

#endif /* PLAYERPRIVATE_H_ */
