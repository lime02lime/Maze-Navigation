
#include "dc_motor.h"
#include "instructions.h"


//Here we use a Switch to execute the instructions associated with each colour.
//Each colour has a value from 0-8.
void executeInstruction(DC_motor *mL, DC_motor *mR, char colourCode) {
    switch (colourCode) {
        case 0:
            Red(mL, mR);
            break;
        case 1:
            Green(mL, mR);
            break;
        case 2:
            Blue(mL, mR);
            break;
        case 3:
            Yellow(mL, mR);
            break;
        case 4:
            Pink(mL, mR);
            break;
        case 5:
            Orange(mL, mR);
            break;
        case 6:
            LightBlue(mL, mR);
            break;
        case 7:
            White(mL, mR);
            break;
        case 8:
            Black(mL, mR);
            break;
        case 9:
            reverseYellow(mL, mR);
            break;
        case 10:
            reversePink(mL, mR);
            break;
        case 11: 
            reverseOrange(mL, mR);
            break;
        case 12:
            reverseLightBlue(mL, mR);
            break;
    }
}

//THE FUNCTIONS BELOW GIVE THE SPECIFIC INSTRUCTIONS ASSOCIATED WITH EACH COLOUR
void Red(DC_motor *mL, DC_motor *mR){
    turnRight(mL, mR);
}

void Green(DC_motor *mL, DC_motor *mR) {
    turnLeft(mL, mR, turnLeftPower);
}

void Blue(DC_motor *mL, DC_motor *mR) {
    turn180(mL, mR);
}

void Yellow(DC_motor *mL, DC_motor *mR) {
    trundleSquare(mL, mR, square, 1);
    turnRight(mL, mR);
}

void Pink(DC_motor *mL, DC_motor *mR) {
    trundleSquare(mL, mR, square, 1);
    turnLeft(mL, mR, turnLeftPower);

}

void Orange(DC_motor *mL, DC_motor *mR) {
    turnRight135(mL, mR);
}

void LightBlue(DC_motor *mL, DC_motor *mR) {
    turnLeft135(mL, mR);
}

void White(DC_motor *mL, DC_motor *mR) {
    reverseRouteFlag=1;
}

void Black(DC_motor *mL, DC_motor *mR) {
    
}

// =======================

void reverseYellow(DC_motor *mL, DC_motor *mR) {
    turnLeft(mL, mR, turnLeftPower);
    trundleSquare(mL, mR, square, 0);
    turn180(mL, mR);
}

void reversePink(DC_motor *mL, DC_motor *mR) {
    turnRight(mL, mR);
    trundleSquare(mL, mR, square, 0);
    turn180(mL, mR);

}

void reverseOrange(DC_motor *mL, DC_motor *mR) {
    turnRight135(mL, mR);
}

void reverseLightBlue(DC_motor *mL, DC_motor *mR) {
    turnLeft135(mL, mR);
}


//This function allows the buggy to retrace its steps to return home.
void reverseRoute(DC_motor *mL, DC_motor *mR) {
    // face the way you came (in theory won't need this when we get white colour detection working)
    int last_increments = increment;
    turn180(mL, mR);
    timed_trundle(mL, mR, last_increments);
    
    // [0, 1, 2, 3, 4, 5, 6, 7, 8]
    char reverseMapping[9] = {1, 0, 2, 9, 10, 11, 12, -1, -1};
    for (int i = instruction_array_index; i >= 0; i--) {
        executeInstruction(mL, mR, reverseMapping[instruction_array[i][0]]);
        timed_trundle(mL, mR, instruction_array[i][1]);
    }
    stop(mL, mR);
    instruction_array_index = 0;
    while (PORTFbits.RF2);
}