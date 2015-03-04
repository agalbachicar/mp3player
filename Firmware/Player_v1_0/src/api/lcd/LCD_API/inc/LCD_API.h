
#ifndef LCD_API_H_
#define LCD_API_H_

// GUI library includes
#include "text.h"
#include "SystemFont5x7.h"
#include "GLCDPrimitives.h"
#include "GLCDLowLevel.h"
#include "MicroInterface.h"
#include "GLCDFonts.h"
#include "GLCDColor.h"


// CMSIS and Drivers includes
#include "lpc17xx_pinsel.h"

/*
 * @define		MAX_X_CHARS
 * @brief		Cantidad de caracteres de 5x7 que entran a lo largo en el Display
 */
#define			MAX_X_CHARS 20


/*
 * @note 	Funciones Publicas
 */

/*
 * @fun		void StartGUI(void)
 * @brief	Arranque de GUI & LCD
 * @param	void
 * @return	void
 */
void StartGUI ();

/*
 * @fun		void Erase_Line(uint16_t line)
 * @brief	Borrado de linea
 * @param	uint8_t line: Linea a borrar
 * @return	void
 */
void	Erase_Line(uint16_t line);

/*
 * @fun		void Erase_Pant(uint8_t color)
 * @brief	Borrado de pantalla
 * @param	void
 * @return	void
 */
void Erase_Pant(uint8_t color);


/*
 * @fun		void Draw_String(uint16_t x, uint16_t y, char* txt, uint8_t color)
 * @brief	Escritura de una linea
 * @param	uint16_t x: Coordenada en X de comienzo
 * 			uint16_t y: Coordenada en Y de comienzo
 * 			char*:	Punetro al stringa a imprimir
 * 			color: 	Color del string
 * @return	void
 */
void Draw_String(uint16_t x, uint16_t y, char* txt, uint8_t color);


void GUI_StartLibrary (); // GLCDLowLevel_Init (NORMAL);


#endif /* DEFINITIONS_H_ */
