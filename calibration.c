#include "calibration.h"



// Calibrates brightness
void calibrate_brightness_sensor(DC_motor *mL, DC_motor* mR) {
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
        // If ambient brightness is brighter than brightness with black card, car will shake its head, because that shouldn't happen - black cannot be detected by brightness interrupt now
        turnRight(mL, mR);
        turnLeft(mL, mR);
        // Instead set a default value
        color_writetoaddr(0x06, 0b00000000); //upper thresh, lower byte 
        color_writetoaddr(0x07, 0b00000010); //upper  thresh, upper byte 
        while (PORTFbits.RF2); //wait for button press
    } else {
        // Take interrupt threshold as midpoint between ambient brightness and brightness with motor reading black card
        unsigned int average = (finalmax + blackBrightness) / 2;
        __delay_ms(1000);
        
        // Extract upper and lower threshold from answer
        char upper_threshold = (average & 0b1111111100000000) >> 8;
        char lower_threshold = average & 0b0000000011111111;

        // Write thresholds to clicker
        color_writetoaddr(0x06, lower_threshold); //upper thresh, lower byte
        color_writetoaddr(0x07, upper_threshold); //upper  thresh, upper byte
        while (PORTFbits.RF2);
    }
    
}

// Motor calibration left turn
void leftCali(DC_motor *mL, DC_motor *mR){
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
void rightCali(DC_motor *mL, DC_motor *mR){
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
unsigned int max(unsigned int x, unsigned int y) {
    if (x > y) {
        return x;
    }
    else {
        return y;
    }
}