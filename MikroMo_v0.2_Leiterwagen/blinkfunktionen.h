#ifndef BLINKFUNKTIONEN_H
#define BLINKFUNKTIONEN_H

// globale Statusvariablen / Deklarierung in main.c / Status aus lichtfunktionen.c
extern volatile uint8_t blinkerLinksStatus;
extern volatile uint8_t blinkerRechtsStatus;
extern volatile uint8_t warnblinkerStatus;
extern volatile uint8_t warnblinkerStuetzenStatus;
extern volatile uint8_t wechselblinkerStuetzenStatus;

#endif // BLINKFUNKTIONEN_H