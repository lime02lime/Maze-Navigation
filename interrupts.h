#ifndef _interrupts_H
#define _interrupts_H

#include <xc.h>

#define _XTAL_FREQ 64000000


void interrupts_init(void);
void __interrupt(high_priority) High_ISR();
unsigned int readInterrupt(void);
void clearInterrupt(void);
void toggleLED(void);
void Timer0_init(void);

#endif
