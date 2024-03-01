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

unsigned int red;
unsigned int green;
unsigned int blue;
unsigned int clear;
int increment = 0; // this is the 'base' time counter, increments every 16 seconds
unsigned int w;
unsigned int x;
unsigned int y;
unsigned int z;

void main(void){
    color_click_init(); //initialize the color clicker
    init_buttons_LED();
    TRISDbits.TRISD7 = 0;
    LATDbits.LATD7 = 1;
    interrupts_init();
    Timer0_init();
    
    
//    struct RGB_val { 
//        unsigned int R;
//        unsigned int G;
//        unsigned int B;
//    };
    LEDturnON();
    
    
    while(1) {
        
//        LATDbits.LATD7 = 1;
        w = PORTB;
        x = color_readfromaddress(0x14);
        y = color_readdoublefromaddress(0x06);
        z = color_readfromaddress(0x01);
        x = color_readfromaddress(0x13);
//        LEDturnON();
        __delay_ms(1000);
        red = readRedColor();
        green = readGreenColor();
        blue = readBlueColor();
        clear = readClearColor();
//        LEDturnOFF();
        
        
//        if (red > 100) {
//            LATDbits.LATD7 = 1;
//            __delay_ms(1000);
//            LATDbits.LATD7 = 0;
//        }
//        
//        if (green > 100) {
//            LATDbits.LATD7 = 1;
//            __delay_ms(1000);
//            LATDbits.LATD7 = 0;
//        }
//        
//        if (blue > 100) {
//            LATDbits.LATD7 = 1;
//            __delay_ms(1000);
//            LATDbits.LATD7 = 0;
//        }
//        
//        if (clear > 100) {
//            LATDbits.LATD7 = 1;
//            __delay_ms(1000);
//            LATDbits.LATD7 = 0;
//        }
        
        __delay_ms(1000);
        __delay_ms(1000);
    
    }
}
