/**************************************************************************+****
*                                                                              *
* doppelblitzfunktion.c                                                        *
* Datum        : 16.09.14                                                      *
* Entwickler   : pEEcor                                                        *
*                                                                              *
*******************************************************************************/

/*--------------------------- Bibliotheken -----------------------------------*/

#include <avr/io.h>

#include "doppelblitzfunktion.h"

/*--------------------------- blinkfunktionen --------------------------------*/

static uint8_t i = 0;
static uint8_t j = 0;

void doppelblitzfunktion()
{
	if(doppelblitzerStatus == 1)
    {
    	
    	// Doppelblitzer 1
    	if(i <= 70)
    	{
    		switch (i)
        	{
        	
	            case 0:
	            {
	                PORTD &= ~(1<<PD0);
	                break;
	            }
	            case 7:
	            {
	                PORTD |= (1<<PD0);
	                break;
	            }
	            case 14:
	            {
	                PORTD &= ~(1<<PD0);
	                break;
	            }
	            case 21:
	            {
	                PORTD |= (1<<PD0);
	                break;
	            }
	        }

            if(i < 70)
		    {
		    	++i;
		    }
		    else
		    {
		    	i = 0;
		    }
    	}

    	// Doppelblitzer 2
    	if(j <= 66)
    	{
    		switch (j)
    		{
	    		case 0:
	            {
	                PORTD &= ~(1<<PD4);
	                break;
	            }
	            case 7:
	            {
	                PORTD |= (1<<PD4);
	                break;
	            }
	            case 14:
	            {
	                PORTD &= ~(1<<PD4);
	                break;
	            }
	            case 21:
	            {
	                PORTD |= (1<<PD4);
	                break;
	            }
    		}

    		if(j < 66)
		    {
		    	++j;
		    }
		    else
		    {
		    	j = 0;
		    }
    	}
    } 
}