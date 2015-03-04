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

#ifndef _TEXT_H_
#define _TEXT_H_

#include <stdint.h>

// *********************
// Structures Definition
// *********************
struct typeText
{
	const char* windowName;
	const char* name;
	uint16_t xPos;
	uint16_t yPos;
	uint8_t color;
	const char* fontName;
	uint8_t fontColor;
	char* text;
};
typedef struct typeText TText;

void GUI_Text_Draw ( TText* pText );

#endif /* TEXT_H_ */
