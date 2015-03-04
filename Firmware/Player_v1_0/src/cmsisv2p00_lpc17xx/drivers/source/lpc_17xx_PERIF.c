/*
 * PERIF.c
 *
 *  Created on: 21/06/2011
 *      Author: Alejandro
 *
 *  Notas:
 *  Las UART estan fijadas a una opcion de pines, se podria agregar
 *  	funcionalidad para que se las pueda inicializar en alguno de los otros.
 *  El ADC no inicializa ningun PIN ya que pueden ser varios, lo hace AIN_INIT.
 */

#include "PIN.h"
#include "PERIPH.h"
#include "lpc_176X_PinNames.h"
#include "lpc_176X_PeriphNames.h"
#include "lpc17xx_clkpwr.h"

void PERIPH_Init(int per) {
	switch(per){

	case NO_PERIPHERAL: // Para detectar errores
		break;

	case UART_0:
		CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCUART0, ENABLE);
		CLKPWR_SetPCLKDiv(CLKPWR_PCLKSEL_UART0, CLKPWR_PCLKSEL_CCLK_DIV_1);
		PIN_Init(TxD0__0_2);
		PIN_Init(RxD0__0_3);
	break;

	case UART_1:
		CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCUART1, ENABLE);
		CLKPWR_SetPCLKDiv(CLKPWR_PCLKSEL_UART1, CLKPWR_PCLKSEL_CCLK_DIV_1);
		PIN_Init(TxD1__0_15);
		PIN_Init(RxD1__0_16);
		break;

	case UART_2:
		CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCUART2, ENABLE);
		CLKPWR_SetPCLKDiv(CLKPWR_PCLKSEL_UART2, CLKPWR_PCLKSEL_CCLK_DIV_1);
		PIN_Init(TxD2__0_10);
		PIN_Init(RxD2__0_11);
		break;

	case UART_3:
		CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCUART3, ENABLE);
		CLKPWR_SetPCLKDiv(CLKPWR_PCLKSEL_UART3, CLKPWR_PCLKSEL_CCLK_DIV_1);
		PIN_Init(TxD3__0_0);
		PIN_Init(RxD3__0_1);
		break;

	case SSP_0:
		CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCSSP0, ENABLE);
		CLKPWR_SetPCLKDiv(CLKPWR_PCLKSEL_SSP0, CLKPWR_PCLKSEL_CCLK_DIV_2);
		PIN_Init(MOSI0__0_18);
		PIN_Init(MISO0__0_17);
		PIN_Init(SCLK0__0_15);
		PIN_Init(SSEL0__0_16);
		break;

	case SSP_1:
		CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCSSP1, ENABLE);
		CLKPWR_SetPCLKDiv(CLKPWR_PCLKSEL_SSP1, CLKPWR_PCLKSEL_CCLK_DIV_2);
		PIN_Init(MOSI1__0_9);
		PIN_Init(MISO1__0_8);
		PIN_Init(SCLK1__0_7);
		PIN_Init(SSEL1__0_6);
		break;

	case ADC_0:
		CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCAD, ENABLE);
		CLKPWR_SetPCLKDiv(CLKPWR_PCLKSEL_ADC, CLKPWR_PCLKSEL_CCLK_DIV_1);
		break;

	}
}
