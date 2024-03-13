#include <xc.h>
#include "color.h"
#include "i2c.h"
#include "interact.h"


void color_click_init(void)
{   
    //setup colour sensor via i2c interface
    I2C_2_Master_Init();      //Initialise i2c Master

    //set integration time to 24 ms
	color_writetoaddr(0x01, 0xF6);
    
    //Enable interrupts from the color clicker while turning colour sensing on and enabling RGBC
    color_writetoaddr(0x00, 0x13); 
    __delay_ms(3); //need to wait 3ms for everthing to start up
    
    }

unsigned int color_readdoublefromaddress(char address) {
    unsigned int tmp;
	I2C_2_Master_Start();         //Start condition
	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode (0x27 was the colour clicker address, but we do <<1 to add a 0 at the end which configures "write", this yields 0x52)
	I2C_2_Master_Write(0x80 | address);    //command (auto-increment protocol transaction) - read value at particular address
	I2C_2_Master_RepStart();			// start a repeated transmission
    I2C_2_Master_Write(0x52 | 0x01);     //7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(1);			//read the Red LSB
	tmp=tmp | (I2C_2_Master_Read(0)<<8); //read the Red MSB (don't acknowledge as this is the last read)			//read the Red LSB
	I2C_2_Master_Stop();          //Stop condition
	return tmp;
}

//following master-slave communication structure to READ from colour clicker.
unsigned int color_readfromaddress(char address) {
    unsigned int tmp;
	I2C_2_Master_Start();         //Start condition
	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode (0x27 was the colour clicker address, but we do <<1 to add a 0 at the end which configures "write", this yields 0x52)
	I2C_2_Master_Write(0x80 | address);    //command (auto-increment protocol transaction) - read value at particular address
	I2C_2_Master_RepStart();			// start a repeated transmission
    I2C_2_Master_Write(0x52 | 0x01);     //7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(0);			//read the Red LSB
	SSP2CON2bits.PEN = 1;           //Initiate stop condition
	return tmp;
}

//following master-slave communication structure to WRITE to colour clicker.
void color_writetoaddr(char address, char value){
    I2C_2_Master_Start();         //Start condition
    I2C_2_Master_Write(0x52 | 0x00);     //7 bit device address + Write mode
    I2C_2_Master_Write(0x80 | address);    //command + register address
    I2C_2_Master_Write(value);    
    I2C_2_Master_Stop();          //Stop condition
}

//following master-slave communication structure to READ the RED colour reading.
unsigned int readRedColor(void)
{
	unsigned int tmp;
	I2C_2_Master_Start();         //Start condition
	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode (0x27 was the colour clicker address, but we do <<1 to add a 0 at the end which configures "write", this yields 0x52)
	I2C_2_Master_Write(0xA0 | 0x16);    //command (auto-increment protocol transaction) + start at RED low register
	I2C_2_Master_RepStart();			// start a repeated transmission
	I2C_2_Master_Write(0x52 | 0x01);     //7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(1);			//read the Red LSB
	tmp=tmp | (I2C_2_Master_Read(0)<<8); //read the Red MSB (don't acknowledge as this is the last read)
	I2C_2_Master_Stop();          //Stop condition
	return tmp;
}

//following master-slave communication structure to READ the GREEN colour reading.
unsigned int readGreenColor(void)
{
	unsigned int tmp;
	I2C_2_Master_Start();         //Start condition
	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode (0x27 was the colour clicker address, but we do <<1 to add a 0 at the end which configures "write", this yields 0x52)
	I2C_2_Master_Write(0xA0 | 0x18);    //command (auto-increment protocol transaction) + start at GREEN low register
	I2C_2_Master_RepStart();			// start a repeated transmission
	I2C_2_Master_Write(0x52 | 0x01);     //7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(1);			//read the Red LSB
	tmp=tmp | (I2C_2_Master_Read(0)<<8); //read the Red MSB (don't acknowledge as this is the last read)
	I2C_2_Master_Stop();          //Stop condition
	return tmp;
}

//following master-slave communication structure to READ the BLUE colour reading.
unsigned int readBlueColor(void)
{
	unsigned int tmp;
	I2C_2_Master_Start();         //Start condition
	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode (0x27 was the colour clicker address, but we do <<1 to add a 0 at the end which configures "write", this yields 0x52)
	I2C_2_Master_Write(0xA0 | 0x1A);    //command (auto-increment protocol transaction) + start at BLUE low register
	I2C_2_Master_RepStart();			// start a repeated transmission
	I2C_2_Master_Write(0x52 | 0x01);     //7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(1);			//read the Red LSB
	tmp=tmp | (I2C_2_Master_Read(0)<<8); //read the Red MSB (don't acknowledge as this is the last read)
	I2C_2_Master_Stop();          //Stop condition
	return tmp;
}

//following master-slave communication structure to READ the CLEAR colour reading.
unsigned int readClearColor(void)
{
	unsigned int tmp;
	I2C_2_Master_Start();         //Start condition
	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode (0x27 was the colour clicker address, but we do <<1 to add a 0 at the end which configures "write", this yields 0x52)
	I2C_2_Master_Write(0xA0 | 0x14);    //command (auto-increment protocol transaction) + start at CLEAR low register
	I2C_2_Master_RepStart();			// start a repeated transmission
	I2C_2_Master_Write(0x52 | 0x01);     //7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(1);			//read the Red LSB
	tmp=tmp | (I2C_2_Master_Read(0)<<8); //read the Red MSB (don't acknowledge as this is the last read)
	I2C_2_Master_Stop();          //Stop condition
	return tmp;
}


//normalising the readings to the sum of their components.
void normalizeColors(colors *RGBC, normColors *normRGB) {
    unsigned int sum = (RGBC->red) + (RGBC->green) + (RGBC->blue); //access the colour Struct.
    
    //we divide the sum by 100 here because we want values out of 100 - should be precise enough.
    //could have multiplied numerator by 100 instead but this would create overflow so therefore we avoided this.
    normRGB->normRed = (RGBC->red) / ((sum)/100);
    normRGB->normGreen = (RGBC->green) / ((sum)/100);
    normRGB->normBlue = (RGBC->blue) / ((sum)/100);
    normRGB->clear = RGBC->clear;
}

//code to read ALL the colours, which calls each of the individual colour read functions above.
void readColors(colors *RGBC) {
    //first we read the CLEAR value with all 3 colours of the tri-colour LED being lit.
    //all the delays included below are to make sure that the LEDs have time to reach their max brightness before being read.
    LEDturnON();
    RGBC->clear = readClearColor();
    __delay_ms(100);
    LEDturnOFF();
    
    //read RED with the RED LED turned on
    redLED = 1;
    __delay_ms(100);
    RGBC->red = readRedColor();
    __delay_ms(100);
    redLED = 0;
    
    //read GREEN with the GREEN LED turned on
    greenLED = 1;
    __delay_ms(100);
    RGBC->green = readGreenColor();
    __delay_ms(100);
    greenLED = 0;
    
    //read BLUE with the BLUE LED turned on
    blueLED = 1;
    __delay_ms(100);
    RGBC->blue = readBlueColor();
    __delay_ms(100);
    blueLED = 0;
}

// Routine to detect colour of piece of paper 
char decideColor(normColors *normRGB, colors *RGBC, DC_motor *mL, DC_motor *mR) {
    if (normRGB->normBlue > 17) {
        creep(mL, mR, 16, 0);
        return 2; //BLUE
    }
    if (normRGB->normBlue > 12 && normRGB->normRed < 50) {
        creep(mL, mR, 16, 0);
        return 6; // LIGHT BLUE
    }
    if (normRGB->normRed > 70,normRGB->normGreen < 22) {
        creep(mL, mR, 16, 0);
        return 0; // RED
    }
    else { 
        // Everything below here will have the buggy move right against the wall for better / more consistent detection
        LEDturnON();
        __delay_ms(1000);
        creep(mL, mR, 8, 1);
        readColors(RGBC); 
        normalizeColors(RGBC, normRGB);
        creep(mL, mR, 16, 0);
    }
        
        if (normRGB->clear < 0x300) {
            return 8; // BLACK
        }
        if (normRGB->normGreen > 48) {
            return 1; // GREEN
        }
        if (normRGB->normRed > 60 && normRGB->normGreen < 30) {
            return 5; // ORANGE
        }
        if (normRGB->normBlue < 7) {
            return 3; //    YELLOW
        }
        else {
            if (normRGB->normGreen > 34) {
                return 7; // WHITE
            }
            else {
                return 4; // PINK
            }
        }
    }
    
    


//unsigned int decideColor(normColors *normRGB) {
//    
//    if (normRGB->normBlue > 18) {
//        return 2; //BLUE
//    }
//    if (normRGB->normGreen > 40) {
//        if (normRGB->normBlue > 12) {
//            return 6; //LIGHT BLUE
//        }
//        
//        return 1; //GREEN
//    } else {
//    if (normRGB->normRed > 55) {
//        //further decision splits
//        if (normRGB->normGreen > 30) {
//            if (normRGB->normBlue > 12) {
//                return 4; //PINK
//            }
//            
//            return 3; //YELLOW
//        }
//        return 0; //RED
//    }}
//    
//    //if nothing:
//    return 0;
//    
//}