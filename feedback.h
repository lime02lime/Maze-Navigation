
#include <xc.h>

#define _XTAL_FREQ 64000000 

void toggleLEDD7(void);
void initBoardLEDs(void);
void indicateInstruction(char period);
void initButtons(void);
void checkBattery(void);