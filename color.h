#ifndef _color_H
#define _color_H

#include <xc.h>

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
typedef struct RGBC { //definition of RGBC structure
    unsigned int red; //value read for RED
    unsigned int green; //value read for GREEN
    unsigned int blue; //value read for BLUE
    unsigned int clear; //value read for CLEAR
} RGBC;

#endif
