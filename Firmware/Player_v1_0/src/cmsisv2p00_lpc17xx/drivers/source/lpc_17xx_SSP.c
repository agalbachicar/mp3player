/*
 * lpc_17xx_SSP.c
 *
 *  Created on: 22/06/2011
 *      Author: Alejandro
 */

#include "PERIPH.h"
#include "SSP.h"
#include "lpc17xx_ssp.h"
#include "lpc_176X_PeriphNames.h"

static LPC_SSP_TypeDef* getSspPtr(SSP * periph);

static char * SSPgetTxBuffer 		(SSP * device);
static char * SSPgetRxBuffer 		(SSP * device);

#define N_SSPS 2
SSP * SSPvector[N_SSPS]; // FIXME: Kludge! Arreglalo, esta dependiendo de la aplicacion!
#define L_SSP_FIFO_TX 100				// Tamanio hard-coded en tiempo de compilacion
#define L_SSP_FIFO_RX 100				// Tamanio hard-coded en tiempo de compilacion
char SSP_TxBuffer[L_SSP_FIFO_TX][N_SSPS];	// Buffer alocado estaticamente
char SSP_RxBuffer[L_SSP_FIFO_RX][N_SSPS];	// Buffer alocado estaticamente


void SSPInit(SSP * device, int periphName, SSP_Mode mode, long clockHz, int dataBits, SPI_Polarity pol, SPI_Phase phase)
{
	device->TxEnCurso = false;
	device->periphName = periphName;
	SSPvector[periphName - SSP_0] = device;

	PERIPH_Init(periphName);
	LPC_SSP_TypeDef 	*pPER;
	SSP_CFG_Type		perConf;

	SSP_ConfigStructInit(&perConf);

	perConf.ClockRate = clockHz;

	switch(dataBits){
	case  5: perConf.Databit = SSP_DATABIT_5;  break;
	case  6: perConf.Databit = SSP_DATABIT_6;  break;
	case  7: perConf.Databit = SSP_DATABIT_7;  break;
	case  8: perConf.Databit = SSP_DATABIT_8;  break;
	case  9: perConf.Databit = SSP_DATABIT_9;  break;
	case 10: perConf.Databit = SSP_DATABIT_10; break;
	case 11: perConf.Databit = SSP_DATABIT_11; break;
	case 12: perConf.Databit = SSP_DATABIT_12; break;
	case 13: perConf.Databit = SSP_DATABIT_13; break;
	case 14: perConf.Databit = SSP_DATABIT_14; break;
	case 15: perConf.Databit = SSP_DATABIT_15; break;
	case 16: perConf.Databit = SSP_DATABIT_16; break;
	}

	switch (mode) {
	case SPI_MASTER: 	perConf.Mode =	SSP_MASTER_MODE;break;
	case SPI_SLAVE:		perConf.Mode =	SSP_SLAVE_MODE;	break;
	}

	switch(pol){
	case CPOL_HIGH:		perConf.CPOL =	SSP_CPOL_HI;	break;
	case CPOL_LOW:		perConf.CPOL =	SSP_CPOL_LO;	break;
	}

	switch(phase){
	case CPHA_FIRST_T:	perConf.CPHA =	SSP_CPHA_FIRST;	break;
	case CPHA_SECOND_T:	perConf.CPHA =	SSP_CPHA_SECOND;break;
	}

	ColaCircular_Init(&device->colaTx, SSPgetTxBuffer(device), L_SSP_FIFO_TX);
	ColaCircular_Init(&device->colaRx, SSPgetRxBuffer(device), L_SSP_FIFO_RX);

	device->periphName = periphName;
	pPER = getSspPtr(device);
	SSP_Init(pPER, &perConf);
	SSP_Cmd(pPER, ENABLE);
}

//-----------------------------------------------------------------------------

short SSPexchangei(SSP * device, short data){
	LPC_SSP_TypeDef *pPER = getSspPtr(device);
	SSP_SendData(pPER, data);
	while(SSPisBusy(device))
		;
	return SSP_ReceiveData(pPER);
}

short SSPgeti(SSP * device)
{
	return SSP_ReceiveData(getSspPtr(device));
}

void SSPputi(SSP * device, short data)
{
	SSP_SendData(getSspPtr(device), data);
}

//-----------------------------------------------------------------------------

bool SSPisBusy	(SSP * device)
{
	return SSP_GetStatus(getSspPtr (device), SSP_STAT_BUSY);
}

bool SSPisTxFIFOFull (SSP * device)
{
	return !SSP_GetStatus(getSspPtr (device), SSP_STAT_TXFIFO_NOTFULL);
}

bool SSPisRxFIFOEmpty (SSP * device)
{
	return !SSP_GetStatus(getSspPtr (device), SSP_STAT_RXFIFO_NOTEMPTY);
}

void SSPsetClock (SSP * device, int target_clock){
	SSP_SetClock (getSspPtr (device), target_clock);
}

void SSPsetDataWidth (SSP * device, int dataWidthBits)
{
	LPC_SSP_TypeDef * pPtr = getSspPtr (device);
	pPtr->CR0 &= ~SSP_CR0_DSS(16);				//Bajo la mascara de 16 bits
	pPtr->CR0 |= ~SSP_CR0_DSS(dataWidthBits);	//Levanto los bits que me digan
}

//-----------------------------------------------------------------------------

static LPC_SSP_TypeDef* getSspPtr(SSP * periph){
	LPC_SSP_TypeDef* pPER;
	switch(periph->periphName){
	case SSP_0: pPER = LPC_SSP0; break;
	case SSP_1: pPER = LPC_SSP1; break;
	default:	pPER = (LPC_SSP_TypeDef*) NULL;	break;
	}
	return pPER;
}

static char * SSPgetTxBuffer (SSP * device)
{
	int n_SSP = device->periphName - SSP_0;
	return &SSP_TxBuffer[0][n_SSP];
}

static char * SSPgetRxBuffer (SSP * device)
{
	int n_SSP = device->periphName - SSP_0;
	return &SSP_RxBuffer[0][n_SSP];
}

void SSPvaciarFIFO (SSP * device)
{
	LPC_SSP_TypeDef * pPer = getSspPtr(device);
	while( SSP_GetStatus(pPer, SSP_STAT_RXFIFO_NOTEMPTY))
	{
		volatile uint32_t dummy = pPer->DR;
		(void)dummy;
	}
}
