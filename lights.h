#ifndef _lights_H
#define _lights_H

#include <xc.h>
#include "color.h"
#include "i2c.h"

#define _XTAL_FREQ 64000000

//defining the tri-color LEDs
#define redLED LATGbits.LATG0
#define greenLED LATEbits.LATE7
#define blueLED LATAbits.LATA3
#define headLights LATDbits.LATD3
#define lowLights LATHbits.LATH1 // VERY DIM, NOT SURE IF NECESSARY
#define breakLights LATDbits.LATD4


void init_buttons_LED(void);
void LEDturnOFF(void);
void LEDturnON(void);


#endif
