#ifndef _instructions_H
#define _instructions_H

#define _XTAL_FREQ 64000000 

extern char instruction_array[20][2];
extern char instruction_array_index;
extern char reverseRouteFlag;
extern char turnLeftPower;
extern char turnRightPower;

void executeInstruction(DC_motor *mL, DC_motor *mR, char colourCode);

void Red(DC_motor *mL, DC_motor *mR);
void Green(DC_motor *mL, DC_motor *mR);
void Blue(DC_motor *mL, DC_motor *mR);
void Yellow(DC_motor *mL, DC_motor *mR);
void Pink(DC_motor *mL, DC_motor *mR);
void Orange(DC_motor *mL, DC_motor *mR);
void LightBlue(DC_motor *mL, DC_motor *mR);
void White(DC_motor *mL, DC_motor *mR);
void Black(DC_motor *mL, DC_motor *mR);

void reverseYellow(DC_motor *mL, DC_motor *mR);
void reversePink(DC_motor *mL, DC_motor *mR);
void reverseOrange(DC_motor *mL, DC_motor *mR);
void reverseLightBlue(DC_motor *mL, DC_motor *mR);
void reverseRoute(DC_motor *mL, DC_motor *mR);

#endif