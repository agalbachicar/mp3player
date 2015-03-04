#include "appInit.h"

/* The libraries use large data arrays.  Place these manually in the AHB RAM
so their combined size is not taken into account when calculating the total
flash+RAM size of the generated executable. */
uint8_t *pucFreeRTOSHeap = NULL;


/**
 * @fn		appSetupHardware()
 * @brief	Inicializa la RAM y las interrupciones
 */
void appSetupHardware()
{
	/* The examples assume that all priority bits are assigned as preemption
	priority bits. */
	NVIC_SetPriorityGrouping( 0UL );

	/* Force some large data structures into AHB RAM to keep the calculated
	flash+RAM code size down. */
	prvManuallyPlaceLargeDataInAHBRAM();
}

/**
 * @fn		appProtocolConfig()
 * @brief	Inicializa el modulo del protocolo por Ethernet
 * @retval	-1 si hay error
 *@retval	0 si se realiza exitosamente
 */
int32_t appProtocolConfig()
{
	TProtocolConfig config;

	//Cargo la configuracion de los handlers
	config.receiveCallback = playerEthernetProtocolReceiveHandler;
	config.errorCallback = playerEthernetErrorHandler;

	//Cargo la configuracion del buffer de recepcion
	config.receptionBuffer = playerEthernetReceptionBuffer;
	config.receptionBufferSize = PLAYER_ETHERNET_BUFFER;
	config.receptionBufferIndex = 0;

	//Cargo la configuracion del buffer de envio
	config.transmitBuffer = playerEthernetTransmitionBuffer;
	config.transmitBufferSize = PLAYER_ETHERNET_BUFFER;
	config.transmitBufferIndex = 0;

	//Configuro el puerto
	config.port = APP_PORT;

	//Configuro el timeout
	config.timeoutTimeInUSec = APP_TIMEOUT;

	//Cargo la configuracion a la estructura
	return protocolSetConfig(&config);
}
/**
 * @fn		appPlayerConfig()
 * @brief	Inicializo la memoria de la API del player
 * @retval	-1 si falla la creacion de algun recurso
 * @retval	0 si se inicializa correctamente
 */
int32_t appPlayerConfig(uint8_t *serverIP, uint16_t port)
{
	//Creo los semaforos
	vSemaphoreCreateBinary( playerReceiveSemph );
	if(playerReceiveSemph == NULL)
	{
		return -1;
	}
	vSemaphoreCreateBinary( playerTransmitSemph );
	if(playerReceiveSemph == NULL)
	{
		return -1;
	}
	vSemaphoreCreateBinary( playerStreamingSemaphore );
	if(playerReceiveSemph == NULL)
	{
		return -1;
	}
	//Creo la tarea de reproduccion
	if(xTaskCreate( playerStreamTask,
					NULL,
					PLAYER_STREAM_STACKSIZE,
					NULL,
					PLAYER_STREAM_PRIORITY,
					NULL ) != pdPASS)
	{
		return -1;
	}

	//Cargo la IP y el puerto del servidor
	playerEthernetSetServerData(serverIP, port);

	return 0;
}

/**
 * @fn		appApplicationConfig()
 * @brief	Inicializa la tarea de aplicacion y el semaforo de los botones
 */
int32_t appApplicationConfig()
{
	//Creo el semaforo
	vSemaphoreCreateBinary( appApplicationButtonSemaphore );
	if(appApplicationButtonSemaphore == NULL)
	{
		return -1;
	}
	//Creo la tarea de aplicacion
	if(xTaskCreate( appApplicationGeneralTask,
					NULL,
					APP_APPLICATION_STACKSIZE,
					NULL,
					APP_APPLICATION_PRIORITY,
					NULL ) != pdPASS)
	{
		return -1;
	}


	return 0;
}

/**
 * @fn		appUserInputInit()
 * @brief	Inicializa los botones para el uso del usuario
 * @retval	0
 */
int32_t appUserInputInit()
{
	PINSEL_CFG_Type pinConfig;

	//Inicializo los puertos de los GPIO
	pinConfig.Funcnum = PINSEL_FUNC_0;
	pinConfig.OpenDrain = PINSEL_PINMODE_NORMAL;
	pinConfig.Pinmode = PINSEL_PINMODE_PULLUP;
	pinConfig.Portnum= APP_APPLICATION_GPIO_PORT;
	pinConfig.Pinnum = APP_APPLICATION_DOWN_GPIO_PIN;
	PINSEL_ConfigPin(&pinConfig);
	pinConfig.Pinnum = APP_APPLICATION_OK_GPIO_PIN;
	PINSEL_ConfigPin(&pinConfig);
	pinConfig.Pinnum = APP_APPLICATION_UP_GPIO_PIN;
	PINSEL_ConfigPin(&pinConfig);
	pinConfig.Pinnum = APP_APPLICATION_BACK_GPIO_PIN;
	PINSEL_ConfigPin(&pinConfig);
	//Coloco los puertos como entradas
	GPIO_SetDir(APP_APPLICATION_GPIO_PORT, 1 << APP_APPLICATION_DOWN_GPIO_PIN, 0);
	GPIO_SetDir(APP_APPLICATION_GPIO_PORT, 1 << APP_APPLICATION_OK_GPIO_PIN, 0);
	GPIO_SetDir(APP_APPLICATION_GPIO_PORT, 1 << APP_APPLICATION_UP_GPIO_PIN, 0);
	GPIO_SetDir(APP_APPLICATION_GPIO_PORT, 1 << APP_APPLICATION_BACK_GPIO_PIN, 0);

	//Coloco los puertos como salidas (de los multiplexores)
	GPIO_SetDir(APP_APPLICATION_GPIO_PORT, APP_APPLICATION_PINS, 1);

	//Inicializo el timer
	//Configuracion general del timer
	timerConfig(APP_APPLICATION_TIMER, INTERRUPT_REPETITIVE, APP_APPLICATION_SAMPLE_GPIO_PERIOD_US);
	timerConfigCallback(APP_APPLICATION_TIMER, appApplicationTimerHandler);

	return 0;
}

/**
 * @fn		appConfigurationServerConfig()
 * @brief	Es la funcion de configuracion de la tarea del servidor de configuracion.
 * @retval	-1 si no se puede crear la tarea
 * @retval	0 si se puede crear la tarea
 */
int32_t appConfigurationServerConfig()
{
	IAP_Config();

	//Creo la tarea de aplicacion
	if(xTaskCreate( ConfigurationServerTask,
					NULL,
					CONFIGURATION_SERVER_STACKSIZE,
					NULL,
					CONFIGURATION_SERVER_PRIORITY,
					NULL ) != pdPASS)
	{
		return -1;
	}

	return 0;
}
/**
 * @details	The libraries use large data arrays.  Place these manually in the AHB RAM
			so their combined size is not taken into account when calculating the total
			Flash+RAM size of the generated executable.
 */
void prvManuallyPlaceLargeDataInAHBRAM( void )
{
	uint32_t ulAddressCalc;
	extern uint32_t __top_RamAHB32[];
	const uint32_t ulBaseAddress = 0x2007c000UL;

	/* Start at the bottom of the second bank of RAM.  Need to use a linker
	variable here! */
	ulAddressCalc = ulBaseAddress;
	networkSetEMACTxBufferAddress( ulAddressCalc );

	/* Move up far enough to hold all the Tx buffers. */
	ulAddressCalc += sizeof( xEMACTxBuffer_t );

	/* Align and store the calculated address. */
	ulAddressCalc += 0x0fUL;
	ulAddressCalc &= ~0x0fUL;
	networkSetEMACRxBufferAddress( ulAddressCalc );

	/* Move up far enough to hold all the Rx buffers. */
	ulAddressCalc += sizeof( xEMACRxBuffer_t );

	/* Align and assign the calculated address to the FreeRTOS heap. */
	ulAddressCalc += 0x0fUL;
	ulAddressCalc &= ~0x0fUL;
	pucFreeRTOSHeap = ( uint8_t * ) ulAddressCalc;

	/* Sanity check that the variables placed in AHB RAM actually fit in AHB
	RAM before zeroing down the memory. */
	configASSERT( ( ulAddressCalc + configTOTAL_HEAP_SIZE ) < ( uint32_t ) __top_RamAHB32 );
	memset( ( void * ) ulBaseAddress, 0x00, ( ulAddressCalc + configTOTAL_HEAP_SIZE ) - ulBaseAddress );

	/* Remove compiler warnings when configASSERT() is not defined. */
	( void ) __top_RamAHB32;
}
