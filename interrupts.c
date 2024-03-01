#include <xc.h>
#include "interrupts.h"
#include "i2c.h"
#include "color.h"
#include "interact.h"

extern int increment;

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
    color_writetoaddr(0x06, 0x00); //upper thresh, lower byte (used to be 0b11010110 - was no need to be so precise)
    color_writetoaddr(0x07, 0b00000011); //upper  thresh, upper byte
    //set persistence register
    color_writetoaddr(0x0C, 0b0111); //1 clear channel value outside of threshold range will trigger interrupt.
    
//    INTCONbits.IPEN=1; //enable priority levels on interrupts
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
    T0CON1bits.T0CKPS=0b0000; // 1:256 is 0b1000 - needed for one increment per second, for testing set to 0b0000, for real operation set to 0b1100
    T0CON0bits.T016BIT=1;	//8bit mode	
	
    // Working from lab 3
    // Note that 2^16 values can be taken in 16 bit counter (0 up to 2^16 - 1)
    // 2^16 = 65536
    // f_interrupt = 64000000/(4*Prescaler*2^16)
    // Prescaler at 256 brings us closest f_interrupt = 0.95
    // 64000000/(4*256) = 62500
    // Need to start count at: 65536-62500=3036
    // Considering this gives an interrupt per second, can slow down to 1 
    // interrupt every 16 seconds through 256 * 16 = 4096 -> 0b1100 for TOCKPS
    
    // Initialising timer registers
    TMR0H=0;            //write High reg first, update happens when low reg is written to
    TMR0L=3036;
    T0CON0bits.T0EN=1;	//start the timer
}
    
void __interrupt(high_priority) High_ISR() {
    if (PIR0bits.INT0IF) {
//    if ((readInterrupt() & 0b0010000) != 0) { //here we check if the interrupt bit is raised.
//        LATDbits.LATD7 = 1;
//        __delay_ms(1000);
//        LATDbits.LATD7 = 0;
//        __delay_ms(1000);
//        
        LATDbits.LATD7 = 0;
        // Clear interrupt
        int w = PORTB;
        int x = color_readfromaddress(0x13);
        clearInterrupt();
        x = color_readfromaddress(0x13);
        // May also need to do this:
        PIR0bits.INT0IF = 0;
    }
    
    if (PIR0bits.TMR0IF) {
        PIR0bits.TMR0IF = 0;
        increment++;
        
    }
    
}
    
//DONT KNOW IF WORKS:
//this SHOULD return a byte containing a 1 in the 5th bit if the interrupt is raised (0b00010000.
//unsigned int readInterrupt(void)
//{
//	unsigned int tmp;
//	I2C_2_Master_Start();         //Start condition
//	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + write mode (0x27 was the color clicker address, but we do <<1 to add a 0 at the end which configures "write", this yields 0x52)
//	I2C_2_Master_Write(0x80 | 0x13);    //command (repeated byte protocol transaction) addressed to the status register (0x13)
//	I2C_2_Master_RepStart();			// start a repeated transmission
//	I2C_2_Master_Write(0x52 | 0x01);     //7 bit address + Read (1) mode
//	tmp=I2C_2_Master_Read(1);			//read the status register byte
//
//	I2C_2_Master_Stop();          //Stop condition
//	return tmp;
//}


void clearInterrupt(void){
    I2C_2_Master_Start();         //Start condition
    I2C_2_Master_Write(0x52 | 0x00);     //7 bit device address + Write mode
    I2C_2_Master_Write(0xe0 | 0x06 );    //command + register address needs 0b11100110
    I2C_2_Master_Stop(); 
//    color_writetoaddr(0x13, 0x00); //write 00 to the status register.
}

void toggleLED(void) {
    int current = LATDbits.LATD7;
    if (current == 0) {
        LATDbits.LATD7 = 1;
    }
    else {
        LATDbits.LATD7 = 0;

    }
}