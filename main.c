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
#include "lights.h"
#include "interrupts.h"
#include "instructions.h"
#include "feedback.h"
#include "calibration.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz
// Flags used to activate / deactivate functionality for testing
#define PAUSE_BETWEEN_INSTRUCTIONS 0 // Introduce pause after execution of instruction broken with button RF2 press
#define NO_TRUNDLING 0 // No forward movement between instructions
#define SKIP_CALIBRATION 1 // Skipping motor and brightness calibration
#define INDICATE_INSTRUCTION 1 // Blink LED number of times corresponding to colour code
#define CHECK_BATTERY 1 // Whether to check and indicate battery level

// =================
// Setting globals
// =================
int increment = 0; // this is the 'base' time counter, increments every 0.125 seconds
char wall_detected = 0; // Flag changed when wall detected through color clicker interrupt

char instruction_array[20][2]; // array to store history of instructions
char instruction_array_index = 0; // stores index of last instruction performed
char reverseRouteFlag = 0; // flag for main loop to know when to activate route reversa routine

char turnLeftPower = 30; // Default power for motors when turning left
char turnRightPower = 31; // Default power for motors when turning right


void main(void){
    // ================
    // Initialisation and calibration routines
    // ================
    
    color_click_init(); //initialise the colour clicker.
    init_buttons_LED(); //initialise LEDs on buggy and colour clicker.
    initBoardLEDs(); //initialise LEDs on picKit.
    initButtons(); //initialise buttons on picKit.
    
    //Structures to store the RGBC values read from the colour sensor, and then their normalised values.
    struct colors RGBC;
    struct normColors normRGB;
    
    // Setting up motors
    char PWMperiod = 99;
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
    if (CHECK_BATTERY) {
        checkBattery();
    }
    // Motor calibration
    if (!SKIP_CALIBRATION) {
        leftCali(&motorL, &motorR);
        __delay_ms(500);
        rightCali(&motorL, &motorR);
    } 
    // Brightness calibration
    interrupts_init(&motorL, &motorR, SKIP_CALIBRATION); //initialise colour sensor interrupts.
    Timer0_init(); //initialise timer overflow interrupts.
    
    //turn on all 3 colours of the tri-colour LED + headlights
    LEDturnON(); 
    __delay_ms(1000);
    
    while (PORTFbits.RF2); //wait until button press to start.
    increment = 0; //resetting the timer counter upon start
    
    // ======================================
    // Main while loop
    // ======================================
    
    while(1) { 
        
        
        if (wall_detected) { // raised in ISR once wall detected.
            // Record how long the buggy has trundled for
            int time_trundled = increment;
            stop(&motorL, &motorR); // stop the buggy

            // Read colour
            readColors(&RGBC); // Read RGBC pixels from colour clicker
            normalizeColors(&RGBC, &normRGB); //normalise colour reading
            char colourCode = decideColor(&normRGB, &RGBC, &motorL, &motorR); //decide colour and return colour code
            
            if (INDICATE_INSTRUCTION) {
                // Indicating the colour code to tester
                indicateInstruction(colourCode);
            }

            // Adds instruction (defined by colour code) to history
            instruction_array[instruction_array_index][0] = colourCode;
            // Add trundle duration before interrupt to history
            instruction_array[instruction_array_index][1] = time_trundled / 8; // Dividing by 8 to ease up storage for char
            instruction_array_index += 1;
            
            // Carry out instruction and turn the light back on
            executeInstruction(&motorL, &motorR, colourCode);
            LEDturnON();
            
            //reset all of the flags, variables and interrupts            
            wall_detected = 0;
            clearInterrupt(); 
            PIE0bits.INT0IE = 1; // Allow brightness interrupts again now that instruction execution has finished
            LATDbits.LATD7 = 0; //turn off the picKit LED that was turned on by the interrupt
            increment = 0; // reset timer
            
            //if we want to press a button to resume its course after completing a set of instructions:
            if (PAUSE_BETWEEN_INSTRUCTIONS) {
                while (PORTFbits.RF2);
            }
        }
        
        //check if it is time to return home
        if (reverseRouteFlag) {
            reverseRoute(&motorL, &motorR);
            reverseRouteFlag = 0; // resets flag
            while (PORTFbits.RF2); // pauses to wait for restart
            // Resetting timer
            increment=0;
        }
        // Continue trundling if flag has been set to allow it
        if (!NO_TRUNDLING) {
            trundle(&motorL, &motorR);
        }
    }
}
