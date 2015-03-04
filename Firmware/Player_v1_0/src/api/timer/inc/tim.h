/**
 * @author	Agustin Alba Chicar
 * @file	timer.h
 * @brief	Es una API de manejo de los timers para poder trabajarlos con una capa de abstracción.
 * @date	4/10/2014
 * @version	1.0
 * @todo
 * @bug
 */

#ifndef TIMER_H
#define	TIMER_H

/**
 * @note        Includes
 */

//! @note Standard Includes
#include <stdint.h>

//! @note CMSIS Includes
#include "lpc17xx_timer.h"


/**
 * @note        Defines
 */
#ifndef ERR
        #define ERR     ((int32_t)-1)
#endif

#define	TIMER_COUNT		4

/**
 * @note        Tipos de datos, estructuras, typdefs
 */

/**
 * @enum ETimerMode
 * @brief Modo de funcionamiento de los timers
 */
typedef enum EnumTimerMode
{
	FREE_RUNNING = 0,			//! Modo simple, corre el timer sin generar una interrupción.
	INTERRUPT_REPETITIVE = 1,	//! Modo repetitivo por interrupciones.
	INTERRUPT_ONE_SHOT = 2		//! Modo de disparo único por interrupciones.
} ETimerMode;

/**
 * @note        Prototipos de funciones publicas
 */
void timerConfig(uint8_t timerId, ETimerMode mode, uint32_t timeInUSec);
void timerConfigCallback(uint8_t timerId, void (*timerHandler) (void));
void timerRun(uint8_t timerId);
void timerStop(uint8_t timerId);

/**
 * @note        Prototipos de funciones privadas
 */
void TIMER0_IRQHandler (void);
void TIMER1_IRQHandler (void);
void TIMER2_IRQHandler (void);
void TIMER3_IRQHandler (void);

/**
 * @note        Variables publicas
 */

#endif	/* TEMPLATE_H */
