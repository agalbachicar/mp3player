/*
 * debFlashMemory.c
 *
 *  Created on: 09/04/2012
 *  Author: Eze
 */

#include "debFlashMemory.h"
#include "stdint.h"
#include "system_LPC17xx.h"

const uint32_t FSectorAddress[30] = {0x00000000,0x00001000,0x00002000,0x00003000,0x00004000,
		0x00005000,0x00006000,0x00007000,0x00008000,0x00009000,0x0000A000,0x0000B000,0x0000C000,
		0x0000D000,0x0000E000,0x0000F000,0x00010000,0x00018000,0x00020000,0x00028000,0x00030000,
		0x00038000,0x00040000,0x00048000,0x00050000,0x00058000,0x00060000,0x00068000,0x00070000,
		0x00078000};

uint32_t* sector29StartAddress = (uint32_t*) FLASH_SECTOR_29;
PointerFunctionIAP IAP_Entry;

void IAP_Config ()
{
	IAP_Entry = (PointerFunctionIAP) IAP_LOCATION;
}
/** Blank check sector(s)
 *
 *  @param    start    a Start Sector Number
 *  @param    end      an End Sector Number (should be greater than or equal to start sector number).
 *  @return error code: CMD_SUCCESS | BUSY | SECTOR_NOT_BLANK | INVALID_SECTOR
 */
uint32_t IAP_BlankCheck (uint32_t sectorStart, uint32_t sectorEnd)
{
	uint32_t IAP_command[5];
	uint32_t IAP_result[5];

    IAP_command[0] = IAPCommand_Blank_check_sector;
    IAP_command[1] = sectorStart;  //  Start Sector Number
    IAP_command[2] = sectorEnd;    //  End Sector Number (should be greater than or equal to start sector number)

    IAP_Entry (IAP_command, IAP_result);

    return ( IAP_result[0] );
}

/** Erase Sector(s)
 *
 *  @param    start    a Start Sector Number
 *  @param    end      an End Sector Number (should be greater than or equal to start sector number).
 *  @return   error code: CMD_SUCCESS | BUSY | SECTOR_NOT_PREPARED_FOR_WRITE_OPERATION | INVALID_SECTOR
 */
uint32_t IAP_Erase (uint32_t start, uint32_t end)
{
	uint32_t IAP_command[5];
	uint32_t IAP_result[5];

    IAP_command[ 0 ]    = IAPCommand_Erase_sector;
    IAP_command[ 1 ]    = start;  //  Start Sector Number
    IAP_command[ 2 ]    = end;    //  End Sector Number (should be greater than or equal to start sector number)
    IAP_command[ 3 ]    = SystemCoreClock;             //  CPU Clock Frequency (CCLK) in kHz

    IAP_Entry( IAP_command, IAP_result );

    return ( IAP_result[0] );
}

/** Prepare sector(s) for write operation
 *
 *  @param    start    a Start Sector Number
 *  @param    end      an End Sector Number (should be greater than or equal to start sector number).
 *  @return   error code: CMD_SUCCESS | BUSY | INVALID_SECTOR
 */
uint32_t IAP_Prepare (uint32_t sectorStart, uint32_t sectorEnd)
{
	uint32_t IAP_command[5];
	uint32_t IAP_result[5];

	IAP_command[0] = IAPCommand_Prepare_sector_for_write_operation;
	IAP_command[1] = sectorStart; 	// Start Sector Number
	IAP_command[2] = sectorEnd; 	// End Sector Number (should be greater than or equal to start sector number)

	IAP_Entry (IAP_command, IAP_result);

	return ( IAP_result[0] );
}

uint32_t IAP_Clean (uint32_t sectorStart, uint32_t sectorEnd)
{
	uint32_t result;
	uint32_t i;

	for (i = sectorStart; i <= sectorEnd; i++)
	{
		result = IAP_BlankCheck (i, i);
		if (result == SECTOR_NOT_BLANK)
		{
			IAP_Prepare (i, i);
			IAP_Erase (i, i);
		}
	}
	return result;
}

/** Copy RAM to Flash. Can write up to 32KByte in 1 sector
 *
 *  @param    source_addr    Source RAM address from which data bytes are to be read. This address should be a word boundary.
 *  @param    target_addr    Destination flash address where data bytes are to be written. This address should be a 256 byte boundary.
 *  @param    size           Number of bytes to be written. Should be 256 | 512 | 1024 | 4096.
 *  @return   error code: CMD_SUCCESS | SRC_ADDR_ERROR (Address not a word boundary) | DST_ADDR_ERROR (Address not on correct boundary) | SRC_ADDR_NOT_MAPPED | DST_ADDR_NOT_MAPPED | COUNT_ERROR (Byte count is not 256 | 512 | 1024 | 4096) | SECTOR_NOT_PREPARED_FOR_WRITE_OPERATION | BUSY
 */
uint32_t IAP_WriteRAMtoFLASH (uint32_t source_addr, uint32_t sector, uint32_t target_addr, uint32_t sizeInBytes)
{
	uint8_t i;

	uint8_t numberOfBlocks_4096;
	uint8_t numberOfBlocks_1024;
	uint8_t numberOfBlocks_512;
	uint8_t numberOfBlocks_256;

	uint32_t IAP_command[5];
	uint32_t IAP_result[5];

	uint32_t targetAddress;
	uint32_t sourceAddress;

	uint32_t rtaPrepare;
	uint32_t rtaWrite;

	// TODO: Verificar la cuenta
	numberOfBlocks_4096 = sizeInBytes / 4096;
	numberOfBlocks_1024 = (sizeInBytes - numberOfBlocks_4096 * 4096) / 1024;
	numberOfBlocks_512 = (sizeInBytes - numberOfBlocks_4096 * 4096 - numberOfBlocks_1024 * 1024) / 512;
	numberOfBlocks_256 = (sizeInBytes - numberOfBlocks_4096 * 4096 - numberOfBlocks_1024 * 1024 - numberOfBlocks_512 * 512) / 256;
	if ( sizeInBytes - numberOfBlocks_4096 * 4096 - numberOfBlocks_1024 * 1024 - numberOfBlocks_512 * 512 - numberOfBlocks_256 * 256 > 0 )
	{
		(numberOfBlocks_256)++;
	}

	targetAddress = target_addr;
	sourceAddress = source_addr;

	for (i = 0; i < numberOfBlocks_4096; i++)
	{
		// Prepare the flash sector for writing operation
		rtaPrepare = IAP_Prepare (sector, sector);

		// Write a block of 4096 bytes
		IAP_command[0] = IAPCommand_Copy_RAM_to_Flash;
	    IAP_command[1] = targetAddress;					    	//  Destination flash address where data bytes are to be written. This address should be a 256 byte boundary.
	    IAP_command[2] = sourceAddress;						   	//  Source RAM address from which data bytes are to be read. This address should be a word boundary.
	    IAP_command[3] = 4096;        							//  Number of bytes to be written. Should be 256 | 512 | 1024 | 4096.
	    IAP_command[4] = SystemCoreClock / 1000;    			//  CPU Clock Frequency (CCLK) in kHz.
	    IAP_Entry (IAP_command, IAP_result);
	    rtaWrite = IAP_result[0];
		targetAddress = targetAddress + 4096;
		sourceAddress = sourceAddress + 4096;
	}

	for (i = 0; i < numberOfBlocks_1024; i++)
	{
		// Prepare the flash sector for writing operation
		rtaPrepare = IAP_Prepare (sector, sector);

		// Write a block of 1024 bytes
		IAP_command[0] = IAPCommand_Copy_RAM_to_Flash;
		IAP_command[1] = targetAddress;					    	//  Destination flash address where data bytes are to be written. This address should be a 256 byte boundary.
		IAP_command[2] = sourceAddress;						   	//  Source RAM address from which data bytes are to be read. This address should be a word boundary.
		IAP_command[3] = 1024;        							//  Number of bytes to be written. Should be 256 | 512 | 1024 | 4096.
		IAP_command[4] = SystemCoreClock / 1000;    			//  CPU Clock Frequency (CCLK) in kHz.
		IAP_Entry (IAP_command, IAP_result);
		rtaWrite = IAP_result[0];
		targetAddress = targetAddress + 1024;
		sourceAddress = sourceAddress + 1024;
	}

	for (i = 0; i < numberOfBlocks_512; i++)
	{
		// Prepare the flash sector for writing operation
		rtaPrepare = IAP_Prepare (sector, sector);

		// Write a block of 512 bytes
		IAP_command[0] = IAPCommand_Copy_RAM_to_Flash;
		IAP_command[1] = targetAddress;					    	//  Destination flash address where data bytes are to be written. This address should be a 256 byte boundary.
		IAP_command[2] = sourceAddress;						   	//  Source RAM address from which data bytes are to be read. This address should be a word boundary.
		IAP_command[3] = 512;        							//  Number of bytes to be written. Should be 256 | 512 | 1024 | 4096.
		IAP_command[4] = SystemCoreClock / 1000;    			//  CPU Clock Frequency (CCLK) in kHz.
		IAP_Entry (IAP_command, IAP_result);
		rtaWrite = IAP_result[0];
		if (rtaWrite != 0)
		{
			rtaWrite = 0;
		}
		targetAddress = targetAddress + 512;
		sourceAddress = sourceAddress + 512;
	}

	for (i = 0; i < numberOfBlocks_256; i++)
		{
		// Prepare the flash sector for writing operation
		rtaPrepare = IAP_Prepare (sector, sector);

		// Write a block of 256 bytes
		IAP_command[0] = IAPCommand_Copy_RAM_to_Flash;
		IAP_command[1] = targetAddress;					    	//  Destination flash address where data bytes are to be written. This address should be a 256 byte boundary.
		IAP_command[2] = sourceAddress;						   	//  Source RAM address from which data bytes are to be read. This address should be a word boundary.
		IAP_command[3] = 256;        							//  Number of bytes to be written. Should be 256 | 512 | 1024 | 4096.
		IAP_command[4] = SystemCoreClock / 1000;    			//  CPU Clock Frequency (CCLK) in kHz.
		IAP_Entry (IAP_command, IAP_result);
		rtaWrite = IAP_result[0];
		targetAddress = targetAddress + 256;
		sourceAddress = sourceAddress + 256;
	}

    return ( 1 ); // FIXME: Poner mensaje de error
}

/**
 * \fn IAP_ReadFlashToRAM
 * \brief	Copia el archivo al buffer de RAM
 * \param	source_addr	Es la direccion de memoria en FLASH desde donde se desea leer
 * \param	target_addr	Es la direccion de memoria en RAM a donde se desea leer
 * \param	size	Es el tamaño en bytes a copiar
 * \retval	1	Si la lectura es correcta
 * 			0	Si la lectura es erronea
 */
uint32_t IAP_ReadFlashToRAM (uint32_t source_addr, uint32_t target_addr, uint32_t size)
{
	uint32_t i;

	//Me protejo contra una lectura en una zona incorrecta de memora
	if ((source_addr + size) >= DEBFLASH_MAX_SOURCE_ADDRESS || target_addr == 0)
		return 0;
	//Leo los bytes
	for (i = 0; i < size; i++)
		*((uint8_t*)(target_addr + i)) = *((uint8_t*)(source_addr + i));

	return 1;
}

/**
 * \fn IAP_GetInitFlashPointer
 * \brief	Retorna el puntero de inicio del sector de la FLASH
 * \param	sector	Es el sector que se requiere el puntero
 * \retval	El puntero o NULL si el sector es incorrecto
 */
uint32_t IAP_GetInitFlashPointer(uint32_t sector)
{
	//Obtengo la direccion de inicio de sector
	switch(sector)
	{
		case 23:
			return FLASH_SECTOR_23;
			break;
		case 24:
			return FLASH_SECTOR_24;
			break;
		case 25:
			return FLASH_SECTOR_25;
			break;
		case 26:
			return FLASH_SECTOR_26;
			break;
		case 27:
			return FLASH_SECTOR_27;
			break;
		case 28:
			return FLASH_SECTOR_28;
			break;
		case 29:
			return FLASH_SECTOR_29;
			break;
		default:
			return NULL;
			break;
	}
}
