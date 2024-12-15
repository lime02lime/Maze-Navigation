#ifndef _color_H
#define _color_H

#include <xc.h>
#include "dc_motor.h"
#include "i2c.h"
#include "lights.h"

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
 *  Creating structures for color values that have been read.
 ***********************************************/
//definition of RGBC structure
typedef struct colors { 
    unsigned int red; //value read for RED
    unsigned int green; //value read for GREEN
    unsigned int blue; //value read for BLUE
    unsigned int clear; //value read for CLEAR
} colors;

//Structure for the normalised colour values:
typedef struct normColors { 
    unsigned int normRed;
    unsigned int normGreen;
    unsigned int normBlue;
    unsigned int clear;
} normColors;

// Functions involved in reading colour channels and deciding colour
void readColors(colors *RGBC);
void normalizeColors(colors *RGBC, normColors *normRGB);
char decideColor(normColors *normRGB, colors * RGBC, DC_motor *mL, DC_motor *mR);

#endif