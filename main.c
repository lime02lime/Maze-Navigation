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
char wall_detected = 0;
struct colors RGBC;
struct normColors normRGB;
unsigned int colourCode = 0;
unsigned int w;
unsigned int x;
unsigned int y;
unsigned int z;

void main(void){
    color_click_init(); //initialize the color clicker
    init_buttons_LED();
    TRISDbits.TRISD7 = 0;
    LATDbits.LATD7 = 0;
    interrupts_init();
    Timer0_init();
    
    LEDturnON();
    __delay_ms(1000);
    
    LATDbits.LATD7 = 1;
    while(1) {
        
        
//        w = PORTB;
//        x = color_readfromaddress(0x13);
//        y = color_readdoublefromaddress(0x06);
//        z = color_readfromaddress(0x01);
        
        if (wall_detected) {
            LATDbits.LATD7 = 0;
            normalizeColors(&RGBC, &normRGB);
            
            //deciding what colour it is
            unsigned int colourCode = decideColor(&normRGB);
            //finding and running the corresponding instructions:
            if (colourCode == 1) {
                TRISHbits.TRISH3 = 0;
                for (char i=0; i<1; i++) {
                    LATHbits.LATH3 = 1;
                    __delay_ms(150);
                    LATHbits.LATH3 = 0;
                    __delay_ms(100);
                }
            }
            
            if (colourCode == 2) {
                TRISHbits.TRISH3 = 0;
                for (char i=0; i<2; i++) {
                    LATHbits.LATH3 = 1;
                    __delay_ms(150);
                    LATHbits.LATH3 = 0;
                    __delay_ms(100);
                }
            }
            
            if (colourCode == 3) {
                TRISHbits.TRISH3 = 0;
                for (char i=0; i<3; i++) {
                    LATHbits.LATH3 = 1;
                    __delay_ms(150);
                    LATHbits.LATH3 = 0;
                    __delay_ms(100);
                }
            }
            
            if (colourCode == 4) {
                TRISHbits.TRISH3 = 0;
                for (char i=0; i<2; i++) {
                    LATHbits.LATH3 = 1;
                    __delay_ms(150);
                    LATHbits.LATH3 = 0;
                    __delay_ms(150);
                }
                LATDbits.LATD7 = 1;
                __delay_ms(150);
                LATDbits.LATD7 = 0;
                __delay_ms(150);
            }
            
            if (colourCode == 6) {
                TRISHbits.TRISH3 = 0;
                for (char i=0; i<3; i++) {
                    LATHbits.LATH3 = 1;
                    __delay_ms(150);
                    LATHbits.LATH3 = 0;
                    __delay_ms(150);
                }
                LATDbits.LATD7 = 1;
                __delay_ms(150);
                LATDbits.LATD7 = 0;
                __delay_ms(150);
            }
            

            //reset flag:            
            wall_detected = 0;
            
            LEDturnON();
            LATHbits.LATH3 = 0;
        }

    
    }
}
