#include <xc.h>
#include "interrupts.h"
#include "i2c.h"
#include "color.h"

//DONT KNOW IF WORKS:
void interrupts_init(void)
{
    clearInterrupt();
//****************************************
    //CONFIGURE GENERAL INTERRUPTS BELOW
//****************************************
    // turn on global interrupts, peripheral interrupts and the interrupt source 
    INTCONbits.IPEN=1; //enable priority levels on interrupts
    INTCONbits.PEIE=1; //peripheral interrupts enable
    PIE8bits.SCANIE=1; //enable memory scanning interrupt
    
    
    
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
    
    
    
    // It's a good idea to turn on global interrupts last, once all other interrupt configuration is done.
    INTCONbits.GIE=1; 	//turn on interrupts GLOBALLY (when this is off, all interrupts are deactivated)
}
    
    
//DONT KNOW IF WORKS:
void __interrupt(high_priority) wall_detected() {
    if ((readInterrupt() & 0b0010000) != 0) { //here we check if the interrupt bit is raised.
        LATDbits.LATD7 = 1;
        __delay_ms(1000);
        LATDbits.LATD7 = 0;
        __delay_ms(1000);
        
        
        //FINALLY WE MUST CLEAR THE INTERRUPT
        clearInterrupt();
    }
    
}
    
//DONT KNOW IF WORKS:
//this SHOULD return a byte containing a 1 in the 5th bit if the interrupt is raised (0b00010000.
unsigned int readInterrupt(void)
{
	unsigned int tmp;
	I2C_2_Master_Start();         //Start condition
	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + write mode (0x27 was the color clicker address, but we do <<1 to add a 0 at the end which configures "write", this yields 0x52)
	I2C_2_Master_Write(0x80 | 0x13);    //command (repeated byte protocol transaction) addressed to the status register (0x13)
	I2C_2_Master_RepStart();			// start a repeated transmission
	I2C_2_Master_Write(0x52 | 0x01);     //7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(1);			//read the status register byte

	I2C_2_Master_Stop();          //Stop condition
	return tmp;
}

//DONT KNOW IF WORKS:
void clearInterrupt(void){
    color_writetoaddr(0x13, 0x00); //write 00 to the status register.
}