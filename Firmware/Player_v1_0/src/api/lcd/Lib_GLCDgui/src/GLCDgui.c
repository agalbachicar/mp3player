
#include "GLCDLowLevel.h"
#include "GLCDPrimitives.h"
#include "GLCDgui.h"

// ******************
// Private attributes
// ******************

void GUI_StartLibrary ()
{
	GLCDLowLevel_Init ( NORMAL );
	GLCDPrimitives_ClearScreen ( WHITE );
	GLCDLowLevel_SwapBuffer ();
	//List_Init (&windowsList);
}
/*
void GUI_DefineHeapMalloc ( void* (*_Malloc) (size_t wantedSize) )
{
	GUI_Malloc_Pointer = _Malloc;
}

void GUI_DefineHeapFree ( void (*_Free) (void* pointer) )
{
	GUI_Free_Pointer = _Free;
}

void* GUI_Malloc (size_t wantedSize)
{
    return GUI_Malloc_Pointer (wantedSize);
}

void GUI_Free (void* MemoryToFreepointer)
{
	GUI_Free_Pointer (MemoryToFreepointer);
}
*/
