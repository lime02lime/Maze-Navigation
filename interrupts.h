#ifndef _interrupts_H
#define _interrupts_H

#include <xc.h>
#include "i2c.h"
#include "color.h"
#include "interact.h"
#include "dc_motor.h"

extern int increment;
extern char wall_detected;
extern struct colors RGBC;
extern struct normColors normRGB;


#define _XTAL_FREQ 64000000

void Interrupts_init(DC_motor *mL, DC_motor *mR);
void __interrupt(high_priority) HighISR();

#endif
