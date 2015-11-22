/**************************************************************************+****
*                                                                              *
* main.c                                                                       *
* Datum        : 16.09.14                                                      *
* Entwickler   : pEEcor                                                        *
*                                                                              *
* Funktion     : - liest SumPPM (high) an ICP1 (PB1) von Deltang Empfänger ein *
*                - splited die 7 Kanaele in 14 auf mit Kanal 5 als Umschalter  *
*                - gibt 9 Servoleitungen aus: - PC0 -> Fahren                  *
*                                             - PC1 -> Lenken                  *
*                                             - PC2 -> Stuetzen horizontal     *
*                                             - PC3 -> Stuetzen vertikal links *
*                                             - PC4 -> Stuetzen vertikal rechts*
*                                             - PC5 -> Leiter heben & senken   *
*                                             - PD6 -> Leiter drehen           *
*                                             - PD7 -> Leiter aus & -einfahren *
*                                             - PD5 -> Impuls fuer Horntiny    *
*                - gibt Lichtfunktionen aus:  - PB1 -> Abblendlicht            *
*                                             - PB2 -> Nebellicht              *
*                                             - PB3 -> Blinker links           *
*                                             - PB4 -> Blinker rechts          *
*                                             - PB5 -> BlinkerStuetzen links   *
*                                             - PB6 -> BlinkerStuetzen rechts  *
*                                             - PB7 -> Flutlicht               *
*                                             - PD0 -> Doppelblitzer 1         *
*                                             - PD4 -> Doppelblitzer 2         *
*                                                                              *
*******************************************************************************/

/*--------------------------- Bibliotheken -----------------------------------*/

#include <avr/io.h>
#include <avr/interrupt.h>

#include "memoryFunktionsabtastung.h"
#include "blinkfunktionen.h"
#include "doppelblitzfunktion.h"
#include "log.h"

/*--------------------------- Präprozessor -----------------------------------*/

#define TRIM 3              // Trim für Mittelstellung, aller Kanäle
#define CHANNELSWITCH 4     // Kanal angeben +1

/*--------------------------- Globale Variablen ------------------------------*/

//PPM
volatile uint8_t kanal[7] = {188, 188, 188, 188, 188, 188, 188};

//Funktion
volatile uint8_t funktion[14] = {188, 188, 188, 188, 188, 188, 188,
                                 188, 188, 188, 188, 188, 188, 188};

// Servoausgaenge
uint8_t ServoPuls[9] = {1<<PC0, 1<<PC1, 1<<PC2, 1<<PC3, 1<<PC4, 1<<PC5, 1<<PD6, 1<<PD7, 1<<PD5};
volatile uint8_t ServoValue[9] = {188, 188, 188, 188, 188, 188, 188, 188, 188};

// Vergleichssekunden fuer MomoryFunktionserfassung
volatile uint8_t secMemoryFunktion = 0;

// Globale Statusvariablen der Blinkfunktionen fuer deren Implementierung mit Timer0
volatile uint8_t blinkerLinksStatus = 0;
volatile uint8_t blinkerRechtsStatus = 0;
volatile uint8_t warnblinkerStatus = 0;
volatile uint8_t warnblinkerStuetzenStatus = 0;
volatile uint8_t wechselblinkerStuetzenStatus = 0;

// Globale Statusvariablen der Blaulichtfunktionen fuer deren Implementierung mit Timer0
volatile uint8_t doppelblitzerStatus = 0;

/*--------------------------- Initialisierungen ------------------------------*/

// PPM Eingang initialiesieren Timer1
void InitPPM()
{
    TIMSK |= (1<<TICIE1) | (1<<OCIE1A) | (1<<OCIE1B);           // Input Capture Interrupt / Compare Match A Interrupt / Compare Match B Interrupt
    OCR1A  = 40;                                                // Kanalwerte in funktionsarry speichern / funktionen ServoValue zuweisen
    OCR1B  = 50;                                                // memoryFunktionsabtastung abfragen
    TCCR1B |= (1<<CS11) | (1<<CS10) | (1<<ICNC1) | (1<<ICES1);  // Timer1: Prescaler 8 / Noise Cancelation / Input Capture Edge (high) PB1 (ICP1)
}


// Servoausgänge und Timer initialisieren
void InitServo()
{
  // Die Servoleitungen auf Ausgang stellen
    DDRC |= (1<<PC0) | (1<<PC1) | (1<<PC2) | (1<<PC3) | (1<<PC4) | (1<<PC5);
    DDRD |= (1<<PD6) | (1<<PD7) | (1<<PD5);

    // Timer auf CTC Modus konfigurieren
    OCR2 = ServoValue[0];
    TIMSK |= (1<<OCIE2);                            // Output Compare Interrupt
    TCCR2 = (1<<WGM21) | (1<<CS22);                 // CTC mode / Prescaler 64
}


// Timer feur Lichtfunktionen
void InitLicht()
{
    // Lichtleitungen auf Ausgang
    DDRB  |= (1<<PB1) | (1<<PB2) | (1<<PB3) | (1<<PB4) | (1<<PB5) | (1<<PB6) | (1<<PB7);
    PORTB &= ~(1<<PB1) &~(1<<PB2) &~(1<<PB3) &~(1<<PB4);
    PORTB |= (1<<PB5) | (1<<PB6) | (1<<PB7);
    DDRD  |= (1<<PD0) | (1<<PD4);
    PORTD |= (1<<PD0) | (1<<PD4);

    // Timer auf Overflow konfigurieren
    TIMSK |= (1<<TOIE0);                            // Overflow Interrupt
    TCCR0 |= (1<<CS02);                             // Prescaler 256

}

/*----------------------------- Funktionen -----------------------------------*/

void memoryZeitzaehler(volatile uint8_t *secMemoryFunktion)  
{                              // inkrementiert mit Hilfe von Timer0 die Variable
    static uint8_t i = 0;      // *secMemoryFunktion im 1sekunden Takt zur Zeit-
                               // erfassung in der Memoryfunktionsabtastung
    if(i == 122)
    {
        ++*secMemoryFunktion;
        i = 0;
    }

    else
    {
        ++i;
    }
}

/*------------------------- Interrupt Behandlungen ---------------------------*/

ISR (TIMER1_CAPT_vect)
{
    
    // PPM Signal Einlesen und Werte im kanal array speichern
    static uint8_t kanalId = 0;

    TCNT1 = 0;

    
    if(ICR1 > 275)
    {
        kanalId = 0;
        return;
    }

    if((ICR1+TRIM) > 255)
    {
        kanal[kanalId] = 255;
        ++kanalId;
        return;
    }

    else
    {
        kanal[kanalId] = (ICR1+TRIM);
        ++kanalId;
        return;
    }
}


ISR (TIMER1_COMPA_vect)
{
    // Kanalwerte in Funktionarray ablegen
    static uint8_t kanalId = 0;

    if(kanal[CHANNELSWITCH] > 245)
    {

        if(kanalId < 7)
        {
           funktion[kanalId] = kanal[kanalId];
            ++kanalId;
        }
        else
        {
            kanalId = 0;
        }
    }

    if(kanal[CHANNELSWITCH] < 130)
    {

        if(kanalId < 7)
        {
           funktion[kanalId+7] = kanal[kanalId];
            ++kanalId;
        }
        else
        {
            kanalId = 0;
        }
    }

    // Funktionswerte ServoValue zuweisen
    ServoValue[0] = funktion[ 0];    // Funktion  0 -> Fahrkanal
    ServoValue[1] = funktion[ 1];    // Funktion  1 -> Lenkkanal
    ServoValue[2] = funktion[ 5];    // Funktion  5 -> Stuetzen horizontal
    ServoValue[3] = funktion[13];    // Funktion 13 -> Stuetzen vertikal links
    ServoValue[4] = funktion[12];    // Funktion 11 -> Stuetzen vertikal rechts
    ServoValue[5] = funktion[ 9];    // Funktion  9 -> Leiter heben & senken
    ServoValue[6] = funktion[ 8];    // Funktion  8 -> Leiter drehen
    ServoValue[7] = funktion[ 7];    // Funktion  7 -> Leiter aus- und einfahren
}


ISR(TIMER1_COMPB_vect)
{
    // Schaltkanäle auswerten
    memoryFunktionsabtastung(funktion, &secMemoryFunktion);
}


ISR (TIMER2_COMP_vect)
{
    //Servosignal ausgeben
    static volatile uint8_t ServoId = 0;

    // den Puls des aktuellen Servos beenden
    if(ServoId <= 5)
    {
        PORTC &= ~ServoPuls[ServoId];
    }

    if((ServoId <= 8) && (ServoId >= 6))
    {
        PORTD &= ~ServoPuls[ServoId];
    }

    // welches ist das nächste aktuelle Servo?
    if(++ServoId >= 9)
    {
        ServoId = 0;
    }

    // die Ausgangsleitung fuer dieses Servo auf 1; den Puls beginnen
    if(ServoId <= 5)
    {
        PORTC |= ServoPuls[ServoId];
    }

    if((ServoId <= 8) && (ServoId >= 6))
    {
        PORTD |= ServoPuls[ServoId];
    }
    
    // den Timer so einstellen, dass bei Pulsende, die ISR erneut aufgerufen wird
    OCR2 = ServoValue[ServoId];
}


ISR(TIMER0_OVF_vect)            // Aufruf alle ~8ms
{
    memoryZeitzaehler(&secMemoryFunktion);
    blinkfunktionen();
    doppelblitzfunktion();
}
/*-------------------------- main Funktion -----------------------------------*/

int main(void)
{
    InitPPM();
    InitServo();
    InitLicht();
    //InitUSART();
    sei();  // IRQ global aktivieren

    /*
    uart_putc(10);                      // Programmstart -> RS232
    uart_putc(13);
    uart_puts("Start: ");
    uart_putc(10);
    uart_putc(13);
    */

    while(1)
    {
        //rs232_log(funktion);
    }
    
    return 0;
}