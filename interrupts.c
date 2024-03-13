
#include "interrupts.h"

// Initialise brightness and timer0 interrupts
void interrupts_init(DC_motor *mL, DC_motor *mR, char skip_calibration)
{
    // ===========================
    // Configuring brightness interrupt from colour clicker
    // ===========================
    
    // Enabling brightness interrupt from colour clicker through INT0 on pin RB0 through Peripheral port select (PPS)
    INT0PPS = 0x08;
    PIE0bits.INT0IE = 1;
    INTCONbits.INT0EDG = 0;
    IPR0bits.INT0IP = 0;
    ANSELBbits.ANSELB0 = 0;
    
    //set interrupt brightness thresholds
    color_writetoaddr(0x04, 0x00); //low thresh, lower byte
    color_writetoaddr(0x05, 0x01); //low thresh, upper byte (used to be 0b00000001)
    
    
    if (skip_calibration) { // Using default values if not calibrating
        // Write thresholds to clicker
        color_writetoaddr(0x06, 0b00000000); //upper thresh, lower byte 
        color_writetoaddr(0x07, 0b00000010); //upper  thresh, upper byte 
    }
    else { // calibrating
        // Start brightness calibration routine
        unsigned int threshold = calibrate_brightness_sensor(mL, mR);
        // Extract upper and lower threshold from answer
        char upper_threshold = (threshold & 0b1111111100000000) >> 8;
        char lower_threshold = threshold & 0b0000000011111111;

        // Write thresholds to clicker
        color_writetoaddr(0x06, lower_threshold); //upper thresh, lower byte
        color_writetoaddr(0x07, upper_threshold); //upper  thresh, upper byte
    }

    //set persistence register
    color_writetoaddr(0x0C, 0b0011); // 3 clear channel values outside of threshold range will trigger interrupt.
    
//    INTCONbits.IPEN=1; //enable priority levels on interrupts
    
// =====================================    
// Configuring timer interrupts
// =====================================
    PIE0bits.TMR0IE = 1;
    PIR0bits.TMR0IF = 0; // set initial interrupt flag
//    IPR0bits.TMR0IP = 0; // Setting the priority of this interrupt to low
    INTCONbits.PEIE=1; //peripheral interrupts enable
    
    
    // Clears brightness interrupt from colour clicker
    clearInterrupt();
    INTCONbits.GIE=1; 	//turn on interrupts globally

}

// Configuring Timer 0 settings
void Timer0_init(void)
{
    T0CON1bits.T0CS=0b010; // Fosc/4
    T0CON1bits.T0ASYNC=1; // see datasheet errata - needed to ensure correct operation when Fosc/4 used as clock source
    // f_interrupt = 64000000/(4*Prescaler*2^16)
    T0CON1bits.T0CKPS=0b0101; // 1:32 prescaler, means 8 overflows a second so timing precise to 125 ms, increment overflows after 2 hours
    T0CON0bits.T016BIT=1;	// 16 bit mode	
	
    // Initialising timer registers
    TMR0H=0;            //write High reg first, update happens when low reg is written to
    TMR0L=0;
    T0CON0bits.T0EN=1;	//start the timer
}

// Interrupt service routine
void __interrupt(high_priority) High_ISR() {
    // Checks flag for colour clicker clear reading brightness interrupt
    if (PIR0bits.INT0IF) { 

        LATDbits.LATD7 = 1; //turn on picKit LED D7
        wall_detected = 1; //raise flag - this is checked for in the main.c while loop.
        // Clear interrupt:
        clearInterrupt(); //clears the interrupt on the colour clicker
        PIR0bits.INT0IF = 0; //clears the interrupt on the picKit.
        
        // Stop further light readings from re-triggering interrupt by disabling global interrupts
        //this is later re-enabled in the main loop after handling the interrupt.
        INTCONbits.GIE=0; 
    }
    //if timer overflow interrupt is raised
    if (PIR0bits.TMR0IF) { 
        PIR0bits.TMR0IF = 0; //reset interrupt flag
        increment++; //add to the overflow counter, helps keep track of time
    }
    
}

//Clearing the interrupt on the colour clicker.
void clearInterrupt(void){
    I2C_2_Master_Start();         //Start condition
    I2C_2_Master_Write(0x52 | 0x00);     //7 bit device address + Write mode
    I2C_2_Master_Write(0xe0 | 0x06 );    //command + register address needs 0b11100110
    I2C_2_Master_Stop(); 
}

