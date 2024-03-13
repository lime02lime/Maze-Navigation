#ifndef _calibration_H
#define _calibration_H

#define _XTAL_FREQ 64000000 

#include "dc_motor.h"
#include "color.h"
#include "interact.h"

extern char turnLeftPower;
extern char turnRightPower;

int max(int x, int y);
unsigned int calibrate_brightness_sensor(DC_motor *mL, DC_motor* mR);

#endif