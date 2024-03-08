

#include "feedback.h"

void initBoardLEDs(void) {
    TRISDbits.TRISD7 = 0;
    LATDbits.LATD7 = 0;
    TRISHbits.TRISH3 = 0;
    LATHbits.LATH3 = 0;
}

void initButtons(void) {
    TRISFbits.TRISF2 = 1;
    ANSELFbits.ANSELF2=0; //turn off analogue input on pin
    TRISFbits.TRISF3 = 1;
    ANSELFbits.ANSELF3=0; //turn off analogue input on pin
}


void toggleLEDD7(void) {
    int current = LATDbits.LATD7;
    if (current == 0) {
        LATDbits.LATD7 = 1;
    }
    else {
        LATDbits.LATD7 = 0;

    }
}

void indicateInstruction(char period) {
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < period; j++) {
            LATHbits.LATH3 = 1;
            __delay_ms(150);
            LATHbits.LATH3 = 0;
            __delay_ms(150);
        }
        __delay_ms(500);
    }
}

void checkBattery(void) {

    TRISFbits.TRISF6=1; // Select pin RA3 as input
    ANSELFbits.ANSELF6=1; //Ensure analogue circuitry is active (it is by default - watch out for this later in the course!)

    // Set up the ADC module - check section 33 of the datasheet for more details
    ADREFbits.ADNREF = 0; // Use Vss (0V) as negative reference
    ADREFbits.ADPREF = 0b00; // Use Vdd (3.3V) as positive reference
    ADPCH=0b101110; // Select channel RF6/ANF6 for ADC  
    ADCON0bits.ADFM = 0; // Left-justified result (i.e. no leading 0s)
    ADCON0bits.ADCS = 1; // Use internal Fast RC (FRC) oscillator as clock source for conversion
    ADCON0bits.ADON = 1; // Enable ADC
    
    ADCON0bits.GO = 1; // Start ADC conversion
    while (ADCON0bits.GO); // Wait until conversion done (bit is cleared automatically when done)
    int tmpval = ADRESH; // Read 8 most significant bits 
    char max_typical = 0b01101101;
    char percent = tmpval * 100 / max_typical;
    while (tmpval >= 0) {
        LATHbits.LATH3 = 1;
        __delay_ms(150);
        LATHbits.LATH3 = 0;
        __delay_ms(150);
        tmpval -= 10;
    }
    
}

// If you still want your old instructions

//            if (colourCode == 1) {
//                TRISHbits.TRISH3 = 0;
//                for (char i=0; i<1; i++) {
//                    LATHbits.LATH3 = 1;
//                    __delay_ms(150);
//                    LATHbits.LATH3 = 0;
//                    __delay_ms(100);
//                }
//            }
//            
//            if (colourCode == 2) {
//                TRISHbits.TRISH3 = 0;
//                for (char i=0; i<2; i++) {
//                    LATHbits.LATH3 = 1;
//                    __delay_ms(150);
//                    LATHbits.LATH3 = 0;
//                    __delay_ms(100);
//                }
//            }
//            
//            if (colourCode == 3) {
//                TRISHbits.TRISH3 = 0;
//                for (char i=0; i<3; i++) {
//                    LATHbits.LATH3 = 1;
//                    __delay_ms(150);
//                    LATHbits.LATH3 = 0;
//                    __delay_ms(100);
//                }
//            }
//            
//            if (colourCode == 4) {
//                TRISHbits.TRISH3 = 0;
//                for (char i=0; i<2; i++) {
//                    LATHbits.LATH3 = 1;
//                    __delay_ms(150);
//                    LATHbits.LATH3 = 0;
//                    __delay_ms(150);
//                }
//                LATDbits.LATD7 = 1;
//                __delay_ms(150);
//                LATDbits.LATD7 = 0;
//                __delay_ms(150);
//            }
//            
//            if (colourCode == 6) {
//                TRISHbits.TRISH3 = 0;
//                for (char i=0; i<3; i++) {
//                    LATHbits.LATH3 = 1;
//                    __delay_ms(150);
//                    LATHbits.LATH3 = 0;
//                    __delay_ms(150);
//                }
//                LATDbits.LATD7 = 1;
//                __delay_ms(150);
//                LATDbits.LATD7 = 0;
//                __delay_ms(150);
//            }