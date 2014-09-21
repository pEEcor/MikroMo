#ifndef LICHTFUNKTIONEN_H
#define LICHTFUNKTIONEN_H

// aufrufende Funktion
void lichtfunktionen();

// Lichtfunktionen
void abblendlichtToggle();
void nebellichtToggle();
void flutlichtToggle();
// lokale Statusvariablen
static uint8_t abblendlichtStatus = 0;
static uint8_t nebellichtStatus = 0;
static uint8_t FlutlichtStatus = 0;

// Blinkfunktionen
void blinkerLinksToggle();
void blinkerRechtsToggle();
void warnblinkerToggle();
void warnblinkerStuetzenToggle();
void wechselblinkerStuetzenToggle();
// globale Statusvariablen / Deklarierung in main.c zur Nutzung in blinkfunktionen.c
extern volatile uint8_t blinkerLinksStatus;
extern volatile uint8_t blinkerRechtsStatus;
extern volatile uint8_t warnblinkerStatus;
extern volatile uint8_t warnblinkerStuetzenStatus;
extern volatile uint8_t wechselblinkerStuetzenStatus;

// Blaulichtfunktionen
void blaulichtToggle();
void blaulichtSireneToggle();
// lokale Statusvariablen
static uint8_t blaulichtStatus = 0;
static uint8_t blaulichtSireneStatus = 0;
// globale Statusvariable / Deklarierung in main.c zur Nutzung in doppelblitzfunktion.c
extern volatile uint8_t doppelblitzerStatus;
// Array mit Servowerten zum Ansprechen des Horntinys
extern volatile uint8_t ServoValue[];


#endif // LICHTFUNKTIONEN_H