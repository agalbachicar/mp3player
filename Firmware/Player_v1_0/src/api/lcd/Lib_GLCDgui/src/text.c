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

#include <string.h>
#include "text.h"
//#include "object.h"
//#include "window.h"
#include "GLCDFonts.h"
#include "SystemFont5x7.h"
//#include "GLCDgui.h"


void GUI_Text_Draw ( TText* pText )
{
	// Draw button name
	GLCD_SelectFont(System5x7, pText->fontColor);
	GLCD_CursorFreeTo(pText->xPos, pText->yPos);
	GLCD_FreePuts( pText->text );

}
