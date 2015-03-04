/*
 * lpc_17xx_AIN.c
 *
 *  Created on: 20/06/2011
 *      Author: Alejandro
 */

#include "CAPI_Definitions.h"
#include "PERIPH.h"
#include "AIN.h"
#include "lpc_176X_PinNames.h"
#include "lpc_176X_PeriphNames.h"
#include "lpc17xx_adc.h"

static LPC_ADC_TypeDef * 	getParentPeripheralPointer	(int pinName);
static int					getParentPeripheralName		(int pinName);
static bool 				PinPerteneceADC0			(int pinName);
static bool 				isParentConfigured			(LPC_ADC_TypeDef * parentPtr);
static void 				DeshabilitarCanales			(LPC_ADC_TypeDef * ADCPtr);
static int					getCanal					(int pinName);

static bool ADC0Configurado = false;

void AIN_Init			(AIN * ain, int pinName)
{
	LPC_ADC_TypeDef * parentPtr = getParentPeripheralPointer(pinName);
	int parentName = getParentPeripheralName(pinName);

	if(!isParentConfigured(parentPtr)){
		PERIPH_Init(parentName);
		ADC_Init(parentPtr, 200000); // ADC conversion rate = 200Khz
		ADC0Configurado = true;
	}

	PIN_Init(pinName);
	ain->pinName = pinName;
}

static LPC_ADC_TypeDef * getParentPeripheralPointer(int pinName){
	return PinPerteneceADC0(pinName) ? LPC_ADC : NULL;
}

static int getParentPeripheralName(int pinName){
	return PinPerteneceADC0(pinName) ? (int) ADC_0 : NO_PERIPHERAL;
}

static bool PinPerteneceADC0(int pin)
{
	bool pertenece;
	switch(pin){
		case AD0_0__0_23:
		case AD0_1__0_24:
		case AD0_2__0_25:
		case AD0_3__0_26:
		case AD0_4__1_30:
		case AD0_5__1_31:
		case AD0_6__0_3:
		case AD0_7__0_2:	pertenece = true; break;
		default:			pertenece = false; break;
	}
	return pertenece;
}

static bool isParentConfigured(LPC_ADC_TypeDef * adcPtr){
// Atento, la unica manera de chequear es si esta prendido. Si alguien lo apago para ahorrar me trae problemas.
	return ADC0Configurado;
}

int	AINread (AIN * ain)
{
	LPC_ADC_TypeDef * parentPtr = getParentPeripheralPointer(ain->pinName);
	int canal = getCanal(ain->pinName);

	DeshabilitarCanales(parentPtr);
	ADC_ChannelCmd(parentPtr, canal, ENABLE);

	ADC_StartCmd(parentPtr, ADC_START_NOW);
	while (!(ADC_ChannelGetStatus(parentPtr, canal, ADC_DATA_DONE))){
		// Espero el fin de la conversion. A 200KHz son 5 uS.
	}

	return (int) ADC_ChannelGetData(parentPtr, canal);
}

static void DeshabilitarCanales(LPC_ADC_TypeDef * ADCPtr){
#define MASCARA_CANALES_ADC 0x000000FF
	ADCPtr->ADCR &= ~MASCARA_CANALES_ADC;
}

static int getCanal(int pin){
	int canal;
	switch(pin){
	case AD0_0__0_23:	canal = 0; break;
	case AD0_1__0_24:	canal = 1; break;
	case AD0_2__0_25:	canal = 2; break;
	case AD0_3__0_26:	canal = 3; break;
	case AD0_4__1_30:	canal = 4; break;
	case AD0_5__1_31:	canal = 5; break;
	case AD0_6__0_3:	canal = 6; break;
	case AD0_7__0_2:	canal = 7; break;
	default:		canal = -1; break;
	}
	return canal;
}
