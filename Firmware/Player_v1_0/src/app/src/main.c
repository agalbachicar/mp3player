#include "appConfig.h"
#include "appInit.h"
#include "appApplication.h"

#include "stdio.h"

//!	@note	Variables globales
ConfigurationSeverT appConfiguration;

int main( void )
{
	uint32_t status;
	//Inicializo la RAM y el Heap de FreeRTOS
	appSetupHardware();

	//Configuro los botones de entrada
	appUserInputInit();

	//Inicializo la FLASH
	IAP_Config();

	//Evaluo la entrada del boton rojo, si al arrancar esta presionado,
	//Consulto el boton OK
	status = GPIO_ReadValue(APP_APPLICATION_GPIO_PORT);
	status = (status >> APP_APPLICATION_OK_GPIO_PIN);
	status = (status & 0x01);
	if(!status)
	{
		//Como esta presionado comenzamos el modo servidor para poder recibir la configuracion
		appConfiguration.interface.ip[0] = 192;
		appConfiguration.interface.ip[1] = 168;
		appConfiguration.interface.ip[2] = 2;
		appConfiguration.interface.ip[3] = 200;
		appConfiguration.interface.netmask[0] = 255;
		appConfiguration.interface.netmask[1] = 255;
		appConfiguration.interface.netmask[2] = 255;
		appConfiguration.interface.netmask[3] = 0;
		appConfiguration.interface.gateway[0] = 192;
		appConfiguration.interface.gateway[1] = 168;
		appConfiguration.interface.gateway[2] = 1;
		appConfiguration.interface.gateway[3] = 1;
		appConfiguration.interface.mac[0] = 2;
		appConfiguration.interface.mac[1] = 0;
		appConfiguration.interface.mac[2] = 1;
		appConfiguration.interface.mac[3] = 0;
		appConfiguration.interface.mac[4] = 0;
		appConfiguration.interface.mac[5] = 1;

		//Inicializo el stack ethernet
		networkInit(&(appConfiguration.interface));
		//Arranco el LCD & GUI
		StartGUI();
		//Creo el servidor de configuracion
		if(appConfigurationServerConfig() == -1)
		{
			while(1);
		}
	}
	else
	{
		//Cargamos la configuracion corriente y levantamos la aplicacion normalmente
		//Configuro el protocolo
		if(appProtocolConfig() == -1)
		{
			while(1);
		}

		//Leemos la configuracion previamente guardada
		if(ConfigurationServerLoadConfiguration(&appConfiguration) != 0)
		{
			//Cargamos la configuracion por defecto
			appConfiguration.interface.ip[0] = 192;
			appConfiguration.interface.ip[1] = 168;
			appConfiguration.interface.ip[2] = 2;
			appConfiguration.interface.ip[3] = 200;
			appConfiguration.interface.netmask[0] = 255;
			appConfiguration.interface.netmask[1] = 255;
			appConfiguration.interface.netmask[2] = 255;
			appConfiguration.interface.netmask[3] = 0;
			appConfiguration.interface.gateway[0] = 192;
			appConfiguration.interface.gateway[1] = 168;
			appConfiguration.interface.gateway[2] = 1;
			appConfiguration.interface.gateway[3] = 1;
			appConfiguration.interface.mac[0] = 2;
			appConfiguration.interface.mac[1] = 0;
			appConfiguration.interface.mac[2] = 1;
			appConfiguration.interface.mac[3] = 0;
			appConfiguration.interface.mac[4] = 0;
			appConfiguration.interface.mac[5] = 1;
			appConfiguration.port = 15000;
			appConfiguration.serverIP[0] = 192;
			appConfiguration.serverIP[1] = 168;
			appConfiguration.serverIP[2] = 2;
			appConfiguration.serverIP[3] = 4;
			ConfigurationServerSaveConfiguration(&appConfiguration);
		}

		//Configuro Sta013
		sta013_Init();

		//Inicializo el stack ethernet
		networkInit(&(appConfiguration.interface));

		//Arranco el LCD & GUI
		StartGUI();

		//Inicializo el player
		if(appPlayerConfig(appConfiguration.serverIP, appConfiguration.port) == -1)
		{
			while(1);
		}

		//Inicializo las tareas de aplicacion
		if(appApplicationConfig() == -1)
		{
			while(1);
		}

	#ifdef	TEST_MODE
		//Creo la tarea de test de la maquina de estados de la aplicacion
		if(xTaskCreate( appApplicationTestTask,
						NULL,
						200,
						NULL,
						APP_APPLICATION_PRIORITY,
						NULL ) != pdPASS)
		{
			while(1);
		}
		#endif
	}

	//Start the FreeRTOS scheduler.
	vTaskStartScheduler();

	for(;;);

	return (1);
}
