/*
 * LCD_API.c
 *
 *  Created on: 29/01/2015
 *      Author: Maximiliano
 */

/*
 * @note Includes
 */

#include "LCD_API.h"


/*
 * @note Funciones Publicas
 */
/*
 * @fun		void StartGUI(void)
 * @brief	Arranque de GUI & LCD
 * @param	void
 * @return	void
 */
void StartGUI ()
{
	GLCDLowLevel_Init ( NORMAL );
	GLCDPrimitives_ClearScreen ( WHITE );
	GLCDLowLevel_SwapBuffer ();
	return;
}

/*
 * @fun		void Erase_Line(uint8_t line)
 * @brief	Borrado de linea
 * @param	uint8_t line: Linea a borrar
 * @return	void
 */
void  Erase_Line(uint16_t line){
	uint32_t i=0;

	for(i=0; i < MAX_X_CHARS ; i++)  GLCDPrimitives_DrawRect(i , line, SYSTEM5x7_WIDTH , SYSTEM5x7_HEIGHT , WHITE);

	GLCDLowLevel_SwapBuffer ();

	return;
}

/*
 * @fun		void Draw_String(uint16_t x, uint16_t y, char* txt, uint8_t color)
 * @brief	Escritura de una linea
 * @param	uint16_t x: Coordenada en X de comienzo
 * 			uint16_t y: Coordenada en Y de comienzo
 * 			char*:	Punetro al stringa a imprimir
 * 			color: 	Color del string
 * @return	void
 */
void Draw_String(uint16_t x, uint16_t y, char* txt, uint8_t color){

	static TText texto;

	uint32_t v;
	static char aux[MAX_X_CHARS+1];
	for(v = 0; v < MAX_X_CHARS; v++) aux[v] = txt[v];
	aux[v] = '\0';

	Erase_Line(y);

	texto.color = WHITE;
	texto.fontColor = color;
	texto.text = aux;
	texto.xPos = x;
	texto.yPos = y;

	GUI_Text_Draw(&texto);

	GLCDLowLevel_SwapBuffer();

	return;
}

/*
 * @fun		void Erase_Pant(void)
 * @brief	Borrado de pantalla
 * @param	uint8_t	color: Color de pantalla
 * @return	void
 */
void Erase_Pant(uint8_t color){

	GLCDPrimitives_ClearScreen(color);

	GLCDLowLevel_SwapBuffer();

	return;
}
