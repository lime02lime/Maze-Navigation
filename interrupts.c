#include <xc.h>
#include "interrupts.h"
#include "i2c.h"
#include "color.h"
#include "interact.h"

extern int increment;
extern char wall_detected;
extern struct colors RGBC;
extern struct normColors normRGB;


//DONT KNOW IF WORKS:
void interrupts_init(void)
{
//****************************************
    //CONFIGURE GENERAL INTERRUPTS BELOW
//****************************************
    // turn on global interrupts, peripheral interrupts and the interrupt source
    // Allowing interrupt INT0 on pin RB0 through Peripheral port select (PPS)
    INT0PPS = 0x08;
    PIE0bits.INT0IE = 1;
    INTCONbits.INT0EDG = 0;
    IPR0bits.INT0IP = 0;
    ANSELBbits.ANSELB0 = 0;
    //set the interrupt thresholds on the TCS3471:
    color_writetoaddr(0x04, 0x00); //low thresh, lower byte
    color_writetoaddr(0x05, 0x01); //low thresh, upper byte (used to be 0b00000001)
    color_writetoaddr(0x06, 0b00000000); //upper thresh, lower byte (AH house - 0b10111111)
    color_writetoaddr(0x07, 0b00000100); //upper  thresh, upper byte (AH house 0b00000001)
    // EMIL SETTINGS
    // color_writetoaddr(0x06, 0x1C0); //upper thresh, lower byte 
    // color_writetoaddr(0x07, 0b0000001); //upper  thresh, upper byte

    //set persistence register
    color_writetoaddr(0x0C, 0b0111); //1 clear channel value outside of threshold range will trigger interrupt.
    
//    INTCONbits.IPEN=1; //enable priority levels on interrupts - don't uncomment this for now, will land us in trouble
    PIE0bits.TMR0IE = 1;
    INTCONbits.PEIE=1; //peripheral interrupts enable
    
    // Clear brightness interrupt    
//****************************************    
    // CONFIGURE TIMER INTERRUPTS BELOW
//****************************************
//    //TIMER INTERRUPTS
//    PIE0bits.TMR0IE = 1; // Turning on the interrupt from the timer
//    PIR0bits.TMR0IF = 0; // set initial interrupt flag
//    IPR0bits.TMR0IP = 0; // Setting the priority of this interrupt to low
//    INTCONbits.PEIE = 1;  //Turning on the peripheral interrupts
    
//****************************************
    // CONFIGURE COLOR INTERRUPTS BELOW
//****************************************
    
    
    clearInterrupt();

    // It's a good idea to turn on global interrupts last, once all other interrupt configuration is done.
    INTCONbits.GIE=1; 	//turn on interrupts GLOBALLY (when this is off, all interrupts are deactivated)

}

void Timer0_init(void)
{
    T0CON1bits.T0CS=0b010; // Fosc/4
    T0CON1bits.T0ASYNC=1; // see datasheet errata - needed to ensure correct operation when Fosc/4 used as clock source
    T0CON1bits.T0CKPS=0b0101; // 1:32 prescaler, means 8 overflows a second so timing precise to 125 ms, increment overflows after 2 hours
    T0CON0bits.T016BIT=1;	//8bit mode	
	
    // f_interrupt = 64000000/(4*Prescaler*2^16)

    // Initialising timer registers
    TMR0H=0;            //write High reg first, update happens when low reg is written to
    TMR0L=0;
    T0CON0bits.T0EN=1;	//start the timer
}
    
void __interrupt(high_priority) High_ISR() {
    if (PIR0bits.INT0IF) {

        
        LATDbits.LATD7 = 1;
      
        wall_detected = 1;

        
        // Clear interrupt
        clearInterrupt();
        // May also need to do this:
        PIR0bits.INT0IF = 0;
        
        // Stop further light readings from re-triggering interrupt by disabling global interrupts
        INTCONbits.GIE=0;
        
        
    }
    
    if (PIR0bits.TMR0IF) {
        PIR0bits.TMR0IF = 0;
        increment++;
        
    }
    
}
    



void clearInterrupt(void){
    I2C_2_Master_Start();         //Start condition
    I2C_2_Master_Write(0x52 | 0x00);     //7 bit device address + Write mode
    I2C_2_Master_Write(0xe0 | 0x06 );    //command + register address needs 0b11100110
    I2C_2_Master_Stop(); 
//    color_writetoaddr(0x13, 0x00); //write 00 to the status register.
}

