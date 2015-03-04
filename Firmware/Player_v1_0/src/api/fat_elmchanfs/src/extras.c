#include "integer.h"
//#include "lpc17xx_rtc.h"

DWORD get_fattime ()
{
#if 1
	// Tengo el cristal del RTC roto! devuelvo valor falso
	return	  ((DWORD)(2012 - 1980) << 25)
			| ((DWORD)8 << 21)
			| ((DWORD)21 << 16)
			| ((DWORD)23 << 11)
			| ((DWORD)30 << 5)
			| ((DWORD)0 >> 1);
#else
	RTC_TIME_Type rtc;
	RTC_GetFullTime(LPC_RTC,&rtc);
	// Pack date and time into a DWORD variable
	return	  ((DWORD)(rtc.YEAR - 1980) << 25)
			| ((DWORD)rtc.MONTH << 21)
			| ((DWORD)rtc.DOM << 16)
			| ((DWORD)rtc.HOUR << 11)
			| ((DWORD)rtc.MIN << 5)
			| ((DWORD)rtc.SEC >> 1);
#endif
}
