/**************************************************************************+****
*                                                                              *
* lichtfunktionen.c                                                            *
* Datum        : 16.09.14                                                      *
* Entwickler   : pEEcor                                                        *
*                                                                              *
*******************************************************************************/

/*--------------------------- Bibliotheken -----------------------------------*/

#include <avr/io.h>

#include "blinkfunktionen.h"

/*--------------------------- blinkfunktionen --------------------------------*/

void blinkfunktionen()
{
	static uint8_t i = 0;							// i = 244 -> ca 2sec

	if(blinkerLinksStatus == 1)
	{
	    if((i == 127) || (i == 255))
	    {
	        PORTB &= ~(1<<PB3);
	    }
	    if((i == 63) || (i == 191))
	    {
	        PORTB |= (1<<PB3);
	    }
	}

	if(blinkerRechtsStatus == 1)
	{
	    if((i == 127) || (i == 255))
	    {
	        PORTB &= ~(1<<PB4);
	    }
	    if((i == 63) || (i == 191))
	    {
	        PORTB |= (1<<PB4);
	    }
	}

	if(warnblinkerStatus == 1)
    {
        if((i == 127) || (i == 255))
        {
            PORTB &= ~(1<<PB3);
            PORTB &= ~(1<<PB4);
        }
        if((i == 63) || (i == 191))
        {
            PORTB |= (1<<PB3);
            PORTB |= (1<<PB4);
        }
    }

    if(warnblinkerStuetzenStatus == 1)
    {
        if((i == 127) || (i == 255))
        {
            PORTB &= ~(1<<PB3);
            PORTB &= ~(1<<PB4);
            PORTB &= ~(1<<PB5);
            PORTB &= ~(1<<PB6);
        }
        if((i == 63) || (i == 191))
        {
            PORTB |= (1<<PB3);
            PORTB |= (1<<PB4);
            PORTB |= (1<<PB5);
            PORTB |= (1<<PB6);

        }
    }

    if(wechselblinkerStuetzenStatus == 1)
    {
        if((i == 127) || (i == 255))
        {
            PORTB &= ~(1<<PB3);
            PORTB |= (1<<PB4);
            PORTB &= ~(1<<PB5);
            PORTB |= (1<<PB6);
        }
        if((i == 63) || (i == 191))
        {
            PORTB |= (1<<PB3);
            PORTB &= ~(1<<PB4);
            PORTB |= (1<<PB5);
            PORTB &= ~(1<<PB6);
        }
    }

	++i;											// i um inkrementieren
}


