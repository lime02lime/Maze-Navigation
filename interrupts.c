#include <xc.h>
#include "interrupts.h"


void interrupts_init(void)
{
//****************************************
    //CONFIGURE GENERAL INTERRUPTS BELOW
//****************************************
    // turn on global interrupts, peripheral interrupts and the interrupt source 
    INTCONbits.IPEN=1; //enable priority levels on interrupts
    
    
    
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
    
    
    
void __interrupt(high_priority) wall_detected() {
    
    
}
    
    
    
    