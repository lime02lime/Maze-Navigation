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
unsigned int calibrate_brightness_sensor(DC_motor *mL, DC_motor* mR);
char leftCali(DC_motor *mL, DC_motor *mR);
char rightCali(DC_motor *mL, DC_motor *mR);
int max(int x, int y);

#endif