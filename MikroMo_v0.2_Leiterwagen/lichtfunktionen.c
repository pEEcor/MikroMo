/**************************************************************************+****
*                                                                              *
* lichtfunktionen.c                                                            *
* Datum        : 16.09.14                                                      *
* Entwickler   : pEEcor                                                        *
*																			   *
* Parameter1   : *data -> aufzurufende Lichtfunktion                           *
*                                                                              *
*******************************************************************************/

/*--------------------------- Bibliotheken -----------------------------------*/

#include <avr/io.h>

#include "lichtfunktionen.h"

/*--------------------- lichtfunktionen -> Aufruf ----------------------------*/

void lichtfunktionen(uint8_t *data)
{
	switch(*data)
    {
        // funktion[2]
        case  1: abblendlichtToggle(); break;
        case  2: nebellichtToggle(); break;
        case  3: break;
        case  4: break;
        case  5: break;
        case  6: break;
        // funktion[3]
        case  7: blinkerLinksToggle(); break;
        case  8: warnblinkerToggle(); break;
        case  9: break;
        case 10: blinkerRechtsToggle(); break;
        case 11: warnblinkerStuetzenToggle(); break;
        case 12: wechselblinkerStuetzenToggle(); break;
        // funktion[6]
        case 13: blaulichtSireneToggle(); break;
        case 14: break;
        case 15: blaulichtToggle(); break;
        case 16: break;
        // funktion[10]
        case 17: flutlichtToggle(); break;
        case 18: break;
        case 19: break;
        case 20: break;
    }
}

/*------------------------------ Abblendlicht --------------------------------*/

void abblendlichtToggle()
{
	if(abblendlichtStatus == 0)
	{
		PORTB &= ~(1<<PB1);
		abblendlichtStatus = 1;
		return;
	}

	if(abblendlichtStatus == 1)
	{
		PORTB |= (1<<PB1);
		abblendlichtStatus = 0;
		return;
	}
}

/*------------------------------ Nebellicht ----------------------------------*/

void nebellichtToggle()
{
	if(nebellichtStatus == 0)
	{
		PORTB &= ~(1<<PB2);
		nebellichtStatus = 1;
		return;
	}

	if(nebellichtStatus == 1)
	{
		PORTB |= (1<<PB2);
		nebellichtStatus = 0;
		return;
	}
}

/*------------------------------- Flutlicht ----------------------------------*/

void flutlichtToggle()
{
	if(FlutlichtStatus == 0)
	{
		PORTB &= ~(1<<PB7);
		FlutlichtStatus = 1;
		return;
	}

	if(FlutlichtStatus == 1)
	{
		PORTB |= (1<<PB7);
		FlutlichtStatus = 0;
		return;
	}
}

/*----------------------------- Blinker links --------------------------------*/

void blinkerLinksToggle()
{
	if(blinkerLinksStatus == 0)
	{
		PORTB |= (1<<PB4);
		PORTB |= (1<<PB5);
        PORTB |= (1<<PB6);
		blinkerRechtsStatus = 0;
		warnblinkerStatus = 0;
		warnblinkerStuetzenStatus = 0;
		wechselblinkerStuetzenStatus = 0;

		blinkerLinksStatus = 1;
		return;
	}

	if(blinkerLinksStatus == 1)
	{
		PORTB |= (1<<PB3);
		blinkerLinksStatus = 0;
		return;
	}
}

/*----------------------------- Blinker rechts -------------------------------*/

void blinkerRechtsToggle()
{
	if(blinkerRechtsStatus == 0)
	{
		PORTB |= (1<<PB3);
		PORTB |= (1<<PB5);
        PORTB |= (1<<PB6);
		blinkerLinksStatus = 0;
		warnblinkerStatus = 0;
		warnblinkerStuetzenStatus = 0;
		wechselblinkerStuetzenStatus = 0;
		
		blinkerRechtsStatus = 1;
		return;
	}

	if(blinkerRechtsStatus == 1)
	{
		PORTB |= (1<<PB4);
		blinkerRechtsStatus = 0;
		return;
	}
}

/*------------------------------- Warnblinker --------------------------------*/

void warnblinkerToggle()
{
    if(warnblinkerStatus == 0)
    {
        blinkerLinksStatus = 0;
        blinkerRechtsStatus = 0;
        warnblinkerStuetzenStatus = 0;
        wechselblinkerStuetzenStatus = 0;

        PORTB |= (1<<PB5);
        PORTB |= (1<<PB6);

        warnblinkerStatus = 1;
        return;
    }
    if(warnblinkerStatus == 1)
    {
        warnblinkerStatus = 0;
        PORTB |= (1<<PB3);
        PORTB |= (1<<PB4);
        return;
    }
}

/*-------------------- Warnblinker incl. Stuetzen ----------------------------*/

void warnblinkerStuetzenToggle()
{
    if(warnblinkerStuetzenStatus == 0)
    {
        blinkerLinksStatus = 0;
        blinkerRechtsStatus = 0;
        warnblinkerStatus = 0;
        wechselblinkerStuetzenStatus = 0;

        warnblinkerStuetzenStatus = 1;
        return;
    }
    if(warnblinkerStuetzenStatus == 1)
    {
        warnblinkerStuetzenStatus = 0;
        PORTB |= (1<<PB3);
        PORTB |= (1<<PB4);
        PORTB |= (1<<PB5);
        PORTB |= (1<<PB6);
        return;
    }
}

/*----------------------- Wechselblinker Stuetzen ----------------------------*/

void wechselblinkerStuetzenToggle()
{
    if(wechselblinkerStuetzenStatus == 0)
    {
        blinkerLinksStatus = 0;
        blinkerRechtsStatus = 0;
        warnblinkerStatus = 0;
        warnblinkerStuetzenStatus = 0;

        wechselblinkerStuetzenStatus = 1;
        return;
    }
    if(wechselblinkerStuetzenStatus == 1)
    {
        wechselblinkerStuetzenStatus = 0;
        PORTB |= (1<<PB3);
        PORTB |= (1<<PB4);
        PORTB |= (1<<PB5);
        PORTB |= (1<<PB6);
        return;
    }
}

/*------------------------------- Blaulicht ----------------------------------*/

void blaulichtToggle()
{
	if(blaulichtStatus == 0)
    {
    	blaulichtSireneStatus = 0;

    	ServoValue[8] = 165;			// Horntiny -> Blaulicht
    	blaulichtStatus = 1;
    	doppelblitzerStatus = 1;
        return;
    }
    if(blaulichtStatus == 1)
    {
    	PORTD |= (1<<PD0) | (1<<PD4);
    	ServoValue[8] = 188;			// Horntiny -> aus
    	blaulichtStatus = 0;
    	doppelblitzerStatus = 0;
        return;
    }
}

/*------------------------- Blaulicht incl. Sirene ---------------------------*/

void blaulichtSireneToggle()
{
	if(blaulichtSireneStatus == 0)
    {
    	blaulichtStatus = 0;

    	ServoValue[8] = 155;			// Horntiny -> Blaulicht & Sirene
    	blaulichtSireneStatus = 1;
    	doppelblitzerStatus = 1;
        return;
    }
    if(blaulichtSireneStatus == 1)
    {
    	PORTD |= (1<<PD0) | (1<<PD4);
    	ServoValue[8] = 188;			// Horntiny -> aus
    	blaulichtSireneStatus = 0;
    	doppelblitzerStatus = 0;
        return;
    }
}