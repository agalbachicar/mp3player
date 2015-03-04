#include "ConfigurationServer.h"

static const char *ConfigurationServerTokenACK = "ACK";
static const char *ConfigurationServerTokenNACK = "NACK";

void ConfigurationServerTask(void *arguments)
{
   int32_t socket,n;
   struct sockaddr_in servaddr,cliaddr;
   socklen_t len;
   ConfigurationSeverT configuration;

   while(1)
   {
	   printRaisingUpInterface();
	   //Bloqueamos hasta que se haya levantado la interfaz
	   while(networkGetInterfaceStatus() == INTERFACE_DOWN)
	   {
		   vTaskDelay(1000);
	   }
	   //Obtenemos el socket
	   socket = socket(AF_INET,SOCK_DGRAM,0);
	   if(socket == -1)
	   {
		   //Error
		   continue;
	   }
	   //Configuramos el socket y lo bindeamos a un puerto y cualquier direccion
	   memset(&servaddr, 0, sizeof(servaddr));
	   servaddr.sin_family = AF_INET;
	   servaddr.sin_addr.s_addr= htonl(INADDR_ANY);
	   servaddr.sin_port=htons(CONFIGURATION_SERVER_PORT);
	   if(lwip_bind(socket, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
	   {
		   //Error
		   lwip_close(socket);
		   continue;
	   }

	   //Nos ponemos a escuchar
	   printWaitingForIncommingConfiguration();
	   len = sizeof(cliaddr);
	   n = lwip_recvfrom(socket, &configuration, sizeof(configuration), 0, (struct sockaddr *)&cliaddr, &len);
	   if(n == sizeof(configuration))
	   {
		   //Leimos los bytes de la configuracion, respondemos con el token de correcto
		   lwip_sendto(socket,ConfigurationServerTokenACK, strlen(ConfigurationServerTokenACK), 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
		   //Cerramos el socket
		   lwip_close(socket);
		   //Cargamos la configuracion en la FLASH
		   ConfigurationServerSaveConfiguration(&configuration);
		   //Informamos que se configuro correctamente
		   printAckConfiguration();
		   //Suspendemos la tarea
		   vTaskSuspend(NULL);
	   }
	   else
	   {
		   //Leimos los bytes de la configuracion, respondemos con el token de correcto
		   lwip_sendto(socket,ConfigurationServerTokenNACK, strlen(ConfigurationServerTokenNACK), 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
		   //Cerramos el socket
		   lwip_close(socket);
		   //Informamos el error configurando
		   printNAckConfiguration();
		   //Volvemos a empezar tras una demora
		   vTaskDelay(2000);
	   }
   }
}

void ConfigurationServerSaveConfiguration(ConfigurationSeverT *configuration)
{
	//Paramos el Scheduler
	taskENTER_CRITICAL();
	//Borramos la FLASH
	IAP_Clean (CONFIGURATIONSERVER_FLASH_SECTOR, CONFIGURATIONSERVER_FLASH_SECTOR);
	//Grabamos en FLASH la informacion
	IAP_WriteRAMtoFLASH((uint32_t)configuration, CONFIGURATIONSERVER_FLASH_SECTOR,
			FSectorAddress[CONFIGURATIONSERVER_FLASH_SECTOR], sizeof(ConfigurationSeverT));
	//Retornamos la ejecucion del Scheduler
	taskEXIT_CRITICAL();
}

int32_t ConfigurationServerLoadConfiguration(ConfigurationSeverT *configuration)
{
	uint32_t result;

	//Evaluamos si el sector esta blanqueado
	result = IAP_BlankCheck (CONFIGURATIONSERVER_FLASH_SECTOR, CONFIGURATIONSERVER_FLASH_SECTOR);
	if (result == SECTOR_NOT_BLANK)
	{
		//Obtenemos el address
		IAP_ReadFlashToRAM(FSectorAddress[CONFIGURATIONSERVER_FLASH_SECTOR], (uint32_t)configuration, sizeof(ConfigurationSeverT));
		return 0;
	}
	//O bien el sector esta blanqueado o es invalido no pudimos leer
	return -1;
}

void printRaisingUpInterface()
{
	//Limpio Pantalla
	Erase_Pant(WHITE);
	//Escribo
	Draw_String( 5, 7, "CONFIGURACION", BLACK);
	Draw_String( 0, 15, "---------------------", BLACK);
	Draw_String( 0, 23, "Levantando", BLACK);
	Draw_String( 0, 31, "interfaz...", BLACK);
	Draw_String( 0, 47, "---------------------", BLACK);
}
void printWaitingForIncommingConfiguration()
{
	//Limpio Pantalla
	Erase_Pant(WHITE);
	//Escribo
	Draw_String( 5, 7, "CONFIGURACION", BLACK);
	Draw_String( 0, 15, "---------------------", BLACK);
	Draw_String( 0, 23, "Esperando", BLACK);
	Draw_String( 0, 31, "configuracion...", BLACK);
	Draw_String( 0, 47, "---------------------", BLACK);
}
void printAckConfiguration()
{
	//Limpio Pantalla
	Erase_Pant(WHITE);
	//Escribo
	Draw_String( 5, 7, "CONFIGURACION", BLACK);
	Draw_String( 0, 15, "---------------------", BLACK);
	Draw_String( 0, 23, "Recepcion exitosa.", BLACK);
	Draw_String( 0, 31, "Reinicie el equipo.", BLACK);
	Draw_String( 0, 47, "---------------------", BLACK);
}
void printNAckConfiguration()
{
	//Limpio Pantalla
	Erase_Pant(WHITE);
	//Escribo
	Draw_String( 5, 7, "CONFIGURACION", BLACK);
	Draw_String( 0, 15, "---------------------", BLACK);
	Draw_String( 0, 31, "Error en recepcion", BLACK);
	Draw_String( 0, 47, "---------------------", BLACK);
}

