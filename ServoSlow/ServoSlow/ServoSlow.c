#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lcd.c"
int cur_angle=90;
//Configure PORTB 5 pin for servo motor 1 operation
void servo1_pin_config (void)
{
 DDRB  = DDRB | 0x20;  //making PORTB 5 pin output
 PORTB = PORTB | 0x20; //setting PORTB 5 pin to logic 1
}
void servo1_off()
{
	DDRB = DDRB | 0x00;
	PORTB = PORTB | 0x00; //setting PORTB 5 pin to logic 1
}
//Configure PORTB 7 pin for servo motor 3 operation
void servo3_off (void)
{
	DDRB  = DDRB | 0x00;  //making PORTB 7 pin output
	PORTB = PORTB | 0x00; //setting PORTB 7 pin to logic 1
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

//Initialize the ports
void port_init(void)
{
 servo1_pin_config(); //Configure PORTB 5 pin for servo motor 1 operation
 servo2_pin_config(); //Configure PORTB 6 pin for servo motor 2 operation 
 servo3_pin_config(); //Configure PORTB 7 pin for servo motor 3 operation  
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
 TCCR1A = 0xAB; /*{COM1A1=1, COM1A0=0; COM1B1=1, COM1B0=0; COM1C1=1 COM1C0=0}
 					For Overriding normal port functionality to OCRnA outputs.
				  {WGM11=1, WGM10=1} Along With WGM12 in TCCR1B for Selecting FAST PWM Mode*/
 TCCR1C = 0x00;
 TCCR1B = 0x0C; //WGM12=1; CS12=1, CS11=0, CS10=0 (Prescaler=256)
}


//Function to initialize all the peripherals
void init_devices(void)
{
 cli(); //disable all interrupts
 DDRC = DDRC | 0x08;		//Setting PORTC 3 as output
 PORTC = PORTC & 0xF7;
 port_init();
 timer1_init();
 sei(); //re-enable interrupts 
}


//Function to rotate Servo 1 by a specified angle in the multiples of 1.86 degrees
void servo_1(unsigned char degrees)  
{
 float PositionPanServo = 0;
  PositionPanServo = ((float)degrees / 1.86) + 35.0;
 OCR1AH = 0x00;
 OCR1AL = (unsigned char) PositionPanServo;
}


//Function to rotate Servo 2 by a specified angle in the multiples of 1.86 degrees
void servo_2(unsigned char degrees)
{
 float PositionTiltServo = 0;
 PositionTiltServo = ((float)degrees / 1.86) + 35.0;
 OCR1BH = 0x00;
 OCR1BL = (unsigned char) PositionTiltServo;
}

//Function to rotate Servo 3 by a specified angle in the multiples of 1.86 degrees
void servo_3(unsigned char degrees)
{
 float PositionServo = 0;
 PositionServo = ((float)degrees / 1.86) + 35.0;
 OCR1CH = 0x00;
 OCR1CL = (unsigned char) PositionServo;
}

//servo_free functions unlocks the servo motors from the any angle 
//and make them free by giving 100% duty cycle at the PWM. This function can be used to 
//reduce the power consumption of the motor if it is holding load against the gravity.

void servo_1_free (void) //makes servo 1 free rotating
{
 OCR1AH = 0x03; 
 OCR1AL = 0xFF; //Servo 1 off
}

void servo_2_free (void) //makes servo 2 free rotating
{
 OCR1BH = 0x03;
 OCR1BL = 0xFF; //Servo 2 off
}

void servo_3_free (void) //makes servo 3 free rotating
{
 OCR1CH = 0x03;
 OCR1CL = 0xFF; //Servo 3 off
} 

void lower(unsigned char side)
{
	if(side==1)
	{
		servo_2(50);
	}
	else
		servo_2(120);
}
void elevate()
{
	servo_2(90);
	_delay_ms(600);
	servo_2_free();
}
void open(unsigned char side)
{
	if (side == 0)
	{
		servo_3(0);
		_delay_ms(500);
		servo_3_free();
	}
	else
	{
		servo_1(0);
		_delay_ms(500);
		servo_1_free();
	}
}
void close(unsigned char side)
{
	if (side == 0)
		servo_3(40);
	else if (side == 1)
		servo_1(60);
}
void pick(int side)
{
	lower(side);
	_delay_ms(1000);
	close(side);
	_delay_ms(3000);
	elevate();//mid
	_delay_ms(1000);
}
void drop(int side)
{
	//	armCount++;
	lower(side);//lower
	_delay_ms(1000);
	open(side);
	_delay_ms(3000);
	elevate();//mid
	_delay_ms(1000);
}
void buzzer_on (void)
{
	unsigned char port_restore = 0;
	port_restore = PINC;
	port_restore = port_restore | 0x08;
	PORTC = port_restore;
}

void buzzer_off (void)
{
	unsigned char port_restore = 0;
	port_restore = PINC;
	port_restore = port_restore & 0xF7;
	PORTC = port_restore;
}

//Main function
int main(void)
{
	
	//servo 1 & 3 0 open 40 close
	//servo 1 60 close
	unsigned char i = 0;
	init_devices();
	servo_3(0);
	servo_2(90);
	servo_1(0);
	while(1)
	{
		_delay_ms(2000);
		pick(1);
		_delay_ms(2000);
		drop(1);
		_delay_ms(2000);
		pick(0);
		_delay_ms(2000);
		drop(0);
	}
	//servo_3(0);
	//pick(1);
	
	_delay_ms(2000);
	pick(0);
	while(1);
	
	drop(0);
	
	 return 0;
}
