#ifndef PROTOCOL_RESOURCES
#define PROTOCOL_RESOURCES

//! @note	Includes

//FreeRTOS Includes
#include "FreeRTOS.h"
#include "semphr.h"
//Standard Includes
#include "stdint.h"

//! @note	Defines
#ifndef NULL
	#define	NULL	0
#endif

//! @note	Variables externas

extern TProtocolConfig protocolConfig;
extern xSemaphoreHandle protocolReceptionSemph;

#endif
