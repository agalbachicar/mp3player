/*
 * lpc_17xx_DigitalOut.cpp
 *
 *  Created on: 20/06/2011
 *      Author: Alejandro
 */

#include "PERIPH.h"
#include "UART.h"
#include "ColaCircular.h"
#include "lpc_176X_PeriphNames.h"
#include "lpc17xx_uart.h"

static LPC_UART_TypeDef* getUartPtr	(UART * uart);
static int 	getIRQNumber			(UART * uart);
static void UART_ISR				(UART * uart);

static void UARTInit_Line			(UART * uart, long baudRate, int dataBits, char parity, int stopBits);
static void UARTInit_FIFOs			(UART * uart);
static void UARTInit_Interrupts		(UART * uart);
static char * UARTgetTxBuffer 		(UART * uart);
static char * UARTgetRxBuffer 		(UART * uart);

#define N_UARTS 4
UART * UARTvector[N_UARTS]; // FIXME: Kludge! Arreglalo, esta dependiendo de la aplicacion!
#define L_UART_FIFO_TX 1000				// Tamanio hard-coded en tiempo de compilacion
#define L_UART_FIFO_RX 1000				// Tamanio hard-coded en tiempo de compilacion
char UART_TxBuffer[L_UART_FIFO_TX][N_UARTS];	// Buffer alocado estaticamente
char UART_RxBuffer[L_UART_FIFO_RX][N_UARTS];	// Buffer alocado estaticamente

void UARTInit(UART * uart, int periphName, long baudRate, int dataBits, char parity, int stopBits)
{
	uart->TxEnCurso = false;
	uart->periphName = periphName;
	UARTvector[periphName - UART_0] = uart;

	PERIPH_Init(periphName);

	UARTInit_Line		(uart, baudRate, dataBits, parity, stopBits);
	UARTInit_FIFOs		(uart);
	UARTInit_Interrupts	(uart);
	UART_TxCmd(getUartPtr(uart), ENABLE);
}

void UARTDeInit(UART * uart)
{
	UART_DeInit(getUartPtr(uart));
}

static void UARTInit_Line(UART * uart, long baudRate, int dataBits, char parity, int stopBits)
{
	UART_CFG_Type uartConf;

	uartConf.Baud_rate = baudRate;

	switch(dataBits)
	{
		case 5: uartConf.Databits = UART_DATABIT_5; break;
		case 6: uartConf.Databits = UART_DATABIT_6; break;
		case 7: uartConf.Databits = UART_DATABIT_7; break;
		case 8: uartConf.Databits = UART_DATABIT_8; break;
	}

	switch(parity)
	{
		case 'N':
		case 'n': uartConf.Parity = UART_PARITY_NONE; break;
		case 'E':
		case 'e': uartConf.Parity = UART_PARITY_EVEN; break;
		case 'O':
		case 'o': uartConf.Parity = UART_PARITY_ODD;  break;
		case '1': uartConf.Parity = UART_PARITY_SP_1; break;
		case '0': uartConf.Parity = UART_PARITY_SP_0; break;
	}

	switch(stopBits)
	{
		case 1: uartConf.Stopbits = UART_STOPBIT_1; break;
		case 2: uartConf.Stopbits = UART_STOPBIT_2; break;
	}

	UART_Init(getUartPtr(uart), &uartConf);
}

static void UARTInit_FIFOs(UART * uart)
{
	/* Initialize FIFOConfigStruct to default state:
	 * - FIFO_DMAMode 		= DISABLE
	 * - FIFO_Level 		= UART_FIFO_TRGLEV0
	 * - FIFO_ResetRxBuf 	= ENABLE
	 * - FIFO_ResetTxBuf 	= ENABLE
	 * - FIFO_State 		= ENABLE
	 */
	UART_FIFO_CFG_Type	fifoConf;
	UART_FIFOConfigStructInit(&fifoConf);
	UART_FIFOConfig(getUartPtr(uart), &fifoConf);

	ColaCircular_Init(&uart->colaTx, UARTgetTxBuffer(uart), L_UART_FIFO_TX);
	ColaCircular_Init(&uart->colaRx, UARTgetRxBuffer(uart), L_UART_FIFO_RX);
}

static char * UARTgetTxBuffer (UART * uart)
{
	int n_uart = uart->periphName - UART_0;
	return &UART_TxBuffer[0][n_uart];
}

static char * UARTgetRxBuffer (UART * uart)
{
	int n_uart = uart->periphName - UART_0;
	return &UART_RxBuffer[0][n_uart];
}

static void UARTInit_Interrupts	(UART * uart)
{
	UART_IntConfig(getUartPtr(uart), UART_INTCFG_RBR, ENABLE);
	UART_IntConfig(getUartPtr(uart), UART_INTCFG_THRE, ENABLE);
	NVIC_EnableIRQ(getIRQNumber(uart));
}

static int getIRQNumber(UART * uart)
{
	//Los hago con switch, no me arriesgo a que en el controlador no esten contiguas
	int IRQNumber;
	switch(uart->periphName){
		case UART_0: IRQNumber = UART0_IRQn; break;
		case UART_1: IRQNumber = UART1_IRQn; break;
		case UART_2: IRQNumber = UART2_IRQn; break;
		case UART_3: IRQNumber = UART3_IRQn; break;
	}
	return IRQNumber;
}

res_t UARTputchar(UART * uart, char c)
{
	UART_Send(getUartPtr(uart), &c, 1, NONE_BLOCKING);
	return CAPI_EXITO;
}

char UARTgetchar(UART * uart)
{
	char c;
	uint32_t result =  UART_Receive(getUartPtr(uart), &c, 1, NONE_BLOCKING);
	if(result == 0)
	{
		return 0;
	}
	return c;
}

static LPC_UART_TypeDef * getUartPtr(UART * uart)
{
	LPC_UART_TypeDef* uartPtr;
	switch(uart->periphName){
	case UART_0: uartPtr = (LPC_UART_TypeDef*) LPC_UART0; break;
	case UART_1: uartPtr = (LPC_UART_TypeDef*) LPC_UART1; break;
	case UART_2: uartPtr = LPC_UART2; break;
	case UART_3: uartPtr = LPC_UART3; break;
	default:	 uartPtr = (LPC_UART_TypeDef*) NULL; break;
	}
	return uartPtr;
}

bool UARTbusy (UART * uart)
{
	return UART_CheckBusy(getUartPtr(uart));
}

#if 1
void UART0_IRQHandler(void)
{
	UART_ISR(UARTvector[0]);
}
void UART1_IRQHandler(void)
{
	UART_ISR(UARTvector[1]);
}
void UART2_IRQHandler(void)
{
	UART_ISR(UARTvector[2]);
}

void UART3_IRQHandler(void)
{
	UART_ISR(UARTvector[3]);
}
#endif

static void UART_ISR (UART * uart)
{
	LPC_UART_TypeDef * uartPtr = getUartPtr(uart);
	int interruptSource = (uartPtr->IIR & 0x03CF) & UART_IIR_INTID_MASK; // Check
	switch(interruptSource)
	{
#if 0	// Falta implementar - Line status interrupt
		case UART_IIR_INTID_RLS:
			if(UARTgetError(uartPtr)){
				UART_IntErr(tmp1);
			}
		break;
#endif
		case UART_IIR_INTID_RDA:
		case UART_IIR_INTID_CTI:
			UART_IntRecepcion(uart);
			break;
		case UART_IIR_INTID_THRE:
			UART_IntTransmision(uart);
			break;
	}
}

void UART_IntTransmision(UART * uart)
{
	ColaCircular * out = &uart->colaTx;
	if(!ColaVacia(out)){
		UARTputchar(uart, ColaSacar(out));
		uart->TxEnCurso = true;
	} else {
		//TODO No me va a quedar la interrupcion prendida?
		uart->TxEnCurso = false;
	}
}

void UART_IntRecepcion(UART * uart)
{
	ColaCircular * in = &uart->colaRx;
	cola_t dato = UARTgetchar(uart);
	if(!ColaLlena(in)){
		ColaPoner(in, dato);
	} else {
		//TODO Manejar error de desborde de cola!
	}
}

#if 0
bool UARTgetError(LPC_UART_TypeDef * uartPtr)
{
	int error = UART_GetLineStatus(uartPtr);
	// Mask out the Receive Ready and Transmit Holding empty status
	error &= (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE | UART_LSR_BI | UART_LSR_RXFE);
	return error;
}
#endif
