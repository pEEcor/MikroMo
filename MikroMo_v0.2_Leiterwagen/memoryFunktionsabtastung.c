/**************************************************************************+****
*                                                                              *
* lichtfunktionen.c                                                            *
* Datum        : 16.09.14                                                      *
* Entwickler   : pEEcor                                                        *
*                                                                              *
* Parameter1   : funktion[]                                                    *
* Parameter2   : *secMemoryFunktion                                            *
*                                                                              *
*******************************************************************************/

/*--------------------------- Bibliotheken -----------------------------------*/

#include <avr/io.h>

#include "memoryFunktionsabtastung.h"
#include "lichtfunktionen.h"


void memoryFunktionsabtastung(uint8_t funktion[], volatile uint8_t *secMemoryFunktion, volatile uint8_t *test)
{
    static uint8_t a = 0;

    static uint8_t i = 0;
    static uint8_t j = 0;
    static uint8_t k = 0;
    static uint8_t l = 0;
    static uint8_t m = 0;

    static uint8_t t = 0;


    // Abtastung von Lenkkanal zum zurueckstellen der Blinker
    if((funktion[1] > 235) && (funktion[1] < 255))
    {
        a = 1;
    }
    if((funktion[1] < 140) && (funktion[1] > 120))
    {
        a = 2;
    }
    if((funktion[1] < 203) && (funktion[1] > 173))
    {
        switch(a)
        {
            case 1:
            {
                a = 0;
                if(blinkerLinksStatus == 1)
                {
                    blinkerLinksStatus = 0;
                    PORTB |= (1<<PB3);
                }
            }
            case 2:
            {
                a = 0;
                if(blinkerRechtsStatus == 1)
                {
                    blinkerRechtsStatus = 0;
                    PORTB |= (1<<PB4);
                }
            }
        }
    }

    // Funktionsabtastung für Funktion 3
    // Oberer Bereich
    if((funktion[2] > 235) && (funktion[2] < 255))
    {

        switch(i)
        {
            case 0: j = 1; break;
            case 1: j = 2; break;
            case 2: j = 3; break;
            case 3: j = 4; break;
            case 4: j = 5; break;
            case 5: j = 6; break;
        }
    }
    // Unterer Bereich
    if((funktion[2] < 140) && (funktion[2] > 120))
    {
        ServoValue[8] = 220; // Horntiny Hupe ein
    }
    // Unterer Bereich
    if((funktion[2] < 170) && (funktion[2] > 150))
    {
        ServoValue[8] = 188; // Horntiny Hupe ein
    }

    if((funktion[2] < 203) && (funktion[2] > 173))
    {
        switch(j)
        {
            case 1: i = 1; break;
            case 2: i = 2; break;
            case 3: i = 3; break;
            case 4: i = 4; break;
            case 5: i = 5; break;
            case 6: i = 6; break;
        }
    }

    // Funktionsabtastung für Funktion 4
    // Oberer Bereich
    if((funktion[3] > 235) && (funktion[3] < 255))
    {
        switch(i)
        {
            case 0: k = 7; break;
            case 7: k = 8; break;
            case 8: k = 9; break;
        }
    }
    // Unterer Bereich
    if((funktion[3] < 140) && (funktion[3] > 120))
    {
        switch(i)
        {
            case 0: k = 10; break;
            case 10: k = 11; break;
            case 11: k = 12; break;
        }
    }

    if((funktion[3] < 203) && (funktion[3] > 173))
    {
        switch(k)
        {
            case  7: i =  7; break;
            case  8: i =  8; break;
            case  9: i =  9; break;
            case 10: i = 10; break;
            case 11: i = 11; break;
            case 12: i = 12; break;
        }
    }

    // Funktionsabtastung für Funktion 5
    // Oberer Bereich
    if((funktion[6] >= 235) && (funktion[6] <= 255))
    {
        switch(i)
        {
            case  0: l = 13; break;
            case 13: l = 14; break;
        }
    }
    // Unterer Bereich
    if((funktion[6] <= 140) && (funktion[6] >= 120))
    {
        switch(i)
        {
            case  0: l = 15; break;
            case 15: l = 16; break;
        }
    }

    if((funktion[6] <= 203) && (funktion[6] >= 173))
    {
        switch(l)
        {
            case 13: i = 13; break;
            case 14: i = 14; break;
            case 15: i = 15; break;
            case 16: i = 16; break;
        }
    }

    // Funktionsabtastung für Funktion 7
    // Oberer Bereich
    if((funktion[10] >= 235) && (funktion[10] <= 255))
    {
        switch(i)
        {
            case  0: m = 17; break;
            case 17: m = 18; break;
        }
    }
    // Unterer Bereich
    if((funktion[10] <= 140) && (funktion[10] >= 120))
    {
        switch(i)
        {
            case  0: m = 19; break;
            case 19: m = 20; break;
        }
    }

    if((funktion[10] <= 203) && (funktion[10] >= 173))
    {
        switch(m)
        {
            case 17: i = 17; break;
            case 18: i = 18; break;
            case 19: i = 19; break;
            case 20: i = 20; break;
        }
    }

    *test = t;

    // Uebergabe der jeweils abgetasteten Lichtfunktion an lichtfunktion()
    if(i != 0)
    {
        if(t == 0)
        {
            *secMemoryFunktion = 0;
            t = 1;
        }
        if(t == 1)
        {
            if(*secMemoryFunktion == 2) // Nach ~2 sec
            {
                lichtfunktionen(&i);

                i = 0;
                j = 0;
                k = 0;
                l = 0;
                m = 0;
                t = 0;
            }
        }
    }
}
