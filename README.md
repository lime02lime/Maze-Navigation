[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/c8ng1gdc)
# Course project - Mine navigation search and rescue

## Project Readme - Alex & Emil
The objective of this project is to code and configure the hardware for a buggy to navigate a maze. The movement of the buggy is informed by coloured cards that are fixed to the walls of the maze, where different colours have associated instructions for movement. The buggy is equipped with a tri-colour LED (red, green, blue light) whose LEDs can be controlled individually or collectively, as well as a light sensor which reads brightness values for individual red, green, blue, and clear sensors (RGBC).

The overarching functionality of the buggy, coded in the main.c file is structured as follows:
1. Initialize modules (LEDs, motors, and light sensor) and interrupts.
2. Press button RF2 to begin maze navigation.
3. The buggy moves forward until interrupted by sensing a colour card close in front of it.
4. Once interrupted, it reads the color, normalizes the readings, decides what the color is, and executes the corresponding action code.
5. Finally it clears interrupts and resumes its forward motion (back to step #3).
6. Once the final White card or a Black dead end are detected, the buggy returns home to the starting point by reversing the instructions that took it to that White / Black card.

A video of the buggy navigating a sample maze is linked below. The calibration routine is separate in order to reduce the file size, and is linked in the Calibration section further down.

[Press this link to watch the complete maze run on YouTube](https://youtu.be/NJkiyJc0TDg)

### Wall Detection and Interrupts
**Surface Detection:**
The buggy's forward motion is halted when a surface is detected in its path. This detection is based on an interrupt triggered by the INT pin from the TCS3471 colour sensor, based on how many times a Clear reading from the detector has exceeded a certain brightness threshold. This is connected through the buggy to the RB0 pin on the picKit to trigger an interrupt which is configured through the Peripheral Port Select (PPS) module of the microcontroller. As a surface gets closer, more of the light emitted from the buggy LEDs will be reflected from the surface and therefore the Clear readings increase in value. The interrupts.c file contains the initialisation of the interrupts as well as the Interrupt Service Routine. When this interrupt is triggered, the buggy is quickly halted to avoid collision with the surface. The threshold for the sensor interrupt is also configured here. This value is highly dependent on the ambient lighting conditions, and is therefore configured with an initial calibration routine.

**Timer:**
The interrupts.c file also configures the timer, which is crucial to keep track of the buggy's movements in order to return home safely. Each overflow of the timer increments a time counter (variable name _increment_). For each movement of the buggy, the associated time is fetched from _increment_ and stored. The timer is configured with a 1:4 prescaler so that it overflows with a frequency of 64 Hz - this enables sufficiently precise timekeeping to provide accuracy when timing motor movements while keeping the _increment_ variable low enough to not overflow before completing the maze. 

### Buggy Movement and Navigation
Once the sensor interrupt is triggered, the buggy quickly halts and raises a flag (char _wall_detected_) that a wall has been detected. The main while loop checks for this flag and if it is raised performs the following actions:
1. Records the colour readings from the sensor and stores these in the RGBC Structure.
2. Normalises the readings to the brightness (in our case this is taken as the sum of the RGB components) and stores these values in the normRGB Structure.
3. Decides what colour the card is through a decision tree process (elaborated in the Colour Clicker section).
4. Most colours are difficult to distinguish from a distance, which gets the buggy to creep slowly right up against the colour card, performing steps 1-3 again in order to decide on a colour, before moving on to step 5.
5. Stores the current _increment_ value and the instruction associated with the colour identified, so that when returning home, the buggy knows how far to move in this segment and what action has been performed.
6. The buggy creeps a short distance backwards so that the wall doesn't interfere with its turning and then the colour-associated action is executed (as coded in the instructions.c file).
7. Finally, the sensor interrupt is cleared and the buggy resumes its forward motion in the main.c while loop until the interrupt is raised again.

The dc_motor.c file contains the code to control the motors on the buggy. Functions are defined to enable movement forward, backwards, and to turn at specified angles. The accuracy of the buggy's turns is dependent on the friction between its wheels and the surface, as well as the battery level of the buggy. Consequently, calibration is required to achieve the desired turn angles.

### Calibration
Before navigating the maze, two calibration routines are performed. These are included in the calibration.c file. A video is shown at the end of this section.

**Interrupt Calibration:** The buggy stops when a surface is detected in its path, which is determined by the Colour Clicker's Clear reading. Importantly, the Clear reading is highly dependent on the ambient light conditions, which means that the threshold for interrupt trigger must be calibrated before operation. The interrupt threshold calibration follows this structure:
1. The buggy reads and stores the ambient Clear value, then turns 90°, reads and stores it again, and so on until it has read this value facing in 4 directions at 90° intervals.
2. The user then places a black card directly in front of the buggy (to simulate the darkest reading at which it would need to interrupt), reading and storing the Clear value.
3. The interrupt high threshold is then set as the midpoint between the highest ambient brightness reading and the reading against the black card.

This procedure is used to ensure that the buggy does not trigger the interrupt in response to ambient light, while ensuring that it can read the black card as well as all of the brighter colour cards (since the reflection against the black card is the lowest of all cards).

**Motor Calibration:** The motors are run by DC and do not include any closed loop control, which means that we must calibrate them before use. Specifically, this applies to the buggy turning, where achieving repeatable turn angles is important. Therefore, the 90° left/right turns are calibrated upon startup, following this structure:
1. The buggy turns left 90° according to the default settings.
2. The user observes whether the turn was sufficiently accurate and presses one of two buttons. LHS button if the buggy should turn more left, and RHS button if the buggy should turn more right. The buggy waits until button press.
3. The buggy repeats steps 1 and 2 until the user is satisfied with the result, at which point they press the two buttons simultaneously - which exits the left turn calibration routine.
4. Steps 1-3 are now performed to calibrate the 90° right turn. The buggy turns to the right according to the default 90° settings, and then waits for button press. As before, the LHS/RHS buttons correspond to the direction in which to increment the turning magnitude.

It is worth noting that we have separate calibration routines for Left and Right because the motors do not always perform equally. The turns of the motor involve ramping up to a maximum turning speed for all motors, holding for a period of time, then ramping downwards in power to stop the turn. The number being changed by the calibration is the maximum motor power during the hold, not the time the turn is being held. As for turning at other angles, the 135 degree turns to the left and right are calculated as increasing the maximum motor power while turning for each respective side at 90 degrees by a constant factor (identical for both sides), but keeping the holding time at maximum power for turning constant. As for the 180 degree turn, the most consistent results across all surfaces were found by repeating a 90 degree turn twice, rather than doing a smooth single turn.

This procedure is used to ensure that the buggy does not trigger the interrupt in response to ambient light, while ensuring that it can read the black card as well as all of the brighter colour cards. A video of the calibration is linked to below.

[Press this link to watch the calibration routine on YouTube](https://youtu.be/75zoIOjzlGE)


### Colour Clicker
The color.c file contains most of the colour sensing functionality. This includes two main parts:
1. Colour sensing - readColours() can be called to collect RGBC data from the colour sensor. It reads the colours one at a time, turning on only the LED colour corresponding to the colour being read. This improves the sensor's ability to distinguish between colours. For the Clear reading, all three LEDs are turned on.
2. Colour deciding - decideColour() implements a decision tree approach to establish what colour is faced. The decision boundaries for the decision tree were informed by looking at data collected and stored in the excel spreadsheet named "Colour detection data". The function accesses the normalised colour values, transitions through the decision tree and returns an integer as its colour decision - each of the 9 colours having a number assigned to it (in the order of the table mapping instructions to colours found below).

Communication with the colour clicker is performed via an I2C protocol for performing activities such as reading colour sensor values and configuring settings. The functions to enable this communication are contained within the i2c.c file, provided alongside the project brief. 


### Returning Home
During the buggy's travel through the maze, each navigation action is stored in an array. Because there is no direct way to measure distances, time is instead used as a proxy. The time interval spent on each straight is recorded every time a navigation action is performed, and stored in the array alongside the navigation action taken. Upon reaching the end of the maze (indicated by a white card, or a black wall - dead end), the navigation array is traversed backwards to return to the buggy to the starting location. Naturally, it is coded so that the opposite direction of each turn is performed on the way back.

### Future Work, Areas of Improvement and Attempted Solutions
Below we have listed the areas that require most continued work.

**Turn Calibration:** Currently, the turn calibration adjusts the _maxPower_ variable within the 90° turn functions. This provides an easy way to increase/decrease the magnitude of the turns. The main downside with this process is that it may not provide precise enough adjustments to achieve an accurate and repeatable turn angle. This is also exacerbated by the way that the turns are performed, where the power is stepped up from 0 to the max power, and thereafter down to 0 again. This means that an increase of 1 in power has a twofold effect as it must be stepped up to and stepped down from. This value is initialised to approximately 30, so a change of 2 units up or down amounts to a considerable change. A method of improving turn accuracy may be to instead adjust the time during which the maximum power is held. This value is currently set to approximately 300 ms, which allows for much smaller percentage changes.

**Route Optimisation:** Currently, when the buggy returns home from the end of the maze, it retraces its steps exactly. In some cases, such as for the yellow and pink cards, this means that the buggy will have to move into a dead end just to traverse the same segment going back. Instead, logic could be implemented such that when the buggy sees the instruction is pink or yellow, it looks ahead to where the instruction before the yellow / pink card was executed, and heads straight to that location, excluding redundant moves and instead returning home more directly.

As a more advanced case of route optimisation, an algorithm could be implemented to detect any time the buggy crosses over the same point throughout the route. Then upon the return, skip portions of the map that would leave it in the same spot anyway. One way of doing this would be to allocate a large array that would be populated with coordinates of the buggy as it progressed through the route (e.g. marking coordinates after a certain time interval). With a large enough array and frequent enough coordinates measured, once the route has been completed. One could create compare coordinates (either for direct equality or closeness based on e.g. a simple L2 metric - i.e. pythagoras) and find where intersections have occurred, taking care to align the buggy in the proper direction to continue the route home (for example by a simple look aheads in the array to check which direction the buggy was going before skipping a path and where it should go after skipping the path).

**Machine Learning** An attempt was made to apply machine learning algorithms to the colour detection by using python in combination with some collected data. These models were deliberately selected to be simple, so that they would be easy to translate into C code. A decision tree and small neural network were attempted. The decision tree yielded better results than the neural network, but it seemed the model lacked sufficient data to make an optimal choice of decision boundaries, despite attempts to optimise the model through limiting the depth of the tree and increasing the number of samples at a leaf needed to split it. Data was collected in different lighting conditions to try to improve reliability, but in the end determining decision boundaries 'by eye' and moving the buggy sensor right up against the colour to detect was the best solution. If curious the code for the attempt can be found in ml.py and the data in mldata.csv and mllab.csv.

**Smooth 180 degree turn**
An attempt was made to make a smooth 180 degree turn, stored in the branch turn180. It was found that while it was possible to get a consistent smooth 180 degree turn on one surface by using a constant factor, to do the same on a different surface - despite recalibrating for the 90 degree turn on that surface - would lead to worse inaccuracy than making two 90 degree turns. However, the attempt was rushed due to an approaching deadline so it is is possible something was missed. If you wish to resume the attempt head to line 170 of dc_motor.c in that branch.

### Final Notes
**Testing flags** To faciliate testing, a number of flags were defined at the top of the main.c file to shorten the time intervals between tests and make them more convenient to perform. Flags include preventing the buggy from moving forward between instructions, skipping calibration and instead using some default values, and using a blinking LED to indicate the colour code of the instruction about to be performed. Please be aware of these flags and use them for your convenience.

END OF PROJECT README


## Challenge brief (original assignment instructions).

Your task is to develop an autonomous robot that can navigate a "mine" using a series of instructions coded in coloured cards and return to its starting position.  Your robot must be able to perform the following: 

1. Navigate towards a coloured card and stop before impacting the card
1. Read the card colour
1. Interpret the card colour using a predefined code and perform the navigation instruction
1. When the final card is reached, navigate back to the starting position
1. Handle exceptions and return back to the starting position if final card cannot be found

## "Mine" environment specification

A "mine" is contstructed from black plywood walls 100mm high with some walls having coloured cards located on the sides of the maze to assist with navigation. The following colour code is to be used for navigation:

Colour | Instruction
---------|---------
Red | Turn Right 90
Green | Turn Left 90
Blue | Turn 180
Yellow | Reverse 1 square and turn right 90
Pink | Reverse 1 square and turn left 90
Orange | Turn Right 135
Light blue | Turn Left 135 
White | Finish (return home)
Black | Maze wall colour

Mine courses will vary in difficulty, with the simplest requiring 4 basic moves to navigate. More advanced courses may require 10 or moves to navigate. The mines may have features such as dead ends but colour cards will always direct you to the end of the maze. Once the end of the maze has been reached, you must return to the starting position. An example course to navigate is shown below. You do not know in advance which colours will be in the course or how many.

![Navi Diagram](gifs/maze.gif)

## Resources and project submission

To develop your solution you have your Clicker 2 board, buggy and colour click add on board. You must not use any hardware that is not provided as part of this course and you must use the XC8 compiler to compile your C code. 

Please use this GitHub repo to manage your software development and submit your project code. 

Final testing will take place in the CAGB foyer and testing areas will be provided around the mechatronics lab. You will not know the exact layout of the maze(s) in advance. You will also be asked to give a short presentation on the testing day.

## Supplementary technical information

### Additional buggy features

In addition to the motor drives we explored in lab 6, the buggy contains some additional features that may be of use during the project. The first feature is additional LEDs, controlled through the pins labelled **H.LAMPS**, **M.BEAM**, **BRAKE**, **TURN-L** and **TURN-R**. H.LAMPS turns on the front white LEDs and rear red LEDs, at a reduced brightness. M.BEAM and BRAKE enable you to turn these LEDs on at full brightness. The turn signals have not hardware based brightness control. These LEDs give you a method to provide feedback for debugging, in addition of the LEDs on the clicker board.

![Buggy pinout](gifs/buggy_pins.png)

A further feature of the buggy is **BAT-VSENSE** pin which allows you to monitor the batter voltage via an analogue input pin. The battery is connected across a voltage divider, as shown in the diagram below:

![Bat sense](gifs/bat_vsense.png)

The voltage at BAT-VSENSE will always be one third of that at the battery. Measuring this value can be useful for determining if your battery needs charging. You could also use it to calibrate your robot to perform well at different charge levels. 

### Colour click

The Colour click board contains 2 devices to help with navigation, a tri-colour LED for illumination and a 4 channel RGBC photodiode sensor. This combination of two devices (an illumination source and a sensor) allow you a make measurements of the reflected colour of objects near the sensor. The circuit diagram for the Colour click is shown below:

![Color Cick](gifs/color_click.png)

The tri-colour LED is the simpler of the two devices to control. Three separate pins control the red, green and blue LEDs individually, despite them being in a single package. Control via these pins is simple digital on/off control and if any brightness control was required, the user would need program the microcontroller to generate a PWM signal to achieve this.  

The second device on the Colour click is the TCS3471 colour light-to-digital converter. The sensor contains a grid of 4x4 photodiodes, 4 are sensitive to red light, 4 green light, 4 blue light and 4 "clear" light (that is, a range of wavelengths, see datasheet for exact spectral response). When light falls on the photodiode the photons are absorbed and current is generated. This signal is then integrated over time using integrators and sampled by 16 bit on board ADCs. Communication with the device is achieved using an I2C interface. This enables configuration of the device to customise sampling of the data (i.e. integration time, gain, etc.) and to read the 16 bit digital values for each of the RGBC channels. The relative magnitude of these values gives you information about the colour of light that is falling on the sensor. The device can also be configured to send an interrupt signal to the PIC when signal reaches a preset value.

### I2C

The I2C interface widely used in industry for communication between microcontrollers and peripheral integrated circuits (other chips) over short distances. I2C is serial communication bus that enables communication between many devices over a simple 2 wire interface. One wire is the data line (SDA) and is used for both transmission and receiving. The second wire (SCL) is used for a clock signal to ensure all devices are synchronous. To ensure communication of data occurs without problem a message protocol must be followed and understood by all devices on the bus. Devices are termed master and slave devices, with master devices initiation communication to a slave device via unique address for that device. The general sequence of communication between a master/slave over the I2C interface is a follows:

1. Send a Start bit
1. Send the slave address, usually 7 bits
1. Send a Read (1) or Write (0) bit to define what type of transaction it is
1. Wait for an Acknowledge bit
1. Send a data or command byte (8 bits)
1. Wait for Acknowledge bit
1. Send the Stop bit

This is shown pictorial in the figure below:

![I2C](gifs/i2c.png)

Although it is possible to program an entirely software based I2C interface, luckily for us our PIC chip has a module dedicated to generating and receiving I2C signals: the Master Synchronous Serial Port Module, or MSSP (see chapter 28 of the PIC datasheet). This module provides methods for sending start/stop/acknowledge bits and allows us to focus on sending/receiving data.

The included i2c.c/h files contain functions to help you get started with I2C communication. The first function below sets up the MSSP module as an I2C master device and configures the necessary pins.

	void I2C_2_Master_Init(void)
	{
		//i2c config  
		SSP2CON1bits.SSPM= 0b1000;    // i2c master mode
		SSP2CON1bits.SSPEN = 1;       //enable i2c
		SSP2ADD = (_XTAL_FREQ/(4*_I2C_CLOCK))-1; //Baud rate divider bits (in master mode)
  
		//pin configuration for i2c  
		TRISDbits.TRISD5 = 1;                   //Disable output driver
		TRISDbits.TRISD6 = 1;                   //Disable output driver
		ANSELDbits.ANSELD5=0;					// disable analogue on pins
		ANSELDbits.ANSELD6=0;					// disable analogue on pins
		SSP2DATPPS=0x1D;      //pin RD5
		SSP2CLKPPS=0x1E;      //pin RD6
		RD5PPS=0x1C;      // data output
		RD6PPS=0x1B;      //clock output
	}
	
Bits with the SSP2CON2 register are set to send the individual start/stop/acknowledge bits used in the protocol. These must only be set when the bus is idle (nothing being sent/received). The I2C_2_Master_Start(), I2C_2_Master_Stop() and I2C_2_Master_RepStart() functions allow you add the necessary bits as defined in the protocol above. Data is sent on the bus using the SSP2BUF register:

	void I2C_2_Master_Write(unsigned char data_byte)
	{
		I2C_2_Master_Idle();
		SSP2BUF = data_byte;         //Write data to SSPBUF
	}

Data is also read using the same SSP2BUF registers. However, to receive data we first need to switch the module into receiver mode. We also need to start the acknowledge sequence to let the slave device know what we have received the data OK. The following function achieves this:

	unsigned char I2C_2_Master_Read(unsigned char ack)
	{
		unsigned char tmp;
		I2C_2_Master_Idle();
		SSP2CON2bits.RCEN = 1;        // put the module into receive mode
		I2C_2_Master_Idle();
		tmp = SSP2BUF;                //Read data from SS2PBUF
		I2C_2_Master_Idle();
		SSP2CON2bits.ACKDT = !ack;     // 0 turns on acknowledge data bit
		SSP2CON2bits.ACKEN = 1;        //start acknowledge sequence
		return tmp;
	}

The functions described so form the basics required for I2C communication with the PIC. To communicate with the TCS3471 onboard the Colour click we first need to know its address. This is listed in the data sheet as 0x29. To send our first byte over the I2C we need to send this address combined with the transaction type (read or write) as defined in the protocol diagram above. This lets the TCS3471 know the message is intended for it and not some other device on the interface. Next we send a byte which is a combination of command type and the register address in the TCS3471 that we want to write to. Finally we the value that we want to write to that register. This sequence is shown in the function below:

	void color_writetoaddr(char address, char value){
		I2C_2_Master_Start();         		//Start condition
		I2C_2_Master_Write(0x52 | 0x00);     //7 bit device address + Write (0) mode (note 0x52=0x29<<1)
		I2C_2_Master_Write(0x80 | address);    //command + register address
		I2C_2_Master_Write(value);    			//value to store in the register
		I2C_2_Master_Stop();          //Stop condition
	}

We then call the function to, for example, turn the device on:

	color_writetoaddr(0x00, 0x01); // write 1 to the PON bit in the device enable register
	
There are additional commands that must be set to initialise the device and many registers that be configured to obtain optimal performance for the sensor in your conditions. It is up to you to carefully read the TCS3471 datasheet and experiment with this.

To read values from the TCS3471 you need to a similar sequence to above but you first need to tell the device which register you want to deal with, before telling the device you want read from it. The example below uses the combined read format to read multiple bytes in sequence. The auto-increment is set so that instead of reading the same register over and over again, it automatically advances to the next one. The example starts at the Red channel low byte address and then automatically advances and reads the associated high byte.

	unsigned int color_read_Red(void)
	{
		unsigned int tmp;
		I2C_2_Master_Start();         //Start condition
		I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode
		I2C_2_Master_Write(0xA0 | 0x16);    //command (auto-increment protocol transaction) + start at RED low register
		I2C_2_Master_RepStart();
		I2C_2_Master_Write(0x52 | 0x01);     //7 bit address + Read (1) mode
		tmp=I2C_2_Master_Read(1);			// read the Red LSB
		tmp=tmp | (I2C_2_Master_Read(0)<<8); //read the Red MSB (don't acknowledge as this is the last read)
		I2C_2_Master_Stop();          //Stop condition
		return tmp;
	}

Instead of separate functions for each channel you may want to create a structure to store all the values together, and pass a pointer to the function so that all values in the structure can be updated in one operation. An example structure might look like this:

	//definition of RGB structure
	struct RGB_val { 
		unsigned int R;
		unsigned int G;
		unsigned int B;
	};

This concludes the basics of I2C and communication with the colour sensor. Best of luck! 
