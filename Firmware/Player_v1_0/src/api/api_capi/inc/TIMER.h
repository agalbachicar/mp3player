/*
 * TIMER.h
 *
 *  Created on: 25/06/2012
 *      Author: Alejandro
 */

#ifndef TIMER_H_
#define TIMER_H_

// que necesito de un timer?
// Primero una demora bloqueante, empecems por ahí.
// Inicializo un timer (vamos a poner el primero, así en cualquier plat se cual es)
// Lo pongo a una resolución de 1uS. Entonces entro a una función que hasta que el timer no hace algo no vuelvo.
// Que puedo hacer con un timer? bajar un flag en una interrupción y nada más? Y sí, por ahora sí.

void TIMERdelay (int us);
int TIMERpwm_start (int frequency_in_hertz, int duty_cycle_in_percentage, void (*callback_transition_to_inactive)(void), void(*callback_period_complete)(void));

#endif /* TIMER_H_ */
