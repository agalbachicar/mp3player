/*
 * MusicProtocol.h
 *
 *  Created on: Jul 18, 2014
 *      Author: agustin
 */

#ifndef MUSICPROTOCOL_H_
#define MUSICPROTOCOL_H_

//! @note	Includes
#include "stdint.h"

//! @note	Defines
#ifndef NULL
	#define	NULL	0
#endif

/**
 * @enum EMusicProtocolCommand
 * @brief Valores de los comandos del protocolo
 */
typedef enum EMusicProtocolCommand
{
    DOWNLOAD_SONG = INITIAL_COMMAND_FOR_APP,
    GET_NEXT_SONG_DATA = INITIAL_COMMAND_FOR_APP + 1,
    GET_PREVIOUS_SONG_DATA = INITIAL_COMMAND_FOR_APP + 2,
    PREPAIRE_SONG = INITIAL_COMMAND_FOR_APP + 3,
    DISPOSE_SONG = INITIAL_COMMAND_FOR_APP + 4,
    AACK = INITIAL_COMMAND_FOR_APP + 5,
}EMusicProtocolCommand;








#endif /* MUSICPROTOCOL_H_ */
