/*
 * AIN.h
 *
 *  Created on: 18/03/2012
 *      Author: Alejandro
 */

#ifndef AIN_H_
#define AIN_H_

#include "CAPI_Definitions.h"

typedef struct _AIN {
	int pinName;
}AIN;

void AIN_Init			(AIN *, int pinName);
int	 AINread			(AIN *);

#endif /* AIN_H_ */
