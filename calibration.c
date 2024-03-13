#include "calibration.h"

int max(int x, int y) {
    if (x > y) {
        return x;
    }
    else {
        return y;
    }
}

unsigned int calibrate_brightness_sensor(DC_motor *mL, DC_motor* mR) {
    while (PORTFbits.RF2);
    __delay_ms(1000);
    LEDturnON();
    unsigned int b1 = readClearColor();
    turnRight(mL, mR, turnRightPower);
    unsigned int b2 = readClearColor();
    turnRight(mL, mR, turnRightPower);
    unsigned int b3 = readClearColor();
    turnRight(mL, mR, turnRightPower);
    unsigned int b4 = readClearColor();
    turnRight(mL, mR, turnRightPower);
    LEDturnOFF();
    
    // Finding maximum
    unsigned int leftmax = max(b1, b2);
    unsigned int rightmax = max(b3, b4);
    unsigned int finalmax = max(leftmax, rightmax);

    LEDturnON();
    while (PORTFbits.RF2); //wait until button press to start read of black card in front of car
    unsigned int blackBrightness = readClearColor();
    
    if (finalmax > blackBrightness) {
        turnRight(mL, mR, turnRightPower);
        turnLeft(mL, mR, turnLeftPower);
        while (PORTFbits.RF2); //wait for button press (something has gone wrong if this happens)
    } else {
        unsigned int average = (finalmax + blackBrightness) / 2;
        __delay_ms(1000);
        while (PORTFbits.RF2);
        return average;
    }
    
}