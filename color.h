#ifndef _color_H
#define _color_H

#include <xc.h>
#include "dc_motor.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

/********************************************//**
 *  Function to initialise the colour click module using I2C
 ***********************************************/
void color_click_init(void);

/********************************************//**
 *  Function to write to the colour click module
 *  address is the register within the colour click to write to
 *	value is the value that will be written to that address
 ***********************************************/
void color_writetoaddr(char address, char value);
unsigned int color_readfromaddress(char address);
unsigned int color_readdoublefromaddress(char address);
/********************************************//**
 *  Function to read the COLOR channels
 *	Returns a 16 bit ADC value representing colour intensity
 ***********************************************/
unsigned int readRedColor(void);
unsigned int readGreenColor(void);
unsigned int readBlueColor(void);
unsigned int readClearColor(void);


/********************************************//**
 *  Creating a structure of the color values that have been read.
 ***********************************************/
typedef struct colors { //definition of RGBC structure
    unsigned int red; //value read for RED
    unsigned int green; //value read for GREEN
    unsigned int blue; //value read for BLUE
    unsigned int clear; //value read for CLEAR
//    unsigned int gb;
//    unsigned int rg;
//    unsigned int rb;
} colors;

#endif

typedef struct normColors { 
    unsigned int normRed;
    unsigned int normGreen;
    unsigned int normBlue;
    unsigned int clear;
} normColors;

void readColors(colors *RGBC);
void normalizeColors(colors *RGBC, normColors *normRGB);
char decideColor(normColors *normRGB, colors * RGBC, DC_motor *mL, DC_motor *mR);
//unsigned int decideColor(normColors *normRGB);
