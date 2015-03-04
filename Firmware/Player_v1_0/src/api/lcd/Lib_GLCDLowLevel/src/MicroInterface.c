/*
   ********************* Authors *********************
   ***** Laboratorio de Sistemas Embebidos (LSE) *****
   *****    http://laboratorios.fi.uba.ar/lse    *****
   ******************* 20/12/2011 ********************
     Ezequiel Espósito <ezequiel.esposito@gmail.com>
         Alan Kharsansky <akharsa@gmail.com>
       Federico Roasio <federoasio@gmail.com>
     Daniel Schermuk <daniel.schermuk@gmail.com>
   ***************************************************
*/

#include <stdint.h>
#include "MicroInterface.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"

inline void MicroInterface_Init ()
{
//	// Set as digital outputs LCD control lines
//	FIO_SetDir (LCD_CMD_PORT, (1<<D_I), 1);
//	FIO_SetDir (LCD_CMD_PORT, (1<<R_W), 1);
//	FIO_SetDir (LCD_CMD_PORT, (1<<EN), 1);
//	FIO_SetDir (LCD_CSEL_PORT, (1<<CSEL1), 1);
//	FIO_SetDir (LCD_CSEL_PORT, (1<<CSEL2), 1);
//
//	// Set as digital outputs LCD data lines
//	FIO_ByteSetDir (LCD_DATA_PORT, 0, 0xFF, 1);
//	FIO_SetDir(0,(1<<DB1),1);
	PINSEL_CFG_Type pinsel;

	pinsel.Funcnum = PINSEL_FUNC_0;
	pinsel.Pinmode = PINSEL_PINMODE_NORMAL;
	pinsel.OpenDrain = PINSEL_PINMODE_NORMAL;

	pinsel.Pinnum = D_I;
	pinsel.Portnum = LCD_DI_PORT;
	PINSEL_ConfigPin(&pinsel);

	pinsel.Pinnum = R_W;
	pinsel.Portnum = LCD_RW_PORT;
	PINSEL_ConfigPin(&pinsel);

	pinsel.Pinnum = EN;
	pinsel.Portnum = LCD_EN_PORT;
	PINSEL_ConfigPin(&pinsel);

	pinsel.Pinnum = CSEL1;
	pinsel.Portnum = LCD_CSEL_PORT;
	PINSEL_ConfigPin(&pinsel);

	pinsel.Pinnum = CSEL2;
	pinsel.Portnum = LCD_CSEL_PORT;
	PINSEL_ConfigPin(&pinsel);

	// Set as digital outputs LCD control lines
	FIO_SetDir (LCD_DI_PORT, (1<<D_I), 1);
	FIO_SetDir (LCD_RW_PORT, (1<<R_W), 1);
	FIO_SetDir (LCD_EN_PORT, (1<<EN), 1);
	FIO_SetDir (LCD_CSEL_PORT, (1<<CSEL1), 1);
	FIO_SetDir (LCD_CSEL_PORT, (1<<CSEL2), 1);

	// Set as digital outputs LCD data lines
	FIO_ByteSetDir (LCD_DATA_PORT, 0, 0xFF, 1);
}

inline void MicroInterface_Bit_WriteLow (uint8_t port, uint8_t pin)
{
	// LCD control lines are physical inverter
	FIO_SetValue(port,(1<<pin));
}

inline void MicroInterface_Bit_WriteHigh (uint8_t port, uint8_t pin)
{
	// LCD control lines are physical inverter
	FIO_ClearValue(port,(1<<pin));
}

inline void MicroInterface_Byte_WriteData (uint8_t buf)
{
	// The buf byte is inverter because the data lines are
	// physical inverted
	FIO_ByteSetValue(LCD_DATA_PORT,0,~buf);
	FIO_ByteClearValue(LCD_DATA_PORT,0,buf);

	// This is because DB1 is not physical connected to the
	// same port byte nibble.
	// This is specific for the actual hardware. In future
	// implementations should be omitted.
//	if ( (buf & 2) == 2 )
//		MicroInterface_Bit_WriteHigh (0, DB1);
//	else
//		MicroInterface_Bit_WriteLow (0, DB1);
}


