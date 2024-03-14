#ifndef _feedback_H
#define _feedback_H

#include <xc.h>

#define _XTAL_FREQ 64000000 

void initBoardLEDs(void);
void indicateInstruction(char period);
void initButtons(void);
void checkBattery(void);

#endif