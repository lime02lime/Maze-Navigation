
#include "dc_motor.h"
// function initialise T2 and CCP for DC motor control
void initDCmotorsPWM(unsigned int PWMperiod){
    //initialise TRIS and LAT registers for PWM
    TRISEbits.TRISE2 = 0;
    TRISEbits.TRISE4 = 0;
    TRISCbits.TRISC7 = 0;
    TRISGbits.TRISG6 = 0;
    
    //configure PPS to map CCP modules to pins
    RE2PPS=0x05; //CCP1 on RE2
    RE4PPS=0x06; //CCP2 on RE4
    RC7PPS=0x07; //CCP3 on RC7
    RG6PPS=0x08; //CCP4 on RG6

    // timer 2 config
    T2CONbits.CKPS=0b100; // 1:16 prescaler
    T2HLTbits.MODE=0b00000; // Free Running Mode, software gate only
    T2CLKCONbits.CS=0b0001; // Fosc/4

    // Tpwm*(Fosc/4)/prescaler - 1 = PTPER
    // 0.0001s*16MHz/16 -1 = 99
    T2PR=PWMperiod; //Period reg 10kHz base period
    T2CONbits.ON=1;
    
    //setup CCP modules to output PMW signals
    //initial duty cycles 
    CCPR1H=0; 
    CCPR2H=0; 
    CCPR3H=0; 
    CCPR4H=0; 
    
    //use tmr2 for all CCP modules used
    CCPTMRS0bits.C1TSEL=0;
    CCPTMRS0bits.C2TSEL=0;
    CCPTMRS0bits.C3TSEL=0;
    CCPTMRS0bits.C4TSEL=0;
    
    //configure each CCP
    CCP1CONbits.FMT=1; // left aligned duty cycle (we can just use high byte)
    CCP1CONbits.CCP1MODE=0b1100; //PWM mode  
    CCP1CONbits.EN=1; //turn on
    
    CCP2CONbits.FMT=1; // left aligned
    CCP2CONbits.CCP2MODE=0b1100; //PWM mode  
    CCP2CONbits.EN=1; //turn on
    
    CCP3CONbits.FMT=1; // left aligned
    CCP3CONbits.CCP3MODE=0b1100; //PWM mode  
    CCP3CONbits.EN=1; //turn on
    
    CCP4CONbits.FMT=1; // left aligned
    CCP4CONbits.CCP4MODE=0b1100; //PWM mode  
    CCP4CONbits.EN=1; //turn on
}

// function to set CCP PWM output from the values in the motor structure
void setMotorPWM(DC_motor *m)
{
    unsigned char posDuty, negDuty; //duty cycle values for different sides of the motor
    
    if(m->brakemode) {
        posDuty=m->PWMperiod - ((unsigned int)(m->power)*(m->PWMperiod))/100; //inverted PWM duty
        negDuty=m->PWMperiod; //other side of motor is high all the time
    }
    else {
        posDuty=0; //other side of motor is low all the time
		negDuty=((unsigned int)(m->power)*(m->PWMperiod))/100; // PWM duty
    }
    
    if (m->direction) {
        *(m->posDutyHighByte)=posDuty;  //assign values to the CCP duty cycle registers
        *(m->negDutyHighByte)=negDuty;       
    } else {
        *(m->posDutyHighByte)=negDuty;  //do it the other way around to change direction
        *(m->negDutyHighByte)=posDuty;
    }
}

// =====================
// Motor movement involving stopping
// =====================


//function to stop the robot gradually - note it is agnostic to direction
void stop(DC_motor *mL, DC_motor *mR)
{
    // Look to see what current power is and floor it to nearest 10
    char current_power = mL->power;
    // Decreases speed in increments of 10 every 100 ms
    for (int i=current_power; i>= 0; i--) {
        mL->power = i;
        mR->power = i;
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(1);
    }
}
// Function stopping motor more slowly - needed when travelling when higher speeds to save transmission
void slowStop(DC_motor *mL, DC_motor *mR)
{
    // Look to see what current power is and floor it to nearest 10
    char current_power = mL->power;
    // Decreases speed in increments of 10 every 100 ms
    for (int i=current_power; i>= 0; i--) {
        mL->power = i;
        mR->power = i;
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(5);
    }
}

// =====================
// Motor movement involving turning
// =====================


void turn(DC_motor *mL, DC_motor *mR, char direction, char max_power, unsigned int max_power_hold) {
    // Check if motors are on, if they are then stop the car
    if (mL->power != 0 || mR->power != 0) {
        slowStop(mL, mR);
    }
    
    // direction=0 is left, direction=1 is right
    if (direction == 0) {
        // Sets motors to turn in opposite directions to maintain buggy position
        mL->direction = 0;
        mR->direction = 1;
    }
    else {
        mL->direction = 1;
        mR->direction = 0;
    }
    
    // Gradually increase power in both motors to max_power
    for (int i = 0; i < max_power; i++) {
        mL->power = i;
        mR->power = i;
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(12);
        
    }
    // holds maximum power for max_power_hold increments
    increment = 0;
    while (increment < max_power_hold);
    
    // Gradually decreases power in the motors to a stop
    for (int i = max_power; i >= 0; i--) {
        mL->power = i;
        mR->power = i;
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(5);
        
    }
    
}

void turnLeft(DC_motor *mL, DC_motor *mR) {
    turn(mL, mR, 0, turnLeftPower, 0.280 * 64); // max power hold is in increments, to convert from ms to increments multiply by 8
}
void turnRight(DC_motor *mL, DC_motor *mR) {
    turn(mL, mR, 1, turnRightPower, 0.250 * 64);
}
void turn180(DC_motor *mL, DC_motor *mR) {
    turn(mL, mR, 0, turnLeftPower, 0.68 * 64);
//    turn(mL, mR, 0, turnLeftPower, 0.280 * 64);
//    turn(mL, mR, 0, turnLeftPower, 0.280 * 64);
}
void turnLeft135(DC_motor *mL, DC_motor *mR) {
    turn(mL, mR, 0, (turnLeftPower*10/9)-1, 0.410 * 64);
}
void turnRight135(DC_motor *mL, DC_motor *mR) {
    turn(mL, mR, 1, turnRightPower*(10/9), 0.410 * 64);
}

// =====================
// Motor movement involving going in straight lines
// =====================

void move(DC_motor *mL, DC_motor *mR, char direction, char power, int increments) { // Setting a negative value of increments will mean not timing the movement
    int start_increment = increment;
    // Set motor directions
    mL->direction = direction;
    mR->direction = direction;
    // Get current power
    char current_power = mL->power;
    char trundle_power = power;
    // Increase speed in increments of 1 every 5 ms
    if (trundle_power > current_power) {
        for (int i=current_power; i<= trundle_power; i++) {
            mL->power = i;
            mR->power = i;
            setMotorPWM(mL);
            setMotorPWM(mR);
            __delay_ms(5);
        }
    }
    else {
        for (int i=current_power; i>= trundle_power; i--) {
            mL->power = i;
            mR->power = i;
            setMotorPWM(mL);
            setMotorPWM(mR);
             __delay_ms(5);
        }
    }
    if (increments < 0) { // Checks to see if you want to time this movement
        // Let it keep trundling
    }
    else {
        while (increment-start_increment < increments);
        stop(mL, mR);
    }
}

void trundle(DC_motor *mL, DC_motor *mR) {
    move(mL, mR, 1, 18, -1);
}
// A square is a time interval of 22 
void trundleSquare(DC_motor *mL, DC_motor *mR, char direction) {
    move(mL, mR, direction, 20, 22 * 8);
}
void timed_trundle(DC_motor *mL, DC_motor *mR, int increments) {
    move(mL, mR, 1, 18, increments);
}
void creep(DC_motor *mL, DC_motor *mR, int increments, char direction) {
    move(mL, mR, direction, 12, increments);
}


