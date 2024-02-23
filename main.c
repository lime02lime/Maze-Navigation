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

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz

void main(void){

    // EXERCISE 3 ONWARDS - DC MOTORS
    unsigned int PWMperiod = 99;
    initDCmotorsPWM(PWMperiod);
	//don't forget TRIS for your output!
    TRISFbits.TRISF2 = 1;
    ANSELFbits.ANSELF2=0; //turn off analogue input on pin

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

    while(1){
		// Exercise 4
        // Wait for button press to start
        while(PORTFbits.RF2 == 1);
        
        // Complete first square
        for (int i = 0; i < 4; i++) {    
            fullSpeedAhead(&motorL, &motorR);
            __delay_ms(100);
            stop(&motorL, &motorR);
            turnLeft(&motorL, &motorR);
        }
        
        // Turn an extra 90 degrees to complete 180 degree turn
        turnLeft(&motorL, &motorR);
        
        // Retrace steps
        for (int i = 0; i < 3; i++) {    
            fullSpeedAhead(&motorL, &motorR);
//            __delay_ms(50);
            stop(&motorL, &motorR);
            turnRight(&motorL, &motorR);
        }
        // Final straight
        fullSpeedAhead(&motorL, &motorR);
        __delay_ms(100);
        stop(&motorL, &motorR);

    
    
//    // Testing
//    turnLeft(&motorL, &motorR);
//    __delay_ms(1000);

    }
}
