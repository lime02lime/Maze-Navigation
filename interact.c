#include <xc.h>
#include "color.h"
#include "i2c.h"
#include "interact.h"

void init_buttons_LED(void){
    
    //Set LEDs as outputs:
    TRISGbits.TRISG0 = 0; // G0 on clicker = AN1 on buggy = RED LED on color clicker
    TRISEbits.TRISE7 = 0; // E7 on clicker = CS1 on buggy = GREEN LED on color clicker
    TRISAbits.TRISA3 = 0; // A3 on clicker = PWM1 on buggy = BLUE LED on color clicker
    
    //Set initial state of LEDs:
    redLED = 0;
    greenLED = 0;
    blueLED = 0;
}

//function to turn on all three colors of the LED
void LEDturnON(void){
    redLED = 1;
    greenLED = 1;
    blueLED = 1;
}
//function to turn off all three colors of the LED
void LEDturnOFF(void){
    redLED = 0;
    greenLED = 0;
    blueLED = 0;
}