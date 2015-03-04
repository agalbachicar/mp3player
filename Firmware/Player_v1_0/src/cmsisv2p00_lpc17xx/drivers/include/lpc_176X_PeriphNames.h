/*
 * PinNames.h
 *
 *  Created on: 19/06/2011
 *      Author: Alejandro
 */

#ifndef LPC_1768_PERIPHNAMES_H_
#define LPC_1768_PERIPHNAMES_H_

// Nombres de los Perifericos
// Los perifericos de la misma familia deben quedar consecutivos (ej, UART_0, UART_1, UART_N)
// Se depende de esto en algunos drivers
enum _periph_names {
	NO_PERIPHERAL = 0,
	UART_0,
	UART_1,
	UART_2,
	UART_3,
	SSP_0,
	SSP_1,
	PWM_0,
	ADC_0
};

typedef enum _periph_names PeriphName;

#endif /* LPC_1768_PERIPHNAMES_H_ */
