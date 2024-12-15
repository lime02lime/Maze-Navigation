#ifndef _calibration_H
#define _calibration_H

#define _XTAL_FREQ 64000000 

#include <xc.h>
#include "dc_motor.h"
#include "color.h"
#include "lights.h"

extern char turnLeftPower;
extern char turnRightPower;

// Calibration functions
void calibrate_brightness_sensor(DC_motor *mL, DC_motor* mR);
void leftCali(DC_motor *mL, DC_motor *mR);
void rightCali(DC_motor *mL, DC_motor *mR);
unsigned int max(unsigned int x, unsigned int y);

#endif