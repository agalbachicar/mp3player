/*
 *  mFlashMemory.h
 *
 *  Created on: 07/03/2012
 *  Author: Eze
 */

// About LPC1769 Flash Memory: http://www.embeddedrelated.com/groups/lpc2000/show/54519.php
// About LPC1769 Flash Memory - File System: http://www.utasker.com/forum/index.php?topic=136.0
// About LPC1769: http://www.nxp.com/documents/user_manual/UM10360.pdf
// Read chapters: (2) Memory Map y (32) Flash Memory
// IAP: Internal Flash Write
// IAP.cpp: http://mbed.org/users/okano/programs/IAP_internal_flash_write/5ypjo/docs/IAP_8cpp_source.html
// IAP.h: http://mbed.org/users/okano/programs/IAP_internal_flash_write/5ypjo/docs/IAP_8h_source.html
// main.cpp: http://mbed.org/users/okano/programs/IAP_internal_flash_write/5ypjo/docs/main_8cpp_source.html
// IAP: Write sector 28
// IAP.cpp: http://mbed.org/users/okano/programs/__IAP_sector28write/m424vi/docs/IAP_8cpp_source.html
// IAP.h: http://mbed.org/users/okano/programs/__IAP_sector28write/m424vi/docs/IAP_8h_source.html
// main.cpp: http://mbed.org/users/okano/programs/__IAP_sector28write/m424vi/docs/main_8cpp_source.html
// IMPORTANTES - SOBRE LA MEMORIA DEL MICROCONTROLADOR
// IAP usa 32 bytes de la parte superior de la memoria RAM. Pero este lugar también está reservado para el stack.
// El stack crece desde la parte superior de la memoria RAM hacia abajo.
// Entonces lo que hay que hacer es poner un offset de 32 bytes para estar tranquilos que la memoria
// reservada para el IAP no sea pisada por el stack. Esto se hace en:
// Project Properties ->  C/C++ Build -> Settings -> MCU Linker -> Target and modify the value in the "Stack Offset" field from 0 to 32.
// En el stack se almacena toda la información relacionada con las llamadas a funciones y el tema de los parámetros
// El stack arranca en la parte superior de la RAM del microcontrolador y va creciendo hacia abajo
// La RAM de usuario inicia en la parte de abajo de la RAM y va creciendo hacia arriba.
// En la parte de abajo de la RAM se pone toda la memoria estática (en tiempo de compilación)
// Lo que sobra entre la RAM y el Stack se puede usar como heap
// Heap es una memoria donde hago asignaciones dinámicas (en tiempo de ejecución)
// Este micro no tiene un heap manager por defecto para eso se usa un sistema operativo
// El microcontrolador tiene RAM (64K) y FLASH (512K) mapeada en una misma memoria lógica (4 GB address space)
// Cuando termino de compilar un programa el IDE me da los siguientes datos:
// text = memory flash ocupada en bytes
// data = son variables puestas tanto en flash como en RAM porque tienen valores iniciales
// bss = son variables puestas en RAM porque no tienen valores iniciales
// Como está hecho el programa ahora, la matriz matrixMemory está en RAM entonces la limitación está por este lado
// Algunas cosas sobre palabras cables:
// En ARM la palabra clave const hace que una variable se ponga en la Flash
// La palabra clave static controla el ciclo de vida de una variable y la visibilidad de la misma
// La palabra clave static es una storage class
// Todas las storage class que existen son: static, extern, auto y register.
// Todas las variables y funciones reciben un storage class
// Si no se lo especifica hay que ver que define el compilador por defecto
// Palabra clave extern: lifetime is static (vive lo que vive el programa) y linkage es visibilidad global al programa
// Palabra clave static: lifetime is static (vive lo que vive el programa y linkage es visibilidad al "archivo"
// Palabra calve static en variables globales en C: visibilidad al "archivo"
// Palabra clave static en variable local en C: visibilidad a la función correspondiente pero retiene el valor a traves de las llamadas
// No confundir la palabra static con static memory allocation. Lo último es memoria estátida diferente a la memoria dinámica.
// La mejor forma de resolver el problema sería copiar la información que recibo por el puerto serie en Flash
// Después moverme con punteros por la Flash y no por la RAM. Así tendría disponibles los 512 KByte de memoria.
// Esto lo voy a hacer en la versión 2 de la matriz
// Ver el tema de Code Read Protection: http://support.code-red-tech.com/CodeRedWiki/CodeReadProtect
// En este proyecto lo tengo desactivado
// Si voy a la carpeta Debug y después voy al archivo .map ahí veo bien como se asigno toda la memoria.
// Abajo de esto en el archivo aparece toda la información
// START GROUP
// LOAD c:/nxp/lpcxpresso_4.1.5_219/lpcxpresso/tools/bin/../lib/gcc/arm-none-eabi/4.5.1/../../../../arm-none-eabi/lib/thumb2\libcr_c.a
// LOAD c:/nxp/lpcxpresso_4.1.5_219/lpcxpresso/tools/bin/../lib/gcc/arm-none-eabi/4.5.1/thumb2\libcr_eabihelpers.a
// END GROUP
// La flash se va llenando desde abajo hacia arriba y arranca en la dirección 0.
// Si paso el proyecto de modo DEBUG a modo RELEASE, ocupa mucho menos en memoria.
// Acá hay información sobre como usar memoria dinámica sin RTOS
// http://tungsys.wordpress.com/2011/10/05/lpcxpresso-flashram-size-and-memory-management/
#ifndef MFLASHMEMORY_H_
#define MFLASHMEMORY_H_

#include "stdint.h"
#include "stdlib.h"

/*
 *      Sector22:    0x00040000 - 0x00047FFF        32K
 *      Sector23:    0x00048000 - 0x0004FFFF        32K
 *      Sector24:    0x00050000 - 0x00057FFF        32K
 *      Sector25:    0x00058000 - 0x0005FFFF        32K
 *      Sector26:    0x00060000 - 0x00067FFF        32K
 *      Sector27:    0x00068000 - 0x0006FFFF        32K
 *      Sector28:    0x00070000 - 0x00077FFF        32K
 *      Sector29:    0x00078000 - 0x0007FFFF        32K
 */

#define	FLASH_SECTOR_23				 0x00048000
#define FLASH_SECTOR_24      		 0x00050000
#define FLASH_SECTOR_25      		 0x00058000
#define FLASH_SECTOR_26      		 0x00060000
#define FLASH_SECTOR_27				 0x00068000
#define FLASH_SECTOR_28				 0x00070000
#define FLASH_SECTOR_29				 0x00078000
#define	DEBFLASH_MAX_SOURCE_ADDRESS	 0x00080000

#define FLASH_SECTOR_SIZE_0_TO_15 	(4 * 1024)
#define FLASH_SECTOR_SIZE_16_TO_20	(32 * 1024)

#define     IAP_LOCATION    		0x1fff1ff1

enum EnumErrorCode
{
	CMD_SUCCESS = 0,
	INVALID_COMMAND = 1,
	SRC_ADDR_ERROR = 2,
	DST_ADDR_ERROR = 3,
	SRC_ADDR_NOT_MAPPED = 4,
	DST_ADDR_NOT_MAPPED = 5,
	COUNT_ERROR = 6,
	INVALID_SECTOR = 7,
	SECTOR_NOT_BLANK = 8,
	SECTOR_NOT_PREPARED_FOR_WRITE_OPERATION = 9,
	COMPARE_ERROR = 10,
	BUSY = 11,
	PARAM_ERROR = 12,
	ADDR_ERROR = 13,
	ADDR_NOT_MAPPED = 14,
	CMD_LOCKED = 15,
	INVALID_CODE = 16,
	INVALID_BAUD_RATE = 17,
	INVALID_STOP_BIT = 18,
	CODE_READ_PROTECTION_ENABLED = 19
};
typedef enum EnumErrorCode EErrorCode;

enum EnumCommandCode
{
	IAPCommand_Prepare_sector_for_write_operation = 50,
	IAPCommand_Copy_RAM_to_Flash = 51,
	IAPCommand_Erase_sector = 52,
	IAPCommand_Blank_check_sector = 53,
	IAPCommand_Read_part_ID = 54,
	IAPCommand_Read_Boot_Code_version = 55,
	IAPCommand_Compare = 56,
	IAPCommand_Reinvoke_ISP = 57,
	IAPCommand_Read_device_serial_number = 58
};
typedef enum EnumCommandCode ECommandCode;

typedef void (*PointerFunctionIAP)(uint32_t[], uint32_t[]);

extern PointerFunctionIAP IAP_Entry;

void IAP_Config();

uint32_t IAP_BlankCheck(uint32_t sectorStart, uint32_t sectorEnd);
uint32_t IAP_Erase(uint32_t start, uint32_t end);
uint32_t IAP_Prepare(uint32_t sectorStart, uint32_t sectorEnd);
uint32_t IAP_Clean(uint32_t sectorStart, uint32_t sectorEnd);
uint32_t IAP_WriteRAMtoFLASH(uint32_t source_addr, uint32_t sector,
		uint32_t target_addr, uint32_t size);
uint32_t IAP_ReadFlashToRAM(uint32_t source_addr, uint32_t target_addr,
		uint32_t size);
uint32_t IAP_GetInitFlashPointer(uint32_t sector);

extern const uint32_t FSectorAddress[30];
#endif /* MFLASHMEMORY_H_ */
