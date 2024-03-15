
#include "dc_motor.h"
#include "instructions.h"


//Here we use a Switch to execute the instructions associated with each colour.
//Each colour has been given a code from 0-8 (same order as in the table in the github README)
// Extra codes added for reversal instructions
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

// =========================
// The functions below give the specific instructions associated with each colour
// =========================
void Red(DC_motor *mL, DC_motor *mR){
    turnRight(mL, mR);
}

void Green(DC_motor *mL, DC_motor *mR) {
    turnLeft(mL, mR);
}

void Blue(DC_motor *mL, DC_motor *mR) {
    turn180(mL, mR);
}

void Yellow(DC_motor *mL, DC_motor *mR) {
    trundleSquare(mL, mR, 0);
    turnRight(mL, mR);
}

void Pink(DC_motor *mL, DC_motor *mR) {
    trundleSquare(mL, mR, 0);
    turnLeft(mL, mR);

}

void Orange(DC_motor *mL, DC_motor *mR) {
    turnRight135(mL, mR);
}

void LightBlue(DC_motor *mL, DC_motor *mR) {
    turnLeft135(mL, mR);
}

void White(DC_motor *mL, DC_motor *mR) {
    reverseRouteFlag=1;
    turn180(mL, mR);
}

void Black(DC_motor *mL, DC_motor *mR) {
    reverseRouteFlag=1;
    turn180(mL, mR);
}

// =======================
// Instructions for the reverse route (that can't be replicated with the above functions alone)
// =======================

void reverseYellow(DC_motor *mL, DC_motor *mR) {
    turnRight(mL, mR);
    trundleSquare(mL, mR, 1);
    turn180(mL, mR);
}

void reversePink(DC_motor *mL, DC_motor *mR) {
    turnLeft(mL, mR);
    trundleSquare(mL, mR, 1);
    turn180(mL, mR);
}

void reverseOrange(DC_motor *mL, DC_motor *mR) {
    turnLeft135(mL, mR);
}

void reverseLightBlue(DC_motor *mL, DC_motor *mR) {
    turnRight135(mL, mR);
}


// Function for the buggy to retrace its steps to return home.
void reverseRoute(DC_motor *mL, DC_motor *mR) {
    
    // Reverse mapping where index corresponds to colour code
    char reverseMapping[9] = {1, 0, 2, 9, 10, 6, 5, -1, -1}; // -1 won't execute any instruction, only start the trundling
    for (int i = (instruction_array_index-1); i >= 0; i--) {
        int time;
        if (instruction_array[i][0] == 3) { // YELLOW
            time = (instruction_array[i][1] * 8) - 40;
            int adjusted_time = time - 22 * 8; // We know from motor.c that square is 22*8 increments
            if (adjusted_time >= 0) {
                turnLeft(mL, mR);
                timed_trundle(mL, mR, adjusted_time);
            }
            else {
                turnRight(mL, mR);
                timed_trundle(mL, mR, adjusted_time);
                turn180(mL, mR); // since will otherwise be facing opposite direction to where it would be if following the proper route
            }
        }
        if (instruction_array[i][0] == 4) { // PINK
            time = (instruction_array[i][1] * 8) - 40;
            int adjusted_time = time - 22 * 8; // We know from motor.c that square is 22*8 increments
            if (adjusted_time >= 0) {
                turnRight(mL, mR);
                timed_trundle(mL, mR, adjusted_time);
            }
            else {
                turnLeft(mL, mR);
                timed_trundle(mL, mR, adjusted_time);
                turn180(mL, mR);
            }
        }
        else {
            executeInstruction(mL, mR, reverseMapping[instruction_array[i][0]]);
            time = (instruction_array[i][1] * 8) - 40;
            if (time < 0) {
                time = 0;
            }
            timed_trundle(mL, mR, time); // Constant of 40 needed to compensate for reversal after colour detection
        }
        
        stop(mL, mR); // stop the buggy
    }
    stop(mL, mR);
    instruction_array_index = 0;
}
