#include <xc.h>
#include "color.h"
#include "i2c.h"
#include "interact.h"

void color_click_init(void)
{   
    //setup colour sensor via i2c interface
    I2C_2_Master_Init();      //Initialise i2c Master

    //set integration time
	color_writetoaddr(0x01, 0xF6);
    
    
    //DONT KNOW IF WORKS (interrupts & thresholds & persistence):
    color_writetoaddr(0x0C, 0b01);
    
    //Enable interrupts from the color clicker while turning colour sensing on and enabling RGBC
    color_writetoaddr(0x00, 0x13); 
    __delay_ms(3); //need to wait 3ms for everthing to start up
    
    }

unsigned int color_readdoublefromaddress(char address) {
    unsigned int tmp;
	I2C_2_Master_Start();         //Start condition
	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode (0x27 was the color clicker address, but we do <<1 to add a 0 at the end which configures "write", this yields 0x52)
	I2C_2_Master_Write(0x80 | address);    //command (auto-increment protocol transaction) - read value at particular address
	I2C_2_Master_RepStart();			// start a repeated transmission
    I2C_2_Master_Write(0x52 | 0x01);     //7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(1);			//read the Red LSB
	tmp=tmp | (I2C_2_Master_Read(0)<<8); //read the Red MSB (don't acknowledge as this is the last read)			//read the Red LSB
	I2C_2_Master_Stop();          //Stop condition
	return tmp;
}


unsigned int color_readfromaddress(char address) {
    unsigned int tmp;
	I2C_2_Master_Start();         //Start condition
	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode (0x27 was the color clicker address, but we do <<1 to add a 0 at the end which configures "write", this yields 0x52)
	I2C_2_Master_Write(0x80 | address);    //command (auto-increment protocol transaction) - read value at particular address
	I2C_2_Master_RepStart();			// start a repeated transmission
    I2C_2_Master_Write(0x52 | 0x01);     //7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(0);			//read the Red LSB
	SSP2CON2bits.PEN = 1;           //Initiate stop condition
	return tmp;
}


void color_writetoaddr(char address, char value){
    I2C_2_Master_Start();         //Start condition
    I2C_2_Master_Write(0x52 | 0x00);     //7 bit device address + Write mode
    I2C_2_Master_Write(0x80 | address);    //command + register address
    I2C_2_Master_Write(value);    
    I2C_2_Master_Stop();          //Stop condition
}

unsigned int readRedColor(void)
{
	unsigned int tmp;
	I2C_2_Master_Start();         //Start condition
	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode (0x27 was the color clicker address, but we do <<1 to add a 0 at the end which configures "write", this yields 0x52)
	I2C_2_Master_Write(0xA0 | 0x16);    //command (auto-increment protocol transaction) + start at RED low register
	I2C_2_Master_RepStart();			// start a repeated transmission
	I2C_2_Master_Write(0x52 | 0x01);     //7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(1);			//read the Red LSB
	tmp=tmp | (I2C_2_Master_Read(0)<<8); //read the Red MSB (don't acknowledge as this is the last read)
	I2C_2_Master_Stop();          //Stop condition
	return tmp;
}

unsigned int readGreenColor(void)
{
	unsigned int tmp;
	I2C_2_Master_Start();         //Start condition
	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode (0x27 was the color clicker address, but we do <<1 to add a 0 at the end which configures "write", this yields 0x52)
	I2C_2_Master_Write(0xA0 | 0x18);    //command (auto-increment protocol transaction) + start at GREEN low register
	I2C_2_Master_RepStart();			// start a repeated transmission
	I2C_2_Master_Write(0x52 | 0x01);     //7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(1);			//read the Red LSB
	tmp=tmp | (I2C_2_Master_Read(0)<<8); //read the Red MSB (don't acknowledge as this is the last read)
	I2C_2_Master_Stop();          //Stop condition
	return tmp;
}

unsigned int readBlueColor(void)
{
	unsigned int tmp;
	I2C_2_Master_Start();         //Start condition
	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode (0x27 was the color clicker address, but we do <<1 to add a 0 at the end which configures "write", this yields 0x52)
	I2C_2_Master_Write(0xA0 | 0x1A);    //command (auto-increment protocol transaction) + start at BLUE low register
	I2C_2_Master_RepStart();			// start a repeated transmission
	I2C_2_Master_Write(0x52 | 0x01);     //7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(1);			//read the Red LSB
	tmp=tmp | (I2C_2_Master_Read(0)<<8); //read the Red MSB (don't acknowledge as this is the last read)
	I2C_2_Master_Stop();          //Stop condition
	return tmp;
}

unsigned int readClearColor(void)
{
	unsigned int tmp;
	I2C_2_Master_Start();         //Start condition
	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode (0x27 was the color clicker address, but we do <<1 to add a 0 at the end which configures "write", this yields 0x52)
	I2C_2_Master_Write(0xA0 | 0x14);    //command (auto-increment protocol transaction) + start at CLEAR low register
	I2C_2_Master_RepStart();			// start a repeated transmission
	I2C_2_Master_Write(0x52 | 0x01);     //7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(1);			//read the Red LSB
	tmp=tmp | (I2C_2_Master_Read(0)<<8); //read the Red MSB (don't acknowledge as this is the last read)
	I2C_2_Master_Stop();          //Stop condition
	return tmp;
}

void normalizeColors(colors *RGBC, normColors *normRGB) {
    unsigned int sum = (RGBC->red) + (RGBC->green) + (RGBC->blue);
    
    normRGB->normRed = (RGBC->red) / ((sum)/100);
    normRGB->normGreen = (RGBC->green) / ((sum)/100);
    normRGB->normBlue = (RGBC->blue) / ((sum)/100);
    
//    normRGB->normRed = (RGBC->red) / ((RGBC->clear)/100);
//    normRGB->normGreen = (RGBC->green) / ((RGBC->clear)/100);
//    normRGB->normBlue = (RGBC->blue) / ((RGBC->clear)/100);
}

void readColors(colors *RGBC) {
    LEDturnON();
    RGBC->clear = readClearColor();
    __delay_ms(100);
    LEDturnOFF();
    redLED = 1;
    __delay_ms(100);
    RGBC->red = readRedColor();
    __delay_ms(100);
    redLED = 0;
    greenLED = 1;
    __delay_ms(100);
    RGBC->green = readGreenColor();
    __delay_ms(100);
    greenLED = 0;
    blueLED = 1;
    __delay_ms(100);
    RGBC->blue = readBlueColor();
    __delay_ms(100);
    blueLED = 0;
    
}


unsigned int decideColor(normColors *normRGB) {
    
    if (normRGB->normBlue > 18) {
        return 2; //BLUE
    }
    if (normRGB->normGreen > 40) {
        if (normRGB->normBlue > 12) {
            return 6; //LIGHT BLUE
        }
        
        return 1; //GREEN
    } else {
    if (normRGB->normRed > 55) {
        //further decision splits
        if (normRGB->normGreen > 30) {
            if (normRGB->normBlue > 12) {
                return 4; //PINK
            }
            
            return 3; //YELLOW
        }
        return 0; //RED
    }}
    
    //if nothing:
    return 0;
    
}