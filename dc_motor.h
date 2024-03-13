#ifndef _DC_MOTOR_H
#define _DC_MOTOR_H

#include <xc.h>
#include "dc_motor.h"
#define _XTAL_FREQ 64000000

extern int increment;
extern char turnLeftPower;
extern char turnRightPower;
extern char square;

//definition of DC_motor structure
typedef struct DC_motor { 
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
void slowStop(DC_motor *mL, DC_motor *mR);
// Turning functions
void turn(DC_motor *mL, DC_motor *mR, char direction, char max_power, char max_power_hold);
void turnLeft(DC_motor *mL, DC_motor *mR);
void turnRight(DC_motor *mL, DC_motor *mR);
void turnLeft135(DC_motor *mL, DC_motor *mR);
void turnRight135(DC_motor *mL, DC_motor *mR);
void turn180(DC_motor *mL, DC_motor *mR);
// Functions for going forwards and backwards
void fullSpeedAhead(DC_motor *mL, DC_motor *mR);
void trundle(DC_motor *mL, DC_motor *mR);
void trundleSquare(DC_motor *mL, DC_motor *mR, char direction);
void timed_trundle(DC_motor *mL, DC_motor *mR, int increments);
void creep(DC_motor *mL, DC_motor *mR, int increments, char direction);


#endif
