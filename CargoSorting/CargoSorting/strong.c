
/*
*
* Team Id: eYRC#2492-CS
* Author List: Yash Joshi
* Filename: strong.c
* Theme: Cargo Sorting Robot
* Functions: void timer5_init(),
*         void motion_pin_config (void),
*         void velocity (unsigned char , unsigned char),
*         void motion_set (unsigned char Direction),
*         void forward (void),
*         void back (void),
*         void left (void),
*         void right (void),
*         void soft_left (void),
*         void soft_right (void),
*         void soft_left_2 (void),
*         void soft_right_2 (void),
*         void stop (void),
*         void lcd_port_config (void),
*         void servo_1_free (void),
*         void servo_2_free (void),
*         void servo_3_free (void),
*         void adc_pin_config (void,
*         void servo1_pin_config (void),
*         void servo2_pin_config (void),
*         void servo3_pin_config (void),
*         void timer1_init(void),
*         void port_init(void),
*         void buzzer_on (void),
*         void buzzer_off (void),
*         unsigned char ADC_Conversion(unsigned char Ch),
*         void print_sensor(char row, char coloumn,unsigned char channel),
*         ISR(INT0_vect),
*         void adc_init(),
*         void servo_1(unsigned char degrees),
*         void servo_2(unsigned char degrees),
*         void servo_3(unsigned char degrees),
*         void color_sensor_pin_config(void),
*         void color_sensor_pin_interrupt_init(void),
*         void filter_red(void),
*         void filter_green(void),
*         void filter_blue(void),
*         void filter_clear(void),
*         void color_sensor_scaling(),
*         void red_read(void),
*         void green_read(void),
*         void blue_read(void),
*         void left_encoder_pin_config (void),
*         void right_encoder_pin_config (void),
*         void port_init_poz(),
*         void left_position_encoder_interrupt_init (void),
*         void right_position_encoder_interrupt_init (void),
*         ISR(INT5_vect),
*         ISR(INT4_vect),
*         void angle_rotate(unsigned int Degrees),
*         void linear_distance_mm(unsigned int DistanceInMM),
*         void forward_mm(unsigned int DistanceInMM),
*         void back_mm(unsigned int DistanceInMM),
*         void left_degrees(unsigned int Degrees),
*         void right_degrees(unsigned int Degrees),
*         void soft_left_degrees(unsigned int Degrees),
*         void soft_right_degrees(unsigned int Degrees),
*         void soft_left_2_degrees(unsigned int Degrees),
*         void soft_right_2_degrees(unsigned int Degrees),
*         void init_devices(void)
* Global Variables: 
*        volatile unsigned long int ShaftCountLeft,
*        volatile unsigned long int ShaftCountRight,
*        volatile unsigned int Degrees,
*        unsigned char ADC_Value,
*        volatile unsigned long int pulse,
*        volatile unsigned long int red,
*        volatile unsigned long int blue,
*        volatile unsigned long int green
*/

/********************************************************************************

   Copyright (c) 2010, NEX Robotics Pvt. Ltd.                       -*- c -*-
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.

   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.

   * Neither the name of the copyright holders nor the names of
     contributors may be used to endorse or promote products derived
     from this software without specific prior written permission.

   * Source code can be used for academic purpose. 
	 For commercial use permission form the author needs to be taken.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE. 

  Software released under Creative Commence cc by-nc-sa licence.
  For legal information refer to: 
  http://creativecommons.org/licenses/by-nc-sa/3.0/legalcode

********************************************************************************/

/*
*
* Function Name: 	<Function Name>
* Input: 		<Inputs (or Parameters) list with description if any>
* Output: 		<Return value with description if any>
* Logic: 		<Description of the function performed and the logic used in the function>
* Example Call:		<Example of how to call this function>
*
*/
#define F_CPU 14745600
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h> //included to support power function
//#include "lcd.h"

volatile unsigned long int ShaftCountLeft = 0; //to keep track of left position encoder
volatile unsigned long int ShaftCountRight = 0; //to keep track of right position encoder
volatile unsigned int Degrees; //to accept angle in degrees for turning
unsigned char ADC_Value;
volatile unsigned long int pulse = 0; //to keep the track of the number of pulses generated by the color sensor
volatile unsigned long int red;       // variable to store the pulse count when read_red function is called
volatile unsigned long int blue;      // variable to store the pulse count when read_blue function is called
volatile unsigned long int green;     // variable to store the pulse count when read_green function is called


/*
*
* Function Name: 	timer5_init
* Input: 			void
* Output: 			Sets register values for starting gast 8-bit PWM
* Logic: 			Timer 5 can be used for PWM generation for controlling speed of
					motors.
					The duty cycle of square wave generated by the Timer5 can be
					varied to produce different average DC values for motors
					Using FAST PWM mode to vary speed of motors
* Example Call:		timer5_init()
*
*/
void timer5_init()
{
	TCCR5B = 0x00;	//Stop
	TCNT5H = 0xFF;	//Counter higher 8-bit value to which OCR5xH value is compared with
	TCNT5L = 0x01;	//Counter lower 8-bit value to which OCR5xH value is compared with
	OCR5AH = 0x00;	//Output compare register high value for Left Motor
	OCR5AL = 0xFF;	//Output compare register low value for Left Motor
	OCR5BH = 0x00;	//Output compare register high value for Right Motor
	OCR5BL = 0xFF;	//Output compare register low value for Right Motor
	OCR5CH = 0x00;	//Output compare register high value for Motor C1
	OCR5CL = 0xFF;	//Output compare register low value for Motor C1
 	TCCR5A = 0xA9;	//{COM5A1=1, COM5A0=0; COM5B1=1, COM5B0=0; COM5C1=1 COM5C0=0}
	  				//  For Overriding normal port functionality to OCRnA outputs.
 				  	//{WGM51=0, WGM50=1} Along With WGM52 in TCCR5B for Selecting FAST PWM 8-bit Mode
	
	TCCR5B = 0x0B;	//WGM12=1; CS12=0, CS11=1, CS10=1 (Prescaler=64)
}

/*
*
* Function Name: 	motion_pin_config
* Input: 			void
* Output: 			Enable the motion in the robot by initialising the pins
* Logic: 			Initialize the motor by setting PL3 and PL4 pins as output for PWM generation
					and PL3 and PL4 pins are for velocity control using PWM.
* Example Call:		motion_pin_config()
*
*/
void motion_pin_config (void)
{
	DDRA = DDRA | 0x0F;		// Output Port
	PORTA = PORTA & 0xF0;	// Initially motor stop
	DDRL = DDRL | 0x18;		//Setting PL3 and PL4 pins as output for PWM generation
	PORTL = PORTL | 0x18;	//PL3 and PL4 pins are for velocity control using PWM.
}

/*
*
* Function Name: 	velocity
* Input: 			unsigned char , unsigned char 
* Output: 			Gives PWM to left and right motor
* Logic: 			Sets the velocity for right and left motors int the range 0x00 to 0xFF
* Example Call:		velocity(0xFF,0xFF)
*
*/

void velocity (unsigned char left_motor, unsigned char right_motor)
{
	//Robot velocity control 
	OCR5AL = (unsigned char)left_motor; //OCR5AL (PortL3) is connected to Left Motor
	OCR5BL = (unsigned char)right_motor; //OCR5BL (PortL4) connected to Right Motor
}

/*
*
* Function Name: 	motion_set
* Input: 			unsigned char 
* Output: 			Set the direction of rotation of right and left motors
* Logic: 			Direction	PA(0)	PA(1)	PA(2) PA(3)
								LB		LF		RF		RB
					Forward		0		1		1		0
					Backward	1		0		0		1
					Left		1		0		1		0
					Right		0		1		0		1
					Soft Left	0		0		1		0
					Soft Right	0		1		0		0
					Stop		0		0		0		0
* Example Call:		motion_set(0x06)
*
*/
void motion_set (unsigned char Direction)
{
	//Set motor's direction
	//PortARestore: Temporary variable to store PORTA's original status
	unsigned char PortARestore = 0;

	Direction &= 0x0F; 			// removing upper nibble as it is not needed
	PortARestore = PORTA; 			// reading the PORTA's original status
	PortARestore &= 0xF0; 			// setting lower direction nibble to 0
	PortARestore |= Direction; 	// adding lower nibble for direction command and restoring the PORTA status
	PORTA = PortARestore; 			// setting the command to the port
}



/*
*
* Function Name: 	forward
* Input: 			void
* Output: 			Set the direction of rotation of right and left motors to forward
* Logic: 			Direction	PA(0)	PA(1)	PA(2) PA(3)
								LB		LF		RF		RB
					Forward		0		1		1		0
* Example Call:		forward()
*
*/
void forward (void) 
{
	//both wheels forward
	motion_set(0x06);
}
/*
*
* Function Name: 	back
* Input: 			void
* Output: 			Set the direction of rotation of right and left motors to backwards
* Logic: 			Direction	PA(0)	PA(1)	PA(2) PA(3)
								LB		LF		RF		RB
					Backward	1		0		0		1
* Example Call:		back()
*
*/
void back (void) 
{
	//both wheels backward
	motion_set(0x09);
}
/*
*
* Function Name: 	left
* Input: 			void
* Output: 			Set the direction of rotation of right motor to forward and left motor to backward
* Logic: 			Direction	PA(0)	PA(1)	PA(2) PA(3)
								LB		LF		RF		RB
					Left		1		0		1		0
* Example Call:		left()
*
*/
void left (void) 
{
	//Left wheel backward, Right wheel forward
	motion_set(0x05);
}
/*
*
* Function Name: 	right
* Input: 			void
* Output: 			Set the direction of rotation of right motor to backward and left motor to forward
* Logic: 			Direction	PA(0)	PA(1)	PA(2) PA(3)
								LB		LF		RF		RB
					Right		0		1		0		1
* Example Call:		right()
*
*/
void right (void) 
{
	//Left wheel forward, Right wheel backward
	motion_set(0x0A);
}
/*
*
* Function Name: 	soft_left
* Input: 			void
* Output: 			Set the direction of rotation of right motor to forward and left motor stop
* Logic: 			Direction	PA(0)	PA(1)	PA(2) PA(3)
								LB		LF		RF		RB
					Soft Left	0		0		1		0
* Example Call:		soft_left()
*
*/
void soft_left (void) 
{
	//Left wheel stationary, Right wheel forward
	motion_set(0x04);
}
/*
*
* Function Name: 	soft_right
* Input: 			void
* Output: 			Set the direction of rotation of left motor to forward and right motor stop
* Logic: 			Direction	PA(0)	PA(1)	PA(2) PA(3)
								LB		LF		RF		RB
					Soft Right	0		1		0		0
* Example Call:		soft_right()
*
*/
void soft_right (void) 
{
	//Left wheel forward, Right wheel is stationary
	motion_set(0x02);
}
/*
*
* Function Name: 	soft_left_2
* Input: 			void 
* Output: 			Set the direction of rotation of left motor backward and right motor stop
* Logic: 			Direction	PA(0)	PA(1)	PA(2) PA(3)
								LB		LF		RF		RB
					Soft Left_2	1		0		0		0
* Example Call:		soft_left_2()
*
*/
void soft_left_2 (void) 
{
	//Left wheel backward, right wheel stationary
	motion_set(0x01);
}
/*
*
* Function Name: 	soft_right_2
* Input: 			void
* Output: 			Set the direction of rotation of right motor to backward and left motor stop
* Logic: 			Direction	PA(0)	PA(1)	PA(2) PA(3)
								LB		LF		RF		RB
				Soft Right_2	0		0		0		1
* Example Call:		soft_right_2()
*
*/
void soft_right_2 (void) 
{
	//Left wheel stationary, Right wheel backward
	motion_set(0x08);
}
/*
*
* Function Name: 	stop
* Input: 			void
* Output: 			Right and left motor stop
* Logic: 			Direction	PA(0)	PA(1)	PA(2) PA(3)
								LB		LF		RF		RB
					Stop		0		0		0		0
* Example Call:		stop()
*
*/
void stop (void)
{
	motion_set(0x00);
}


//*************SERVO*************************

/*
*
* Function Name: 	servo_1_free
* Input: 			void
* Output: 			Unlocks the servo motors from the any angle 
					and make them free by giving 100% duty cycle at the PWM. This function can be used to 
					reduce the power consumption of the motor if it is holding load against the gravity.

* Logic: 			Turns the servo off by setting OCR1AL to 0xFF
* Example Call:		servo_1_free()
*
*/
void servo_1_free (void)
{
	//makes servo 1 free rotating
	OCR1AH = 0x03;
	OCR1AL = 0xFF; //Servo 1 off
}
/*
*
* Function Name: 	servo_2_free
* Input: 			void
* Output: 			Unlocks the servo motors from the any angle 
					and make them free by giving 100% duty cycle at the PWM. This function can be used to 
					reduce the power consumption of the motor if it is holding load against the gravity.

* Logic: 			Turns the servo off by setting OCR1BL to 0xFF
* Example Call:		servo_2_free()
*
*/
void servo_2_free (void) 
{
	//makes servo 2 free rotating
	OCR1BH = 0x03;
	OCR1BL = 0xFF; //Servo 2 off
}
/*
*
* Function Name: 	servo_3_free
* Input: 			void
* Output: 			Unlocks the servo motors from the any angle 
					and make them free by giving 100% duty cycle at the PWM. This function can be used to 
					reduce the power consumption of the motor if it is holding load against the gravity.

* Logic: 			Turns the servo off by setting OCR1CL to 0xFF
* Example Call:		servo_3_free()
*
*/
void servo_3_free (void) 
{
	//makes servo 3 free rotating
	OCR1CH = 0x03;
	OCR1CL = 0xFF; //Servo 3 off
}

/*
*
* Function Name: 	servo_1
* Input: 			void
* Output: 			Moves the servo motor to the specified angle

* Logic: 			Divide the angle by 1.86 which is the angular resulution of the servo and then add 35.
					The resultant value is the value to be given to ORC1Al
* Example Call:		servo_1()
*
*/
void servo_1(unsigned char degrees)
{
	//Function to rotate Servo 1 by a specified angle in the multiples of 1.86 degrees
	float PositionPanServo = 0;
	//PositionPanServo: The value to which the servo is to be positioned at
	PositionPanServo = ((float)degrees / 1.86) + 35.0;
	OCR1AH = 0x00;
	OCR1AL = (unsigned char) PositionPanServo;
}



void servo_2(unsigned char degrees)
{
	//Function to rotate Servo 2 by a specified angle in the multiples of 1.86 degrees
	float PositionTiltServo = 0;
	//PositionTiltServoServo: The value to which the servo is to be positioned at
	PositionTiltServo = ((float)degrees / 1.86) + 35.0;
	OCR1BH = 0x00;
	OCR1BL = (unsigned char) PositionTiltServo;
}


void servo_3(unsigned char degrees)
{
	//Function to rotate Servo 3 by a specified angle in the multiples of 1.86 degrees
	float PositionServo = 0;
	//PositionServo: The value to which the servo is to be positioned at
	PositionServo = ((float)degrees / 1.86) + 35.0;
	OCR1CH = 0x00;
	OCR1CL = (unsigned char) PositionServo;
}

//Configure PORTB 5 pin for servo motor 1 operation
void servo1_pin_config (void)
{
	DDRB  = DDRB | 0x20;  //making PORTB 5 pin output
	PORTB = PORTB | 0x20; //setting PORTB 5 pin to logic 1
}

//Configure PORTB 6 pin for servo motor 2 operation
void servo2_pin_config (void)
{
	DDRB  = DDRB | 0x40;  //making PORTB 6 pin output
	PORTB = PORTB | 0x40; //setting PORTB 6 pin to logic 1
}

//Configure PORTB 7 pin for servo motor 3 operation
void servo3_pin_config (void)
{
	DDRB  = DDRB | 0x80;  //making PORTB 7 pin output
	PORTB = PORTB | 0x80; //setting PORTB 7 pin to logic 1
}
//TIMER1 initialization in 10 bit fast PWM mode  
//prescale:256
// WGM: 7) PWM 10bit fast, TOP=0x03FF
// actual value: 52.25Hz 
void timer1_init(void)
{
 TCCR1B = 0x00; //stop
 TCNT1H = 0xFC; //Counter high value to which OCR1xH value is to be compared with
 TCNT1L = 0x01;	//Counter low value to which OCR1xH value is to be compared with
 OCR1AH = 0x03;	//Output compare Register high value for servo 1
 OCR1AL = 0xFF;	//Output Compare Register low Value For servo 1
 OCR1BH = 0x03;	//Output compare Register high value for servo 2
 OCR1BL = 0xFF;	//Output Compare Register low Value For servo 2
 OCR1CH = 0x03;	//Output compare Register high value for servo 3
 OCR1CL = 0xFF;	//Output Compare Register low Value For servo 3
 ICR1H  = 0x03;	
 ICR1L  = 0xFF;
 TCCR1A = 0xAB;   //{COM1A1=1, COM1A0=0; COM1B1=1, COM1B0=0; COM1C1=1 COM1C0=0}
 				 //For Overriding normal port functionality to OCRnA outputs.
				 //{WGM11=1, WGM10=1} Along With WGM12 in TCCR1B for Selecting FAST PWM Mode*/
 TCCR1C = 0x00;
 TCCR1B = 0x0C; //WGM12=1; CS12=1, CS11=0, CS10=0 (Prescaler=256)
}

//***************SERVO**END***********************

/*
*
* Function Name: 	adc_pin_config
* Input: 			void
* Output: 			Sets Ports K and F as input ports
* Logic: 			Set DDRF PORTF DDRK PORTK as 0x00 to set them as input ports
* Example Call:		adc_pin_config()
*
*/
void adc_pin_config (void)
{
	//ADC pin configuration
	DDRF = 0x00;
	PORTF = 0x00;
	DDRK = 0x00;
	PORTK = 0x00;
}

/*
*
* Function Name: 	buzzer_on
* Input: 			void
* Output: 			Turns Buzzer on
* Logic: 			Set PINC3 high to turn the buzzer on
* Example Call:		buzzer_on()
*
*/
void buzzer_on (void)
{
	unsigned char port_restore = 0;
	//port_restore: To retain the original values of PINC
	port_restore = PINC;
	port_restore = port_restore | 0x08;
	PORTC = port_restore;
}


/*
*
* Function Name: 	buzzer_off
* Input: 			void
* Output: 			Turns Buzzer off
* Logic: 			Set PINC3 low to turn the buzzer off
* Example Call:		buzzer_off()
*
*/
void buzzer_off (void)
{
	unsigned char port_restore = 0;
	//port_restore: To retain the original values of PINC
	port_restore = PINC;
	port_restore = port_restore & 0xF7;
	PORTC = port_restore;
}

/*
*
* Function Name: 	buzzer_init
* Input: 			void
* Output: 			Initialises ports if buzzer
* Logic: 			Set PORTC3 as output
* Example Call:		buzzer_init()
*
*/
void buzzer_init()
{
	DDRC = DDRC | 0x08;			//Setting PORTC 3 as output
	PORTC = PORTC & 0xF7;		//Setting PORTC 3 logic low to turn off buzzer
}




/*
*
* Function Name: 	ADC_Conversion
* Input: 			unsigned char channel_number
* Output: 			Converts analog value to digital
* Logic: 			
* Example Call:		ADC_Conversion()
*
*/
unsigned char ADC_Conversion(unsigned char Ch)
{
	unsigned char a;
	if(Ch>7)
	{
		ADCSRB = 0x08;
	}
	Ch = Ch & 0x07;
	ADMUX= 0x20| Ch;
	ADCSRA = ADCSRA | 0x40;		//Set start conversion bit
	while((ADCSRA&0x10)==0);	//Wait for conversion to complete
	a=ADCH;
	ADCSRA = ADCSRA|0x10; //clear ADIF (ADC Interrupt Flag) by writing 1 to it
	ADCSRB = 0x00;
	return a;
}

/*
*
* Function Name: 	adc_init
* Input: 			void
* Output: 			Initialises ports for ADC 
* Logic: 			
* Example Call:		adc_init()
*
*/
void adc_init()
{
	ADCSRA = 0x00;
	ADCSRB = 0x00;		//MUX5 = 0
	ADMUX = 0x20;		//Vref=5V external --- ADLAR=1 --- MUX4:0 = 0000
	ACSR = 0x80;
	ADCSRA = 0x86;		//ADEN=1 --- ADIE=1 --- ADPS2:0 = 1 1 0
}




/*
*
* Function Name: 	ISR
* Input: 			INT0_vect
* Output: 			Increment pulse count by 1
* Logic: 			Increment the pulse count to get the Red or Green or Blue colour pulse count
* Example Call:		Triggered by interrupt no call required
*
*/
ISR(INT0_vect)
{
	pulse++; //increment on receiving pulse from the color sensor
}


/*
*
* Function Name: 	color_sensor_pin_config
* Input: 			void
* Output: 			Configure Interrupt 0 (INT0 : External Interrupt Request 0 )
* Logic: 			Set PD0 as input and enable internal pull-up for PORTD 0 pin
* Example Call:		color_sensor_pin_config()
*
*/
void color_sensor_pin_config(void)
{
	DDRD  = DDRD | 0xFE; //set PD0 as input for color sensor output
	PORTD = PORTD | 0x01;//Enable internal pull-up for PORTD 0 pin
}
/*
*
* Function Name: 	color_sensor_pin_interrupt_init
* Input: 			void
* Output: 			Enable Interrupt 0 (INT0 : External Interrupt Request 0 )
* Logic: 			Set Interrupt 0 to trigger with falling edge and Enable Interrupt INT0 for colour sensor
* Example Call:		color_sensor_pin_interrupt_init()
*
*/
void color_sensor_pin_interrupt_init(void) //Interrupt 0 enable
{
	cli(); //Clears the global interrupt
	EICRA = EICRA | 0x02; // INT0 is set to trigger with falling edge
	EIMSK = EIMSK | 0x01; // Enable Interrupt INT0 for color sensor
	sei(); // Enables the global interrupt
}

/*
*
* Function Name: 	filter_red
* Input: 			void
* Output: 			Red Filter Selection
* Logic: 			PORTD6 & PORTD7 Control S2 and S3 pins of the color sensor to choose the color filter.   
					S2  S3   Filter
					L   L	 RED
* Example Call:		filter_red()
*
*/
void filter_red(void)    //Used to select red filter
{
	PORTD = PORTD & 0xBF; //set S2 low
	PORTD = PORTD & 0x7F; //set S3 low
}
/*
*
* Function Name: 	filter_green
* Input: 			void
* Output: 			Green Filter Selection
* Logic: 			PORTD6 & PORTD7 Control S2 and S3 pins of the color sensor to choose the color filter.   
					S2  S3   Filter
					H   H	 GREEN
* Example Call:		filter_green()
*
*/
void filter_green(void)	//Used to select green filter
{
	PORTD = PORTD | 0x40; //set S2 High
	PORTD = PORTD | 0x80; //set S3 High
}
/*
*
* Function Name: 	filter_blue
* Input: 			void
* Output: 			Green Filter Selection
* Logic: 			PORTD6 & PORTD7 Control S2 and S3 pins of the color sensor to choose the color filter.
					S2  S3   Filter
					L   H	 BLUE
* Example Call:		filter_blue()
*
*/
void filter_blue(void)	//Used to select blue filter
{
	PORTD = PORTD & 0xBF; //set S2 low
	PORTD = PORTD | 0x80; //set S3 High
}
/*
*
* Function Name: 	filter_clear
* Input: 			void
* Output: 			Clear Filter Selection
* Logic: 			PORTD6 & PORTD7 Control S2 and S3 pins of the color sensor to choose the color filter.
					S2  S3   Filter
					H   L	 CLEAR
* Example Call:		filter_clear()
*
*/
void filter_clear(void)	//select no filter
{
	//Filter Select - no filter
	PORTD = PORTD | 0x40; //set S2 High
	PORTD = PORTD & 0x7F; //set S3 Low
}
/*
*
* Function Name: 	color_sensor_scaling
* Input: 			void
* Output: 			Scales down the original frequency of the output by 20%
* Logic: 			Output scaling can be used to increase the resolution for a given clock rate or
					to maximize resolution as the light input changes.
					Scaling is accomplished by internally connecting the pulse-train output
					of the converter to a series of frequency dividers.
					PORTD4 & PORTD5 Control the S0 and S1 pins of the color sensor to choose a scaling factor
* Example Call:		color_sensor_scaling()
*
*/
void color_sensor_scaling()
{
	PORTD = PORTD | 0x10;	//set S0 high
	PORTD = PORTD | 0x20;	//set S1 high
}
/*
*
* Function Name: 	red_read
* Input: 			void
* Output: 			Capture Pulses for red filter and store the count in variable red
* Logic: 			Filter  red colour and capture the pulses for 100ms 
* Example Call:		red_read()
*
*/
void red_read(void) // function to select red filter and display the count generated by the sensor on LCD. The count will be more if the color is red. The count will be very less if its blue or green.
{
	filter_red();	 //select red filter
	pulse=0;	    //reset the count to 0
	_delay_ms(100);//capture the pulses for 100 ms or 0.1 second
	red = pulse;   //store the count in variable called red
}

/*
*
* Function Name: 	green_read
* Input: 			void
* Output: 			Capture Pulses for green filter and store the count in variable green
* Logic: 			Filter  green colour and capture the pulses for 100ms 
* Example Call:		green_read()
*
*/
void green_read(void)
{
	filter_green(); //select green filter
	pulse=0;		//reset the count to 0
	_delay_ms(100); //capture the pulses for 100 ms or 0.1 second
	green = pulse;  //store the count in variable called green
	
}
/*
*
* Function Name: 	blue_read
* Input: 			void
* Output: 			Capture Pulses for blue filter and store the count in variable blue
* Logic: 			Filter  green colour and capture the pulses for 100ms
* Example Call:		blue_read()
*
*/
void blue_read(void)
{
	filter_blue();	 //select blue filter
	pulse=0;		 //reset the count to 0
	_delay_ms(100);	 //capture the pulses for 100 ms or 0.1 second
	blue = pulse;	 //store the count in variable called blue
}

//Function to configure INT4 (PORTE 4) pin as input for the left position encoder
void left_encoder_pin_config (void)
{
	DDRE  = DDRE & 0xEF;  //Set the direction of the PORTE 4 pin as input
	PORTE = PORTE | 0x10; //Enable internal pull-up for PORTE 4 pin
}

//Function to configure INT5 (PORTE 5) pin as input for the right position encoder
void right_encoder_pin_config (void)
{
	DDRE  = DDRE & 0xDF;  //Set the direction of the PORTE 4 pin as input
	PORTE = PORTE | 0x20; //Enable internal pull-up for PORTE 4 pin
}

//Function to initialize ports
void port_init_poz()
{
	motion_pin_config(); //robot motion pins config
	left_encoder_pin_config(); //left encoder pin config
	right_encoder_pin_config(); //right encoder pin config
}

void left_position_encoder_interrupt_init (void) //Interrupt 4 enable
{
	cli(); //Clears the global interrupt
	EICRB = EICRB | 0x02; // INT4 is set to trigger with falling edge
	EIMSK = EIMSK | 0x10; // Enable Interrupt INT4 for left position encoder
	sei();   // Enables the global interrupt
}

void right_position_encoder_interrupt_init (void) //Interrupt 5 enable
{
	cli(); //Clears the global interrupt
	EICRB = EICRB | 0x08; // INT5 is set to trigger with falling edge
	EIMSK = EIMSK | 0x20; // Enable Interrupt INT5 for right position encoder
	sei();   // Enables the global interrupt
}

//ISR for right position encoder
ISR(INT5_vect)
{
	ShaftCountRight++;  //increment right shaft position count
}


//ISR for left position encoder
ISR(INT4_vect)
{
	ShaftCountLeft++;  //increment left shaft position count
}


//Function used for turning robot by specified degrees
void angle_rotate(unsigned int Degrees)
{
	float ReqdShaftCount = 0;
	unsigned long int ReqdShaftCountInt = 0;

	ReqdShaftCount = (float) Degrees/ 4.090; // division by resolution to get shaft count
	ReqdShaftCountInt = (unsigned int) ReqdShaftCount;
	ShaftCountRight = 0;
	ShaftCountLeft = 0;

	while (1)
	{
		if((ShaftCountRight >= ReqdShaftCountInt) | (ShaftCountLeft >= ReqdShaftCountInt))
		break;
	}
	stop(); //Stop robot
}

//Function used for moving robot forward by specified distance

void linear_distance_mm(unsigned int DistanceInMM)
{
	float ReqdShaftCount = 0;
	unsigned long int ReqdShaftCountInt = 0;

	ReqdShaftCount = DistanceInMM / 5.338; // division by resolution to get shaft count
	ReqdShaftCountInt = (unsigned long int) ReqdShaftCount;
	
	ShaftCountRight = 0;
	while(1)
	{
		if(ShaftCountRight > ReqdShaftCountInt)
		{
			break;
		}
	}
	stop(); //Stop robot
}

void forward_mm(unsigned int DistanceInMM)
{
	forward();
	linear_distance_mm(DistanceInMM);
}

void back_mm(unsigned int DistanceInMM)
{
	back();
	linear_distance_mm(DistanceInMM);
}

void left_degrees(unsigned int Degrees)
{
	// 88 pulses for 360 degrees rotation 4.090 degrees per count
	left(); //Turn left
	angle_rotate(Degrees);
}



void right_degrees(unsigned int Degrees)
{
	// 88 pulses for 360 degrees rotation 4.090 degrees per count
	right(); //Turn right
	angle_rotate(Degrees);
}


void soft_left_degrees(unsigned int Degrees)
{
	// 176 pulses for 360 degrees rotation 2.045 degrees per count
	soft_left(); //Turn soft left
	Degrees=Degrees*2;
	angle_rotate(Degrees);
}

void soft_right_degrees(unsigned int Degrees)
{
	// 176 pulses for 360 degrees rotation 2.045 degrees per count
	soft_right();  //Turn soft right
	Degrees=Degrees*2;
	angle_rotate(Degrees);
}

void soft_left_2_degrees(unsigned int Degrees)
{
	// 176 pulses for 360 degrees rotation 2.045 degrees per count
	soft_left_2(); //Turn reverse soft left
	Degrees=Degrees*2;
	angle_rotate(Degrees);
}

void soft_right_2_degrees(unsigned int Degrees)
{
	// 176 pulses for 360 degrees rotation 2.045 degrees per count
	soft_right_2();  //Turn reverse soft right
	Degrees=Degrees*2;
	angle_rotate(Degrees);
}

/*
*
* Function Name: 	port_init
* Input: 			void
* Output: 			Initialises the ports of buzzer,servo 1,servo 2,servo 3 and colour sensor
* Logic: 			Initialises the ports by calling the respective initialisation function
* Example Call:		port_init()
*
*/
void port_init(void)
{
	adc_pin_config();
	buzzer_init();
	servo1_pin_config(); //Configure PORTB 5 pin for servo motor 1 operation
	servo2_pin_config(); //Configure PORTB 6 pin for servo motor 2 operation
	servo3_pin_config(); //Configure PORTB 7 pin for servo motor 3 operation

	color_sensor_pin_config();//color sensor pin configuration
}

/*
*
* Function Name: 	init_devices
* Input: 			void
* Output: 			Initialises all the devices connected to Firebird V 
* Logic: 			Initialises the devices by calling the respective intialisation functions
* Example Call:		init_devices()
*
*/
void init_devices(void)
{
	cli(); //Clears the global interrupt
	port_init_poz();  //Initializes all the ports
	left_position_encoder_interrupt_init();
	right_position_encoder_interrupt_init();
	port_init();  //Initializes all the ports
	color_sensor_pin_interrupt_init();
	adc_init();
	motion_pin_config();
	timer5_init();
	timer1_init();
	sei();   // Enables the global interrupt
}
