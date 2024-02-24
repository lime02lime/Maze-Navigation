// CONFIG1L
#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL// Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)

// CONFIG3L
#pragma config WDTCPS = WDTCPS_31// WDT Period Select bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

#include <xc.h>
//#include "dc_motor.h"
#include "color.h"
#include "i2c.h"
#include "interact.h"
#include "interrupts.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz

void main(void){
    color_click_init(); //initialize the color clicker
    init_buttons_LED();
    
    while(1) {
        TRISDbits.TRISD7 = 0;
        LATDbits.LATD7 = 0;

        LEDturnON();
        __delay_ms(1000);
        unsigned int red = readRedColor();
        unsigned int green = readGreenColor();
        unsigned int blue = readBlueColor();
        unsigned int clear = readClearColor();
        LEDturnOFF();
        
        if (red > 100) {
            LATDbits.LATD7 = 1;
            __delay_ms(1000);
            LATDbits.LATD7 = 0;
        }
        
        if (green > 100) {
            LATDbits.LATD7 = 1;
            __delay_ms(1000);
            LATDbits.LATD7 = 0;
        }
        
        if (blue > 100) {
            LATDbits.LATD7 = 1;
            __delay_ms(1000);
            LATDbits.LATD7 = 0;
        }
        
        if (clear > 100) {
            LATDbits.LATD7 = 1;
            __delay_ms(1000);
            LATDbits.LATD7 = 0;
        }
        
        __delay_ms(1000);
    
    }
}