#ifndef _DC_MOTOR_H
#define _DC_MOTOR_H

#include <xc.h>
#include "dc_motor.h"
#define _XTAL_FREQ 64000000
extern int increment;

typedef struct DC_motor { //definition of DC_motor structure
    char power;         //motor power, out of 100
    char direction;     //motor direction, forward(1), reverse(0)
    char brakemode;		// short or fast decay (brake or coast)
    unsigned int PWMperiod; //base period of PWM cycle
    unsigned char *posDutyHighByte; //PWM duty address for motor +ve side
    unsigned char *negDutyHighByte; //PWM duty address for motor -ve side
} DC_motor;

//function prototypes
void initDCmotorsPWM(unsigned int PWMperiod); // function to setup PWM
void setMotorPWM(DC_motor *m);
void stop(DC_motor *mL, DC_motor *mR);
void fastStop(DC_motor *mL, DC_motor *mR);
void turnLeft(DC_motor *mL, DC_motor *mR);
void turnRight(DC_motor *mL, DC_motor *mR);
void fullSpeedAhead(DC_motor *mL, DC_motor *mR);
void trundle(DC_motor *mL, DC_motor *mR);
void trundleSquare(DC_motor *mL, DC_motor *mR, char square, char reverse);
void timed_trundle(DC_motor *mL, DC_motor *mR, int increments);
void turn180(DC_motor *mL, DC_motor *mR);
void turnLeft135(DC_motor *mL, DC_motor *mR);
void turnRight135(DC_motor *mL, DC_motor *mR);
void creep(DC_motor *mL, DC_motor *mR, int increments, char direction);

#endif
