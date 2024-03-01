#include <xc.h>
#include "color.h"
#include "i2c.h"
#include "interact.h"

void color_click_init(void)
{   
    //setup colour sensor via i2c interface
    I2C_2_Master_Init();      //Initialise i2c Master

    
     //set device PON=1 
	color_writetoaddr(0x00, 0x01);
    __delay_ms(3); //need to wait 3ms for everthing to start up
    
    
    //turn on device ADC
    //0x03 = 0b11. The final value is what we write (1 to enable, otherwise 0), and the rest is the address.
    //in this case we want to write to AEN (RGBC enable), which is at address 0b10.
	color_writetoaddr(0x00, 0x03);

    //set integration time
	color_writetoaddr(0x01, 0xF6);
    
    
    //DONT KNOW IF WORKS (interrupts & thresholds & persistence):
    
    //Enable interrupts from the color clicker.
    //we want to write 0b10001 (address 0b10000, value 0b00001). This equals 0x11.
    color_writetoaddr(0x00, 0x13); // Used to be 0x11 instead of 0x13 - note this makes ADC and PON setting above redundant
    
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
    normRGB->normRed = (RGBC->red) / ((RGBC->clear)/100);
    normRGB->normGreen = (RGBC->green) / ((RGBC->clear)/100);
    normRGB->normBlue = (RGBC->blue) / ((RGBC->clear)/100);
}

