#ifndef APPINIT_H
	#define APPINIT_H

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Library includes. */
#include "LPC17xx.h"
#include "lpc17xx_emac.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_timer.h"

/* lwIP includes. */
#include "lwip/tcpip.h"

//Standard includes
#include "stdlib.h"
#include "string.h"

//API propia
#include "appConfig.h"
#include "Network.h"
#include "Protocol.h"
#include "appHandler.h"
#include "Player.h"
#include "ConfigurationServer.h"
#include "debFlashMemory.h"

//API sta013
#include "sta013_api.h"

void appSetupHardware();
int32_t appProtocolConfig();
int32_t appPlayerConfig(uint8_t *serverIP, uint16_t port);
int32_t appApplicationConfig();
int32_t appUserInputInit();
void prvManuallyPlaceLargeDataInAHBRAM( void );
int32_t appConfigurationServerConfig();

extern ConfigurationSeverT appConfiguration;

#endif

