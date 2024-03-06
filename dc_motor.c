
#include "dc_motor.h"
// function initialise T2 and CCP for DC motor control
void initDCmotorsPWM(unsigned int PWMperiod){
    //initialise your TRIS and LAT registers for PWM
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
    T2CONbits.CKPS=0b100; // 1:??? prescaler
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

//function to stop the robot gradually - note it is agnostic to direction
void stop(DC_motor *mL, DC_motor *mR)
{
    // Look to see what current power is and floor it to nearest 10
    unsigned int current_power = mL->power;
    // Decreases speed in increments of 10 every 100 ms
    for (int i=current_power; i>= 0; i--) {
        mL->power = i;
        mR->power = i;
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(5);
    }
}

void fastStop(DC_motor *mL, DC_motor *mR)
{
    // Look to see what current power is and floor it to nearest 10
    unsigned int current_power = mL->power;
    // Decreases speed in increments of 10 every 100 ms
    for (int i=current_power; i>= 0; i--) {
        mL->power = i;
        mR->power = i;
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(1);
    }
}

//function to make the robot turn left 
void turnLeft(DC_motor *mL, DC_motor *mR)
{
    // Check if motors are on, if they are then stop the car
    if (mL->power != 0 || mR->power != 0) {
        stop(mL, mR);
    }
    
    // Sets motors to turn in opposite directions to maintain buggy position
    mL->direction = 0;
    mR->direction = 1;
    int maxpower = 40;
    
    // Gradually increases power in the motors
    for (int i = 0; i < maxpower; i++) {
        mL->power = i;
        mR->power = i;
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(5);
        
    }
    // holds maximum power
    __delay_ms(325);
    
    // Gradually decreases power in the motors to a stop
    for (int i = maxpower; i >= 0; i--) {
        mL->power = i;
        mR->power = i;
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(5);
        
    }

}

//function to make the robot turn right 
void turnRight(DC_motor *mL, DC_motor *mR)
{   
    // If motors are on then stop the car
    if (mL->power != 0 || mR->power != 0) {
        stop(mL, mR);
    }
    
     // Sets motors to turn in opposite directions to maintain buggy position
    mL->direction = 1;
    mR->direction = 0;
    int maxpower = 40;
    
    // Gradually increases power in the motors
    for (int i = 0; i < maxpower; i++) {
        mL->power = i;
        mR->power = i;
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(5);
        
    }
    // holds maximum power
    __delay_ms(270);
    
    // Gradually decreases power in the motors to a stop
    for (int i = maxpower; i >= 0; i--) {
        mL->power = i;
        mR->power = i;
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(5);
        
    }
    
    // Old attempt
//    mL->power = 50;
//    mL->direction = 0;
//    mR->power = 50;
//    mR->direction = 1;
//    setMotorPWM(mL);
//    setMotorPWM(mR);
//    __delay_ms(270);
//    mL->power = 0;
//    mR->direction = 1;
//    mR->power = 0;
//    setMotorPWM(mL);
//    setMotorPWM(mR);
//    __delay_ms(1500);
}

//function to make the robot turn left 
void turn180(DC_motor *mL, DC_motor *mR)
{
    // Check if motors are on, if they are then stop the car
    if (mL->power != 0 || mR->power != 0) {
        stop(mL, mR);
    }
    
    // Sets motors to turn in opposite directions to maintain buggy position
    mL->direction = 0;
    mR->direction = 1;
    int maxpower = 40;
    
    // Gradually increases power in the motors
    for (int i = 0; i < maxpower; i++) {
        mL->power = i;
        mR->power = i;
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(5);
        
    }
    // holds maximum power
    __delay_ms(500);
    
    // Gradually decreases power in the motors to a stop
    for (int i = maxpower; i >= 0; i--) {
        mL->power = i;
        mR->power = i;
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(5);
        
    }

}

//function to make the robot turn left 
void turnLeft135(DC_motor *mL, DC_motor *mR)
{
    // Check if motors are on, if they are then stop the car
    if (mL->power != 0 || mR->power != 0) {
        stop(mL, mR);
    }
    
    // Sets motors to turn in opposite directions to maintain buggy position
    mL->direction = 0;
    mR->direction = 1;
    int maxpower = 40;
    
    // Gradually increases power in the motors
    for (int i = 0; i < maxpower; i++) {
        mL->power = i;
        mR->power = i;
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(5);
        
    }
    // holds maximum power
    __delay_ms(410);
    
    // Gradually decreases power in the motors to a stop
    for (int i = maxpower; i >= 0; i--) {
        mL->power = i;
        mR->power = i;
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(5);
        
    }

}

//function to make the robot turn left 
void turnRight135(DC_motor *mL, DC_motor *mR)
{
    // Check if motors are on, if they are then stop the car
    if (mL->power != 0 || mR->power != 0) {
        stop(mL, mR);
    }
    
    // Sets motors to turn in opposite directions to maintain buggy position
    mL->direction = 1;
    mR->direction = 0;
    int maxpower = 40;
    
    // Gradually increases power in the motors
    for (int i = 0; i < maxpower; i++) {
        mL->power = i;
        mR->power = i;
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(5);
        
    }
    // holds maximum power
    __delay_ms(410);
    
    // Gradually decreases power in the motors to a stop
    for (int i = maxpower; i >= 0; i--) {
        mL->power = i;
        mR->power = i;
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(5);
        
    }

}


//function to make the robot go straight
void fullSpeedAhead(DC_motor *mL, DC_motor *mR)
{
    // Ensure motor directions are forward
    mL->direction = 1;
    mR->direction = 1;
    // Get current power
    unsigned int current_power = mL->power;
    // Increase speed in increments of 1 every 5 ms
    for (int i=current_power; i<= 100; i++) {
        mL->power = i;
        mR->power = i;
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(5);
    }
    
}

void trundle(DC_motor *mL, DC_motor *mR)
{
//    if (mL->power != 0 || mR->power != 0) {
//        stop(mL, mR);
//    }
    // Ensure motor directions are forward
    mL->direction = 1;
    mR->direction = 1;
    // Get current power
    char current_power = mL->power;
    char trundle_power = 20;
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
    
}

void timed_trundle(DC_motor *mL, DC_motor *mR, int increments) {
    if (mL->power != 0 || mR->power != 0) {
        stop(mL, mR);
    }
    increment = 0;
    // Ensure motor directions are forward
    mL->direction = 1;
    mR->direction = 1;
    // Get current power
    unsigned int current_power = mL->power;
    // Increase speed in increments of 1 every 5 ms
    for (int i=current_power; i<= 20; i++) {
        mL->power = i;
        mR->power = i;
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(5);
    }
    
    while (increment < increments);
    stop(mL, mR);
}

// Square is a time interval
void trundleSquare(DC_motor *mL, DC_motor *mR, char square, char reverse) {
    // Make sure to bring motor to a stop it isn't already
    if (mL->power != 0 || mR->power != 0) {
        stop(mL, mR);
    }
    // Ensure motor directions are forward
    if (reverse == 1) {
        mL->direction = 0;
        mR->direction = 0;
    }
    else {
        mL->direction = 1;
        mR->direction = 1;
    }
    // Increase speed in increments of 1 every 5 ms
    for (int i=0; i<= 20; i++) {
        mL->power = i;
        mR->power = i;
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(5);
    }
    
    // Goes in reverse until time exceeds duration needed to cover square
    char start_increment = increment;
    while (increment - start_increment < square);
    stop(mL, mR);
    
}

