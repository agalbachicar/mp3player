/**
 * @author		Agustin Alba Chicar
 * @file		timer.c
 * @brief		Implementacion de la API de timers
 * @date		4/10/2014
 * @version		1.0
 * @todo
 * @bug
 */

/**
 * @note Includes
 */
#include <tim.h>

/**
 * @note Variables globales
 */

/**
 * @note Funciones publicas
 */

/**
 * @fn timerModes
 * @brief Modos de configuracion de los timers
 */
static ETimerMode timerModes[TIMER_COUNT] = {FREE_RUNNING, FREE_RUNNING, FREE_RUNNING, FREE_RUNNING};

/**
 * @fn timerCallbacks
 * @brief Punteros a funcion a los callbacks de los eventos de los timers
 */
static void (*timerCallbacks[TIMER_COUNT])(void) = {NULL, NULL, NULL, NULL};


/**
 * @fn timerConfig
 * @brief Configuro el timer con el modo y la periodicidad
 * @param timerId
 * @param mode
 * @param timerInUSec
 */
void timerConfig(uint8_t timerId, ETimerMode mode, uint32_t timeInUSec)
{
	//Estructuras de configuracion del timer
	TIM_TIMERCFG_Type timerCfg;
	//Estructura de configuracion del match
	TIM_MATCHCFG_Type matchCfg;

	//Evaluo el id del timer
	if(timerId >= TIMER_COUNT)
	{
		return;
	}

	//Configuracion del Timer
	timerCfg.PrescaleOption = TIM_PRESCALE_USVAL;
	timerCfg.PrescaleValue = 1;
	//Configuracion del match
	matchCfg.MatchChannel = 0;
	matchCfg.MatchValue = timeInUSec;
	//Configuro el modo de interrupciones, reset on match y lo que se hace
	switch(mode)
	{
		case FREE_RUNNING:
			matchCfg.IntOnMatch = DISABLE;
			matchCfg.ResetOnMatch = ENABLE;
			matchCfg.StopOnMatch = DISABLE;
			matchCfg.ExtMatchOutputType = TIM_EXTMATCH_NOTHING;
			break;
		case INTERRUPT_REPETITIVE:
			matchCfg.IntOnMatch = ENABLE;
			matchCfg.ResetOnMatch = ENABLE;
			matchCfg.StopOnMatch = DISABLE;
			matchCfg.ExtMatchOutputType = TIM_EXTMATCH_NOTHING;
			break;
		case INTERRUPT_ONE_SHOT:
			matchCfg.IntOnMatch = ENABLE;
			matchCfg.ResetOnMatch = ENABLE;
			matchCfg.StopOnMatch = ENABLE;
			matchCfg.ExtMatchOutputType = TIM_EXTMATCH_NOTHING;
			break;
		default:
			return;
			break;
	}
	timerModes[timerId] = mode;
	//Configuro los timers
	switch(timerId)
	{
		case 0:
			TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &timerCfg);
			TIM_ConfigMatch(LPC_TIM0, &matchCfg);
			break;
		case 1:
			TIM_Init(LPC_TIM1, TIM_TIMER_MODE, &timerCfg);
			TIM_ConfigMatch(LPC_TIM1, &matchCfg);
			break;
		case 2:
			TIM_Init(LPC_TIM2, TIM_TIMER_MODE, &timerCfg);
			TIM_ConfigMatch(LPC_TIM2, &matchCfg);
			break;
		case 3:
			TIM_Init(LPC_TIM3, TIM_TIMER_MODE, &timerCfg);
			TIM_ConfigMatch(LPC_TIM3, &matchCfg);
			break;
	}
}

/**
 * @fn timerConfigCallback
 * @brief Configuro el callback del timer que se emite en la interrupcion.
 * @param timerId
 * @param timerHandler
 */
void timerConfigCallback(uint8_t timerId, void (*timerHandler) (void))
{
	//Evaluo el callback
	if(timerId >= TIMER_COUNT)
	{
		return;
	}
	//Cargo el callback
	timerCallbacks[timerId] = timerHandler;
}

/**
 * @fn timerRun
 * @brief Inicio del timer y de las interrupciones si para eso fue configurado
 * @param timerId
 */
void timerRun(uint8_t timerId)
{
	LPC_TIM_TypeDef *pTimer;
	//Evaluo el callback
	if(timerId >= TIMER_COUNT)
	{
		return;
	}
	//Evaluo el timerId
	switch(timerId)
	{
		case 0:
			pTimer = LPC_TIM0;
			break;
		case 1:
			pTimer = LPC_TIM1;
			break;
		case 2:
			pTimer = LPC_TIM2;
			break;
		case 3:
			pTimer = LPC_TIM3;
			break;
	}
	//Configuro las interrupciones
	if(timerModes[timerId] == INTERRUPT_REPETITIVE || timerModes[timerId] == INTERRUPT_ONE_SHOT)
	{
		NVIC_EnableIRQ(TIMER0_IRQn + timerId);
	}
	//Arranco el timer
	TIM_Cmd(pTimer, ENABLE);
}

/**
 * @fn timerStop
 * @brief Para el timer determinado
 * @param timerId
 */
void timerStop(uint8_t timerId)
{
	//Evaluo el callback
	if(timerId >= TIMER_COUNT)
	{
		return;
	}
	//Freno el timer
	switch(timerId)
	{
		case 0:
			TIM_Cmd(LPC_TIM0, ENABLE);
			break;
		case 1:
			TIM_Cmd(LPC_TIM1, ENABLE);
			break;
		case 2:
			TIM_Cmd(LPC_TIM2, ENABLE);
			break;
		case 3:
			TIM_Cmd(LPC_TIM3, ENABLE);
			break;
	}
}


/**
 * @note Funciones Privadas
 */

/**
 * @fn TIMER0_IRQHandler
 * @brief Handler de interrupcion del timer 0. Emito el evento si esta cargado
 */
void TIMER0_IRQHandler (void)
{
	//Evaluo la interrupcion
	if(TIM_GetIntStatus(LPC_TIM0, 0) == SET)
	{
		if(timerCallbacks[0] != NULL)
		{
			timerCallbacks[0]();
		}
	}
	//Borro las interrupciones pendientes
	TIM_ClearIntPending(LPC_TIM0, 0);
}
/**
 * @fn TIMER1_IRQHandler
 * @brief Handler de interrupcion del timer 1. Emito el evento si esta cargado
 */
void TIMER1_IRQHandler (void)
{
	//Evaluo la interrupcion
	if(TIM_GetIntStatus(LPC_TIM1, 0) == SET)
	{
		if(timerCallbacks[1] != NULL)
		{
			timerCallbacks[1]();
		}
	}
	//Borro las interrupciones pendientes
	TIM_ClearIntPending(LPC_TIM1, 0);
}
/**
 * @fn TIMER2_IRQHandler
 * @brief Handler de interrupcion del timer 2. Emito el evento si esta cargado
 */
void TIMER2_IRQHandler (void)
{
	//Evaluo la interrupcion
	if(TIM_GetIntStatus(LPC_TIM2, 0) == SET)
	{
		if(timerCallbacks[2] != NULL)
		{
			timerCallbacks[2]();
		}
	}
	//Borro las interrupciones pendientes
	TIM_ClearIntPending(LPC_TIM2, 0);
}
/**
 * @fn TIMER3_IRQHandler
 * @brief Handler de interrupcion del timer 3. Emito el evento si esta cargado
 */
void TIMER3_IRQHandler (void)
{
	//Evaluo la interrupcion
	if(TIM_GetIntStatus(LPC_TIM3, 0) == SET)
	{
		if(timerCallbacks[3] != NULL)
		{
			timerCallbacks[3]();
		}
	}
	//Borro las interrupciones pendientes
	TIM_ClearIntPending(LPC_TIM3, 0);
}
