/*******************************************************************************
 *                                 											   *
 * Funktion: log.c 															   *
 * Parameter1   : funktion[]         										   *
 * Parameter2   : ...         												   *
 * Rückgabewert : gibt Strings per USART aus | Baudrate 9600      			   *
 *                            												   *
 ******************************************************************************/


#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "log.h"

// USART initialisieren
void InitUSART()
{                                           
    UCSRB |= (1 << TXEN);                        // UART TX (senden) einschalten
    UCSRC |= (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);   // Asynchron, 8N1 

    UBRRH = ((51)>>8);                           // Baudrate 9600
    UBRRL = 51;
}

// Ein Zeichen senden
void uart_putc(unsigned char c)
{
    while(!(UCSRA & (1 << UDRE)));      // warte, bis UDR bereit

    UDR = c;                            // sende Zeichen
}

// Einen String senden
void uart_puts(char *s)
{
    while (*s)
    {   // so lange *s != NULL
        uart_putc(*s);
        s++;
    }
}

void rs232_log(uint8_t data[])
{
	static char str[4];
	static uint8_t kanalID = 0;

	if(kanalID < 9)
	{
		uart_puts("K:");
        uart_putc(kanalID+49);
        uart_puts(":");
		itoa(data[kanalID], str, 10 );  // 10 fuer radix -> Dezimalsystem
		uart_puts(str);
		uart_puts(" ");
		++kanalID;
		_delay_ms(50);
	}

	else
	{
		kanalID = 0;
		uart_putc(10); // LF -> Line Feed -> Neue Zeile
		uart_putc(13); // CR -> Carriage Return -> Wagenruecklauf
	}
}