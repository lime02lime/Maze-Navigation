#include "calibration.h"



// Calibrates brightness
unsigned int calibrate_brightness_sensor(DC_motor *mL, DC_motor* mR) {
    // Wait for user input to start calibration routine
    while (PORTFbits.RF2);
    __delay_ms(1000);
    
    // Turn four times and read clear brightness in each direction
    LEDturnON();
    unsigned int b1 = readClearColor();
    turnRight(mL, mR);
    unsigned int b2 = readClearColor();
    turnRight(mL, mR);
    unsigned int b3 = readClearColor();
    turnRight(mL, mR);
    unsigned int b4 = readClearColor();
    turnRight(mL, mR);
    LEDturnOFF();
    
    // Find maximum brightness of all directions
    unsigned int leftmax = max(b1, b2);
    unsigned int rightmax = max(b3, b4);
    unsigned int finalmax = max(leftmax, rightmax);

    LEDturnON();
    //wait for button press to start read of black card in front of car
    while (PORTFbits.RF2); 
    unsigned int blackBrightness = readClearColor();
    
    if (finalmax > blackBrightness) {
        // If ambient brightness is brighter than brightness with black card, car will shake its head, because that shouldn't happen
        turnRight(mL, mR);
        turnLeft(mL, mR);
        while (PORTFbits.RF2); //wait for button press (something has gone wrong if this happens)
    } else {
        // Take interrupt threshold as midpoint between ambient brightness and brightness with motor reading black card
        unsigned int average = (finalmax + blackBrightness) / 2;
        __delay_ms(1000);
        while (PORTFbits.RF2);
        return average;
    }
    
}

// Motor calibration left turn
char leftCali(DC_motor *mL, DC_motor *mR){
    // Wait for button press to start
    while(PORTFbits.RF2);
    while (PORTFbits.RF2 || PORTFbits.RF3) { //while both buttons aren't pressed, continue calibrating.
        __delay_ms(1000);
        turnLeft(mL,mR);
        // Wait for at least one of buttons to be pressed
        while (PORTFbits.RF2 && PORTFbits.RF3);
        // Small delay to give time for both buttons to be pressed if double pressing
        __delay_ms(80);
        if(!PORTFbits.RF2 && !PORTFbits.RF3) {
            // Do nothing, will now exit current while loop because calibration accepted
        } 
        else{
            if(!PORTFbits.RF2){ // decrease current turning power
                turnLeftPower--;
            }
            else{ // increase current turning power
                turnLeftPower++;
            }
        }
    }
}

// Motor calibration right turn
char rightCali(DC_motor *mL, DC_motor *mR){
    // Wait for button press to start
    while(PORTFbits.RF2);
    while (PORTFbits.RF2 || PORTFbits.RF3) { //while both buttons aren't pressed, continue calibrating.
        __delay_ms(1000);
        turnRight(mL,mR);
        // Wait for at least one of buttons to be pressed
        while (PORTFbits.RF2 && PORTFbits.RF3);
        // Small delay to give time for both buttons to be pressed if double pressing
        __delay_ms(80);
        if(!PORTFbits.RF2 && !PORTFbits.RF3) {
            // Do nothing, will now exit current while loop because calibration accepted
        } else{
            if(!PORTFbits.RF2){ // increase current turning power
                turnRightPower++;
            }
            else{ // decrease current turning power
                turnRightPower--;
            }
        }
    }
}

// Utility function to return the maximum of two numbers
int max(int x, int y) {
    if (x > y) {
        return x;
    }
    else {
        return y;
    }
}