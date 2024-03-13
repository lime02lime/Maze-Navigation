// CONFIG1L
#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL// Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)

// CONFIG3L
#pragma config WDTCPS = WDTCPS_31// WDT Period Select bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

// include relevant files:
#include <xc.h>
#include "dc_motor.h"
#include "color.h"
#include "i2c.h"
#include "interact.h"
#include "interrupts.h"
#include "instructions.h"
#include "feedback.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz
#define PAUSE_BETWEEN_INSTRUCTIONS 0 // For testing, ill introduce pause after execution of instruction broken with button RF2 press
#define NO_TRUNDLING 0 // For testing, no forward movement between instructions

//unsigned int revsc
int increment = 0; // this is the 'base' time counter, increments every 16 seconds
char wall_detected = 0;

char square = 8 * 2;// increments per second * seconds
char instruction_array[20][2];
char instruction_array_index = 0;
char reverseRouteFlag = 0;

char turnLeftPower = 30;
char turnRightPower = 31;

void main(void){
    color_click_init(); //initialise the colour clicker.
    init_buttons_LED(); //initialise LEDs on buggy and colour clicker.
    initBoardLEDs(); //initialise LEDs on picKit.
    initButtons(); //initialise buttons on picKit.
    
    
    //Structures to store the RGBC values read from the colour sensor, and then their normalised values.
    struct colors RGBC;
    struct normColors normRGB;
    
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
    //checkBattery();

    turnLeftPower = leftCali(&motorL, &motorR);
    __delay_ms(500);
    turnRightPower = rightCali(&motorL, &motorR);

    interrupts_init(); //initialise colour sensor interrupts.
    Timer0_init(); //initialise timer overflow interrupts.
    
    LEDturnON(); //turn on all 3 colours of the tri-colour LED + headlights.
    __delay_ms(1000);
    
    while (PORTFbits.RF2); //wait until button press to start.
    increment = 0; //resetting the timer counter once we start.
    
    while(1) { //loop infinitely until a wall is detected
        
        if (wall_detected) { //this would be raised in the ISR once a wall is detected.
            
            fastStop(&motorL, &motorR); // stop the buggy

            readColors(&RGBC); //determine the colour of the card in front of the buggy
            
            normalizeColors(&RGBC, &normRGB); //normalise the colour reading

            char colourCode = decideColor(&normRGB, &RGBC, &motorL, &motorR); //deciding what colour it is
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
            
            LATDbits.LATD7 = 0; //turn off the picKit LED that was turned on by the interrupt
            
            //if we want to press a button to resume its course after completing a set of instructions:
            if (PAUSE_BETWEEN_INSTRUCTIONS) {
                while (PORTFbits.RF2);
            }
        }
        
        //check if it is time to return home
        if (reverseRouteFlag) {
            reverseRoute(&motorL, &motorR);
        }
        // NO_TRUNDLING is a flag you can set for testing if you don't want the motor to move forward between colour detections
        if (!NO_TRUNDLING) {
            trundle(&motorL, &motorR);
        }
    }
}
