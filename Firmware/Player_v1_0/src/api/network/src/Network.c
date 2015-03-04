/**
 * @author	Agustin Alba Chicar
 * @file	Network.c
 * @brief	Archivo fuente que contiene la API Network que permite manejar lo basico del stack TCP/IP
 * @date	27/7/2014
 * @version	1
 * @todo
 * @bug
 */

//!	@note	Includes
#include "Network.h"

//! @note	Variables globales

/**
 * @var	ulEMACTxBufferStart
 * @brief Direccion de memoria del TX buffer de inicio de la EMAC
 * @details The emac buffers are manually placed at the start of the AHB RAM.  These
			variables store the calculated addresses, which are then made available
			through the pvApplicationGetEMACTxBufferAddress() and
			pvApplicationGetEMACRxBufferAddress() access functions.
 */
uint32_t ulEMACTxBufferStart;
/**
 * @var	ulEMACRxBufferStart
 * @brief Direccion de memoria del RX buffer de inicio de la EMAC
 */
uint32_t ulEMACRxBufferStart;


NetworkTInterface *networkTInterface = NULL;

/**
 * @var	networkInterfaceState
 * @brief Estado de la interfaz.
 */
static volatile NetworkInterfaceState networkInterfaceState = INTERFACE_DOWN;

struct netif *networkInterface = NULL;

//! @note Funciones Privadas

/**
 *	@fn	networkStatusHandler(struct netif *netif)
 *	@brief Handler que se ejecuta cuando la interfaz Ethernet se cae o se levanta.
 *	@param netif puntero a la interfaz ethernet que sufrio el cambio.
 */
void networkStatusHandler(struct netif *netif)
{
	//Evaluo el puntero
	if(netif == NULL)
	{
		return;
	}
	else
	{
		networkInterface = netif;
	}

	//Evaluo los flags para saber si esta o no activa la interfaz
	if(netif->flags & NETIF_FLAG_UP)
	{
		//Se levanto la interfaz
		networkInterfaceState = INTERFACE_UP;
	}
	else if(netif->flags & NETIF_FLAG_UP)
	{
		//Se cayo la interfaz
		networkInterfaceState = INTERFACE_DOWN;
	}
}

//! @note Funciones Publicas

/**
 * @fn	networkGetEMACTxBufferAddress( void )
 * @brief Getter del puntero de inicio del buffer de TX de la EMAC
 * @return void * con la direccion de inicio del buffer.
 */
void *networkGetEMACTxBufferAddress( void )
{
	/* Simply return the address calculated as being the bottom of the emac
	Tx buffers. */
	return ( void * ) ulEMACTxBufferStart;
}

/**
 * @fn	networkGetEMACRxBufferAddress( void )
 * @brief Getter del puntero de inicio del buffer de TX de la EMAC
 * @return void * con la direccion de inicio del buffer.
 */
void *networkGetEMACRxBufferAddress( void )
{
	/* Simply return the address calculated as being the bottom of the emac
	Rx buffers. */
	return ( void * ) ulEMACRxBufferStart;
}

/**
 * @fn	networkSetEMACTxBufferAddress( uint32_t pEMACTxBufferStart)
 * @brief Setter del puntero de inicio del buffer de TX de la EMAC
 * @param pEMACTxBufferStart con la direccion de inicio del buffer.
 */
void networkSetEMACTxBufferAddress( uint32_t pEMACTxBufferStart )
{
	ulEMACTxBufferStart = pEMACTxBufferStart;
}

/**
 * @fn	networkSetEMACRxBufferAddress( uint32_t pEMACRxBufferStart )
 * @brief Setter del puntero de inicio del buffer de TX de la EMAC
 * @param pEMACRxBufferStart con la direccion de inicio del buffer.
 */
void networkSetEMACRxBufferAddress( uint32_t pEMACRxBufferStart )
{
	ulEMACRxBufferStart = pEMACRxBufferStart;
}

/**
 * @fn networkInit()
 * @brief Llama a que se inicie el stack, con la funcion de inicio de la interfaz
 */
void networkInit(NetworkTInterface *interface)
{
	networkTInterface = interface;
	/* This call creates the TCP/IP thread. */
	tcpip_init( networkInterfaceInit, networkTInterface );
}

/**
 * @fn networkInterfaceInit(void *arguments)
 * @brief Configura la interfaz con IP fija.
 * @details	En caso de estar habilitadas crea 2 tareas, un cliente y un servidor UDP.
 * @param arguments	es un puntero a void con argumentos genericos para esta funcion.
 */
void networkInterfaceInit(void *arguments)
{
	ip_addr_t xIPAddr, xNetMask, xGateway;
	extern err_t ethernetif_init( struct netif *xNetIf );
	static struct netif xNetIf;

	/* Set up the network interface. */
//	ip_addr_set_zero( &xGateway );
//	ip_addr_set_zero( &xIPAddr );
//	ip_addr_set_zero( &xNetMask );
//
//	LWIP_PORT_INIT_GW(&xGateway);
//	LWIP_PORT_INIT_IPADDR(&xIPAddr);
//	LWIP_PORT_INIT_NETMASK(&xNetMask);

	//Falta MAC!

	//Configuro la interfaz IP con las IPs provistas
	if(networkTInterface != NULL)
	{
		xIPAddr.addr = (uint32_t)networkTInterface->ip[0] |
				((uint32_t)networkTInterface->ip[1] << 8)  |
				((uint32_t)networkTInterface->ip[2] << 16)  |
				((uint32_t)networkTInterface->ip[3] << 24);
		xGateway.addr = (uint32_t)networkTInterface->gateway[0] |
				((uint32_t)networkTInterface->gateway[1] << 8)  |
				((uint32_t)networkTInterface->gateway[2] << 16)  |
				((uint32_t)networkTInterface->gateway[3] << 24);
		xNetMask.addr = (uint32_t)networkTInterface->netmask[0] |
				((uint32_t)networkTInterface->netmask[1] << 8)  |
				((uint32_t)networkTInterface->netmask[2] << 16)  |
				((uint32_t)networkTInterface->netmask[3] << 24);
	}
	else
	{
		ip_addr_set_zero( &xGateway );
		ip_addr_set_zero( &xIPAddr );
		ip_addr_set_zero( &xNetMask );

		LWIP_PORT_INIT_GW(&xGateway);
		LWIP_PORT_INIT_IPADDR(&xIPAddr);
		LWIP_PORT_INIT_NETMASK(&xNetMask);
	}

	//Cargo los valores de la interfaz
	netif_set_default( netif_add( &xNetIf, &xIPAddr, &xNetMask, &xGateway, NULL, ethernetif_init, tcpip_input ) );
	//Cargo el callback de estado de la interfaz
	xNetIf.status_callback = networkStatusHandler;
	//Configuro la interfaz
	netif_set_up( &xNetIf );

#if(NETWORK_CONFIG_UDP_CLIENT_TASK == 1)
	xTaskCreate( networkUDPClientTask, NULL, UDP_CLIENT_TASK_STACK_SIZE, NULL, UDP_CLIENT_TASK_PRIORITY, NULL );
#endif
#if(NETWORK_CONFIG_UDP_SERVER_TASK == 1)
	xTaskCreate( networkUDPServerTask, NULL, configCOMMAND_INTERPRETER_STACK_SIZE, NULL, configCOMMAND_INTERPRETER_TASK_PRIORITY, NULL );
#endif
}

/**
 * @fn networkGetInterfaceStatus()
 * @brief Devuelve el estado de la interfaz.
 * @return networkInterfaceState
 * @retval INTERFACE_DOWN si la interfaz esta caida.
 * 		   INTERFACE_UP si la interfaz esta levantada.
 */
inline NetworkInterfaceState networkGetInterfaceStatus()
{
	return networkInterfaceState;
}

/**
 * @fn networkUDPClientTask (void *arguments)
 * @brief Cliente UDP de proposito de desarrollo
 * @para arguments	Argumentos de la tarea
 */
void networkUDPClientTask (void *arguments)
{
#if(NETWORK_CONFIG_UDP_CLIENT_TASK == 1)
	int udpSocket;
	struct sockaddr_in sLocalAddr, sDestAddr, sRemAddr;
	char text[] = "Hola mundo desde UDP";
	char receiveBuffer[100];

	int i;

	//Creo el socket datagrama
	udpSocket = lwip_socket(AF_INET, SOCK_DGRAM, 0);

	if(udpSocket == -1)
	{
		//Suspendo la tarea ya que se produjo un error irrecuperable
		vTaskSuspend(NULL);
	}

	//Limpio la memoria de las estructuras
	memset((char*)&sLocalAddr, 0, sizeof(sLocalAddr));
	memset((char*)&sDestAddr, 0, sizeof(sDestAddr));

	//Cargo el destinatario
	sDestAddr.sin_family = AF_INET;
	sDestAddr.sin_len = sizeof(sDestAddr);
	sDestAddr.sin_addr.s_addr = inet_addr("192.168.0.4");
	sDestAddr.sin_port = htons( ( ( uint16_t ) 15000 ) );

	//Cargo la direccion de origen
	sLocalAddr.sin_family = AF_INET;
	sLocalAddr.sin_len = sizeof(sLocalAddr);
	sLocalAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	sLocalAddr.sin_port = htons( ( ( uint16_t ) 10000 ) );

	//bind de la direccion de origen
	lwip_bind(udpSocket, (struct sockaddr *)&sLocalAddr, sizeof(sLocalAddr));

	while(1)
	{
		//Escribo
		i = lwip_sendto(udpSocket, text, sizeof(text), 0, (struct sockaddr *)&sDestAddr, sizeof(sDestAddr));
		//Escucho por el socket antes de enviar
		i = lwip_recvfrom(udpSocket, receiveBuffer, 100, 0, NULL,NULL);//(struct sockaddr *)&sRemAddr, sizeof(sRemAddr));//NULL, NULL);
	}
#else
	//Loop infinito suspendiendo la tarea
	while(1)
	{
		vTaskSuspend(NULL);
	}
#endif
}

/**
 * @fn networkUDPServerTask (void *arguments)
 * @brief Servidor UDP de proposito de desarrollo
 * @param arguments	Argumentos de la tarea
 */
void networkUDPServerTask (void *arguments)
{
#if(NETWORK_CONFIG_UDP_CLIENT_TASK == 1)
	int udpSocket;
	struct sockaddr_in sLocalAddr, sDestAddr, sRemAddr;
	char text[] = "Hola mundo desde UDP";
	char receiveBuffer[100];

	int i;

	//Creo el socket datagrama
	udpSocket = lwip_socket(AF_INET, SOCK_DGRAM, 0);

	if(udpSocket == -1)
	{

	}

	//Limpio la memoria de las estructuras
	memset((char*)&sLocalAddr, 0, sizeof(sLocalAddr));
	memset((char*)&sDestAddr, 0, sizeof(sDestAddr));

	//Cargo el destinatario
	sDestAddr.sin_family = AF_INET;
	sDestAddr.sin_len = sizeof(sDestAddr);
	sDestAddr.sin_addr.s_addr = inet_addr("192.168.0.4");
	sDestAddr.sin_port = htons( ( ( uint16_t ) 15000 ) );

	//Cargo la direccion de origen
	sLocalAddr.sin_family = AF_INET;
	sLocalAddr.sin_len = sizeof(sLocalAddr);
	sLocalAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	sLocalAddr.sin_port = htons( ( ( uint16_t ) 10000 ) );


	//bind de la direccion de destino
	lwip_bind(udpSocket, (struct sockaddr *)&sLocalAddr, sizeof(sLocalAddr));

	while(1)
	{
		//i =  lwip_recvfrom(udpSocket, receiveBuffer, sizeof(receiveBuffer), 0, (struct sockaddr *)&sRemAddr, sizeof(sRemAddr));
		i =  lwip_recvfrom(udpSocket, receiveBuffer, sizeof(receiveBuffer), 0, NULL, NULL);
		vTaskDelay(200);
		//i = lwip_sendto(udpSocket, receiveBuffer, i, 0, (struct sockaddr *)&sRemAddr, sizeof(sRemAddr));
		//vTaskDelay(200);
	}
#else
	//Loop infinito suspendiendo la tarea
	while(1)
	{
		vTaskSuspend(NULL);
	}
#endif
}

/**
 * @fn		networkGetAddress(uint32_t address, char *ipBuffer)
 * @brief	Transforma una direccion IP como string
 * @param	addressId	it is the id of the address to request, 0 for IP, 1 for netmask, 2 for gateway
 * @param	ipBuffer	pointer to a valid char buffer for storing at least 4x3 + 3 + 1 = 16 bytes
 * @return	the size of bytes written
 * @retval	-1	si ocurrio un error por address o por que la interfaz no esta levantada aun.
 */
int32_t networkGetAddress(uint32_t addressId, char *ipBuffer)
{
	uint32_t octet;
	uint32_t address;
	uint32_t index;

	if(networkInterface == NULL)
	{
		return -1;
	}

	//Obtengo la direccion requerida
	switch(addressId)
	{
		case 0:
			address = networkInterface->ip_addr.addr;
			break;
		case 1:
			address = networkInterface->netmask.addr;
			break;
		case 2:
			address = networkInterface->gw.addr;
			break;
		default:
			return -1;
			break;
	}
	//Parseamos octeto a octeto
	octet = address & 0xFF;
	index = sprintf(ipBuffer, "%d.",octet);

	octet = (address >> 8) & 0xFF;
	index += sprintf(&(ipBuffer[index]), "%d.",octet);

	octet = (address >> 16) & 0xFF;
	index += sprintf(&(ipBuffer[index]), "%d.",octet);

	octet = (address >> 24) & 0xFF;
	index += sprintf(&(ipBuffer[index]), "%d",octet);

	//Cargo el terminador final para asegurarme
	ipBuffer[index] = 0;

	return index;
}




