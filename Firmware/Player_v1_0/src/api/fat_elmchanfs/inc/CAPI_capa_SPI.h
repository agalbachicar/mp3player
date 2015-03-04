#ifndef __LPC17XX_SPI_H__
#define __LPC17XX_SPI_H__
#include <stdint.h>

void 	CAPI_SPI_DeInit		(void); //FIXME: Agregaselo a CAPI!
void 	CAPI_SPI_Init		(void);
void 	CAPI_SPI_Release	(void);
void 	CAPI_SPI_Select 	(void);
void 	CAPI_SPI_DeSelect 	(void);
void	CAPI_SPI_SendByte	(uint8_t data);
uint8_t	CAPI_SPI_RecvByte	(void);

#endif	// __LPC17XX_SPI_H__
