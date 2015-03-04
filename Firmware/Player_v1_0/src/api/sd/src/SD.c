/*
 * SD_functions.c
 *
 *  Created on: 17/07/2014
 *      Author: Maximiliano
 */

/*
 * @note Includes
 */

#include "SD.h"

/*
 * @note Defines
 */

/*
 * @note Variables globales
 */

/*
 * @var		FATFS SD_sector
 * @brief	Objeto del FILE SYSTEM
 */
extern FATFS SD_sector; //Sector para el FILE SISTEM


/*
 * @note Funciones privadas
 */



/*
 * @note Funciones publicas
 */

/*
 * @fn		uint8_t FAT_Init(void)
 * @brief	Inicializacion del File System
 * @param	void
 * @return	Retorna resultadod e operacion
 */
init_stat 	FAT_Init(void){

	DSTATUS res;

	res = disk_initialize(0);

	if(!res) {
		//Inicializacion y montaje de la SD
		if(!f_mount(0,&SD_sector))
			return(fatInit_ok);
		else
			return(mount_err);
	}
	else {
		if(res == STA_NODISK)
			return(no_card);
		else
			return(disk_err);
	}

}
