#ifndef _interrupts_H
#define _interrupts_H

#include <xc.h>
#include "i2c.h"
#include "color.h"
#include "interact.h"
#include "dc_motor.h"
#include "calibration.h"

extern int increment;
extern char wall_detected;
extern struct colors RGBC;
extern struct normColors normRGB;


#define _XTAL_FREQ 64000000

void interrupts_init(DC_motor *mL, DC_motor *mR, char skip_calibration);
void Timer0_init(void);
void __interrupt(high_priority) HighISR();
void clearInterrupt(void);

#endif
