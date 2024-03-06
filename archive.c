//DONT KNOW IF WORKS:
//this SHOULD return a byte containing a 1 in the 5th bit if the interrupt is raised (0b00010000.
//unsigned int readInterrupt(void)
//{
//	unsigned int tmp;
//	I2C_2_Master_Start();         //Start condition
//	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + write mode (0x27 was the color clicker address, but we do <<1 to add a 0 at the end which configures "write", this yields 0x52)
//	I2C_2_Master_Write(0x80 | 0x13);    //command (repeated byte protocol transaction) addressed to the status register (0x13)
//	I2C_2_Master_RepStart();			// start a repeated transmission
//	I2C_2_Master_Write(0x52 | 0x01);     //7 bit address + Read (1) mode
//	tmp=I2C_2_Master_Read(1);			//read the status register byte
//
//	I2C_2_Master_Stop();          //Stop condition
//	return tmp;
//}