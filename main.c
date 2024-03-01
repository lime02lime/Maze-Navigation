// CONFIG1L
#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL// Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)

// CONFIG3L
#pragma config WDTCPS = WDTCPS_31// WDT Period Select bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

#include <xc.h>
#include "dc_motor.h"
#include "color.h"
#include "i2c.h"
#include "interact.h"
#include "interrupts.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz

int increment = 0; // this is the time counter, increments every 0.125 seconds
char wall_detected = 0;
//unsigned int w;
//unsigned int x;
//unsigned int y;
//unsigned int z;
char square = 8 * 2;// increments per second * seconds
char instruction_array[20][2];
char instruction_array_index = 0;

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
    int colourCode;
    LEDturnON();
    
    // Setting up motors
    unsigned int PWMperiod = 99;
    initDCmotorsPWM(PWMperiod);
    
    struct DC_motor motorL;
    motorL.power = 0;
    motorL.direction = 1;
    motorL.brakemode = 1;
    motorL.PWMperiod = PWMperiod;
    motorL.posDutyHighByte = &CCPR1H;
    motorL.negDutyHighByte = &CCPR2H;
    setMotorPWM(&motorL);
    struct DC_motor motorR;
    motorR.power = 0;
    motorR.direction = 1;
    motorR.brakemode = 1;
    motorR.PWMperiod = PWMperiod;
    motorR.posDutyHighByte = &CCPR3H;
    motorR.negDutyHighByte = &CCPR4H;
    setMotorPWM(&motorR);
    
    
    while(1) {
        
//        w = PORTB;
//        x = color_readfromaddress(0x14);
//        y = color_readdoublefromaddress(0x06);
//        z = color_readfromaddress(0x01);
        if (wall_detected) {
            stop(&motorL, &motorR);
            unsigned int red = readRedColor();
            unsigned int green = readGreenColor();
            unsigned int blue = readBlueColor();
            unsigned int clear = readClearColor();
            
            
            char colourCode = getColourCode(RGB);
            
           // Adds instruction to history
            instruction_array[instruction_array_index] = {colourCode, increment};
            instruction_array_index += 1;
            //
            executeInstruction(&motorL, &motorR, colourCode);
            LEDturnON();
            increment = 0;
        }
        trundle(&motorL, &motorR);
        
        
        
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
        
//        __delay_ms(1000);
//        __delay_ms(1000);
    
    }
}
