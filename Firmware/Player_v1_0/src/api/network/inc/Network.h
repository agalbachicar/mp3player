/**
 * @author	Agustin Alba Chicar
 * @file	Network.h
 * @brief	Archivo header de la API de configuracion de la interfaz de red.
 * @date	27/7/2014
 * @version	1.0
 * @todo
 * @bug
 */
#ifndef NETWROK_H
#define NETWROK_H

//!	@note Includes

//FreeRTOS includes
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
//lwIP core includes
#include "lwip/opt.h"
#include "lwip/tcpip.h"
#include "netif/etharp.h"
#include "lwip/opt.h"
#include "lwip/sockets.h"
//Standard Includes
#include "stdlib.h"

//!	@note Defines
/* The constants that define the IP address, net mask, gateway address and MAC
address are located at the bottom of FreeRTOSConfig.h. */
#define LWIP_PORT_INIT_IPADDR(addr)   IP4_ADDR((addr), configIP_ADDR0, configIP_ADDR1, configIP_ADDR2, configIP_ADDR3 )
#define LWIP_PORT_INIT_GW(addr)       IP4_ADDR((addr), configGW_IP_ADDR0, configGW_IP_ADDR1, configGW_IP_ADDR2, configGW_IP_ADDR3 )
#define LWIP_PORT_INIT_NETMASK(addr)  IP4_ADDR((addr), configNET_MASK0,configNET_MASK1,configNET_MASK2,configNET_MASK3)
#define LWIP_MAC_ADDR_BASE            { configMAC_ADDR0, configMAC_ADDR1, configMAC_ADDR2, configMAC_ADDR3, configMAC_ADDR4, configMAC_ADDR5 }

/**
 * @def	NETWORK_CONFIG_UDP_CLIENT_TASK
 * @brief Con un 0 deshabilita el modulo, con un 1 lo habilita
 */
#define	NETWORK_CONFIG_UDP_CLIENT_TASK	0
#if (NETWORK_CONFIG_UDP_CLIENT_TASK == 1)
	/**
	 * @def	UDP_CLIENT_TASK_STACK_SIZE
	 * @brief Stack de la tarea del cliente UDP.
	 */
	#define UDP_CLIENT_TASK_STACK_SIZE		(1000)
	/**
	 * @def	UDP_CLIENT_TASK_PRIORITY
	 * @brief Prioridad de la tarea del cliente UDP.
	 */
	#define UDP_CLIENT_TASK_PRIORITY		((uint32_t)3)
#endif

/**
 * @def	NETWORK_CONFIG_UDP_SERVER_TASK
 * @brief Con un 0 deshabilita el modulo, con un 1 lo habilita
 */
#define	NETWORK_CONFIG_UDP_SERVER_TASK	0
#if (NETWORK_CONFIG_UDP_SERVER_TASK == 1)
	/**
	 * @def	UDP_CLIENT_TASK_STACK_SIZE
	 * @brief Stack de la tarea del servidor UDP.
	 */
	#define UDP_CLIENT_TASK_STACK_SIZE		(1000)
	/**
	 * @def	UDP_CLIENT_TASK_PRIORITY
	 * @brief Prioridad de la tarea del servidor UDP.
	 */
	#define UDP_CLIENT_TASK_PRIORITY		((uint32_t)3)
#endif


//!	@note	Tipos de dato
/**
 * @enum NetworkInterfaceState
 * @brief	Estado de la interfaz
 * @details	INTERFACE_DOWN	interfaz caida
 * 			INTERFACE_UP	interfaz levantada
 */
typedef enum NetworkInterfaceState
{
	INTERFACE_DOWN = 0,
	INTERFACE_UP = 1
}NetworkInterfaceState;

typedef struct NetworkTypeInterface
{
	uint8_t ip[4];
	uint8_t netmask[4];
	uint8_t gateway[4];
	uint8_t mac[6];
}NetworkTInterface;

//!	@note Prototipos de funciones privadas
void networkStatusHandler(struct netif *netif);

//!	@note Prototipos de funciones publicas
void *networkGetEMACTxBufferAddress( void );
void *networkGetEMACRxBufferAddress( void );
void networkSetEMACTxBufferAddress( uint32_t pEMACTxBufferStart );
void networkSetEMACRxBufferAddress( uint32_t pEMACRxBufferStart );

void networkInit(NetworkTInterface *interface);
void networkInterfaceInit(void *arguments);
inline NetworkInterfaceState networkGetInterfaceStatus();

void networkUDPClientTask (void *arguments);
void networkUDPServerTask (void *arguments);

int32_t networkGetAddress(uint32_t addressId, char *ipBuffer);

//!	@note Variables externas
extern uint32_t ulEMACTxBufferStart;
extern uint32_t ulEMACRxBufferStart;

#endif
