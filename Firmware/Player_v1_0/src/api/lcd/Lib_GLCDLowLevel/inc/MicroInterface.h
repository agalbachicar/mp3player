
// ********************************
// Microcontroller Interface (Head)
// ********************************

#ifndef _MICROINTERFACE_H_
#define _MICROINTERFACE_H_

#include <stdint.h>

// ********************************
// LPC pins assigned to LCD WG12864
// ********************************

#define LCD_CSEL_PORT		2
#define CSEL1				12		// CS1 Bit (swap pin assignments with CSEL2 if left/right image is reversed)
#define CSEL2				11		// CS2 Bit

#define	LCD_DI_PORT			0
#define D_I					4		// D/I Bit

#define	LCD_RW_PORT			0
#define R_W					5		// R/W Bit

#define LCD_EN_PORT			2
#define EN					8		// EN Bit

#define LCD_DATA_PORT		2		//Puerto 2


// ******************************
// LPC digital I/O pins Functions
// ******************************
inline void MicroInterface_Init ();
inline void MicroInterface_Bit_WriteLow (uint8_t port, uint8_t pin);
inline void MicroInterface_Bit_WriteHigh (uint8_t port, uint8_t pin);
inline void MicroInterface_Byte_WriteData (uint8_t buf);

// ******
// Macros
// ******
#define EN_DELAY() for (_delayCounter=0; _delayCounter<10; _delayCounter++);

#endif
