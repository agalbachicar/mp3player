/**
 * @author Vega Maximiliano
 * @file SD.h
 * @brief Header para manejo de memoria SD
 * @date 14/07/2014
 * @version 1.0
 * @todo
 * @bug Beta
 */

#ifndef SD_H
#define	SD_H

/**
 * @note        Includes
 */

//-----------------------------String------------------------------------
#include <string.h>
#include <stdint.h>
//---------------------------Lib_Contenedres-----------------------------
# include <ColaCircular.h>
//----------------------------Lib_API_CAPI-------------------------------
# include <GPIO.h>
# include <SSP.h>
# include <CAPI_Definitions.h>
//--------------------------Lib_FAT_ElmChanFS----------------------------
# include <CAPI_capa_SPI.h>
# include <CAPI_diskio.h>
# include <ff.h>
# include <ffconf.h>
# include <integer.h>
# include <SD_Definitions.h>
//--------------------------Lib_MCU_LPC176x-----------------------------
# include <lpc_176X_PeriphNames.h>
# include <lpc_176X_PinNames.h>
//----------------------------------------------------------------------


/**
 * @note        Defines
 */

#define MAX_EXT ((uint8_t) 5) //maxima cantidad de caracteres de extension

/**
 * @note        Tipos de datos, estructuras, typdefs
 */

/*
 * @enum	sd_ext
 * @brief	Extensiones admitidas
 */
typedef enum{
	mp3,
	directorio,
	no_recon,
	ext_recon,
}sd_ext;

/*
 * @enum	dir_stat
 * @brief	Estado del directorio
 */
typedef enum{
	dir_empty, //no hay mas items por sacar
	valid_item, //hay items por sacar y se saco uno valido
	invalid_item, //hay items para sacar y se saco uno no valido
	dir_full, //esatdo de directorio aun no comenzado a leer
}dir_stat;

/*
 * @enum	init_stat
 * @brief	Estado de inicializacion
 */
typedef enum{
	mount_err = 3,
	fatInit_ok = 0,
	disk_err = 2,
	no_card = 1,
}init_stat;

/*
 * @struct  dir_type
 * @brief	Estructura para manejar directorio actual
 */
typedef struct{
	DIR 		data;
	char 		path[_MAX_LFN+1];
	uint32_t	org_pos; //Posicion de origen del directorio actual
	uint32_t	predir_org; //Posiicon de origen del directorio anteior
}dir_type;


/*
 * @note 	Variables Publicas
 */

FATFS SD_sector; //Sector para el FILE SISTEM


/**
 * @note        Funciones publicas
 */

/*
 * @fn		uint8_t FAT_Init(void)
 * @brief	Inicializacion del File System
 * @param	void
 * @return	Retorna resultado de la operacion
 */
init_stat	FAT_Init(void);

/*
 * @note 	Funciones privadas
 */


#endif	/* SD_H */


