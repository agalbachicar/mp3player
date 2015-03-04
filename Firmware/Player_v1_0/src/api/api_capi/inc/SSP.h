/*
 * SSP.h
 *
 *  Created on: 21/06/2011
 *      Author: Alejandro
 */

#ifndef SSP_H_
#define SSP_H_

#include "PERIPH.h"
#include "ColaCircular.h"



typedef enum _SSP_MODE {
	SPI_MASTER,
	SPI_SLAVE
} SSP_Mode;

typedef enum _CLOCK_PHASE {
	CPHA_FIRST_T,
	CPHA_SECOND_T
} SPI_Phase;

typedef enum _CLOCK_POLARITY {
	CPOL_HIGH,
	CPOL_LOW
} SPI_Polarity;

typedef struct SSPStruct {
	int periphName;
	ColaCircular colaTx;
	ColaCircular colaRx;
	bool TxEnCurso;
} SSP;

//void SSPInit(int periphName, SSP_Mode mode = SPI_MASTER, long bitRate = 1000000, int dataBits = 8, SPI_Polarity pol = CPOL_HIGH, SPI_Phase phase = CPHA_SECOND_T);
void 	SSPInit			(SSP * device, int periphName, SSP_Mode, long clockHz, int dataBits, SPI_Polarity, SPI_Phase);
short 	SSPgeti			(SSP * device);
void 	SSPputi			(SSP * device, short data);
short	SSPexchangei	(SSP * device, short data);	// En SPI se envia uno para recibir uno
bool 	SSPisBusy		(SSP * device);
bool 	SSPisTxFIFOFull (SSP * device);
bool 	SSPisRxFIFOEmpty(SSP * device);
void	SSPsetClock		(SSP * device, int clock);
void	SSPsetDataWidth	(SSP * device, int dataWidthBits);
void 	SSPvaciarFIFO	(SSP * device);

#endif /* SSP_H_ */
