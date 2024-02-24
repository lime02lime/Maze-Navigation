#ifndef _interact_H
#define _interact_H

#include <xc.h>

#define _XTAL_FREQ 64000000

//defining the tri-color LEDs
#define redLED LATGbits.LATG0
#define greenLED LATEbits.LATE7
#define blueLED LATAbits.LATA3


void init_buttons_LED(void);
void LEDturnOFF(void);
void LEDturnON(void);


#endif
