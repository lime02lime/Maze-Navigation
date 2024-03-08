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
#include "instructions.h"
#include "feedback.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz
#define PAUSE_BETWEEN_INSTRUCTIONS 1
#define NO_TRUNDLING 1

//unsigned int revsc
int increment = 0; // this is the 'base' time counter, increments every 16 seconds
char wall_detected = 0;

char square = 8 * 2;// increments per second * seconds
char instruction_array[20][2];
char instruction_array_index = 0;
char reverseRouteFlag = 0;

void main(void){
    color_click_init(); //initialize the color clicker
    init_buttons_LED();
    initBoardLEDs();
    initButtons();
    
    interrupts_init();
    Timer0_init();
    
    struct colors RGBC;
    struct normColors normRGB;
    
    LEDturnON();
    __delay_ms(1000);
    
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
    
    // Checking battery
    checkBattery();
    
    while (PORTFbits.RF2);
    increment = 0;
    
    while(1) {
        
        if (wall_detected) {
            // stop the buggy
            fastStop(&motorL, &motorR);

            readColors(&RGBC);
            
            normalizeColors(&RGBC, &normRGB);
             
            //deciding what colour it is
            char colourCode = decideColor(&normRGB);
            //finding and running the corresponding instructions:
            
            // Indicating the instruction code
            indicateInstruction(colourCode);

//            LEDturnON();
//            LATHbits.LATH3 = 0;

            // Adds instruction to history
            instruction_array[instruction_array_index][0] = colourCode;
            instruction_array[instruction_array_index][1] = increment;
            instruction_array_index += 1;
            //
            executeInstruction(&motorL, &motorR, colourCode);
            LEDturnON();
            increment = 0;
            
            // Allow interrupts again now that instruction execution has finished
            //reset flag:            
            wall_detected = 0;
            clearInterrupt(); 
            INTCONbits.GIE=1;
            
            LATDbits.LATD7 = 0;
            if (PAUSE_BETWEEN_INSTRUCTIONS) {
                while (PORTFbits.RF2);
            }
            
        }
        
        if (reverseRouteFlag) {
            reverseRoute(&motorL, &motorR);
        }
        
        // For testing
        if (!PORTFbits.RF3) {
            reverseRouteFlag=1;
        }
        if (!NO_TRUNDLING) {
            trundle(&motorL, &motorR);
        }
    }
}
