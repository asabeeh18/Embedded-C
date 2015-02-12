#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <math.h> //included to support power function
#include "lcd.c"
void turnDelay();
void port_init();
void timer5_init();
void velocity(unsigned char, unsigned char);
void motors_delay();
void onNode();
void buzzer();
unsigned char ADC_Conversion(unsigned char);
unsigned char ADC_Value;
unsigned char flag = 0;
unsigned char Left_white_line = 0;
unsigned char Center_white_line = 0;
unsigned char Right_white_line = 0;
//
unsigned long int ShaftCountLeft = 0; //to keep track of left position encoder
unsigned long int ShaftCountRight = 0; //to keep track of right position encoder
unsigned int Degrees; //to accept angle in degrees for turning


//Function to configure LCD port
void lcd_port_config (void)
{
 DDRC = DDRC | 0xF7; //all the LCD pin's direction set as output
 PORTC = PORTC & 0x80; // all the LCD pins are set to logic 0 except PORTC 7
}

//ADC pin configuration
void adc_pin_config (void)
{
 DDRF = 0x00; 
 PORTF = 0x00;
 DDRK = 0x00;
 PORTK = 0x00;
}

//Function to configure ports to enable robot's motion
void motion_pin_config (void) 
{
 DDRA = DDRA | 0x0F;
 PORTA = PORTA & 0xF0;
 DDRL = DDRL | 0x18;   //Setting PL3 and PL4 pins as output for PWM generation
 PORTL = PORTL | 0x18; //PL3 and PL4 pins are for velocity control using PWM.
}
//

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

//Function to Initialize PORTS
void port_init()
{
	lcd_port_config();
	adc_pin_config();
	motion_pin_config();	
	//
	left_encoder_pin_config(); //left encoder pin config
	right_encoder_pin_config(); //right encoder pin config

}

// Timer 5 initialized in PWM mode for velocity control
// Prescale:256
// PWM 8bit fast, TOP=0x00FF
// Timer Frequency:225.000Hz
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
	TCCR5A = 0xA9;	/*{COM5A1=1, COM5A0=0; COM5B1=1, COM5B0=0; COM5C1=1 COM5C0=0}
 					  For Overriding normal port functionality to OCRnA outputs.
				  	  {WGM51=0, WGM50=1} Along With WGM52 in TCCR5B for Selecting FAST PWM 8-bit Mode*/
	
	TCCR5B = 0x0B;	//WGM12=1; CS12=0, CS11=1, CS10=1 (Prescaler=64)
}

void adc_init()
{
	ADCSRA = 0x00;
	ADCSRB = 0x00;		//MUX5 = 0
	ADMUX = 0x20;		//Vref=5V external --- ADLAR=1 --- MUX4:0 = 0000
	ACSR = 0x80;
	ADCSRA = 0x86;		//ADEN=1 --- ADIE=1 --- ADPS2:0 = 1 1 0
}

//Function For ADC Conversion
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

//Function To Print Sesor Values At Desired Row And Coloumn Location on LCD
void print_sensor(char row, char coloumn,unsigned char channel)
{
	
	ADC_Value = ADC_Conversion(channel);
	lcd_print(row, coloumn, ADC_Value, 3);
}

//Function for velocity control
void velocity (unsigned char left_motor, unsigned char right_motor)
{
	OCR5AL = (unsigned char)left_motor;
	OCR5BL = (unsigned char)right_motor;
}
//
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



//Function used for setting motor's direction
void motion_set (unsigned char Direction)
{
 unsigned char PortARestore = 0;

 Direction &= 0x0F; 		// removing upper nibbel for the protection
 PortARestore = PORTA; 		// reading the PORTA original status
 PortARestore &= 0xF0; 		// making lower direction nibbel to 0
 PortARestore |= Direction; // adding lower nibbel for forward command and restoring the PORTA status
 PORTA = PortARestore; 		// executing the command
}

void forward (void) 
{
  motion_set (0x06);
}

void stop (void)
{
  motion_set (0x00);
}

void back (void) //both wheels backward
{
	motion_set(0x09);
}

void left (void) //Left wheel backward, Right wheel forward
{
	motion_set(0x05);
}

void right (void) //Left wheel forward, Right wheel backward
{
	motion_set(0x0A);
}

void soft_left (void) //Left wheel stationary, Right wheel forward
{
	motion_set(0x04);
}

void soft_right (void) //Left wheel forward, Right wheel is stationary
{
	motion_set(0x02);
}

void soft_left_2 (void) //Left wheel backward, right wheel stationary
{
	motion_set(0x01);
}

void soft_right_2 (void) //Left wheel stationary, Right wheel backward
{
	motion_set(0x08);
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
		//lcd_print(2,1,5,1);
		if((ShaftCountRight >= ReqdShaftCountInt) | (ShaftCountLeft >= ReqdShaftCountInt))
		{
			
				break;
		}
		
	}
	stop(); //Stop robot
	return;
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


void init_devices (void)
{
 	cli(); //Clears the global interrupts
	port_init();
	adc_init();
	left_position_encoder_interrupt_init();
	right_position_encoder_interrupt_init();
	DDRC = DDRC | 0x08;		//Setting PORTC 3 as output
	PORTC = PORTC & 0xF7;
	timer5_init();
	sei();   //Enables the global interrupts
}


// forward jaa plz

void set_color()
{
	Left_white_line = ADC_Conversion(3);	//Getting data of Left WL Sensor
	Center_white_line = ADC_Conversion(2);	//Getting data of Center WL Sensor
	Right_white_line = ADC_Conversion(1);	//Getting data of Right WL Sensor

}

void correct()
{
	if(Center_white_line<40 && Left_white_line<40 && Right_white_line<40)
	{
		lcd_print(2,1,4,1);
		left();
		_delay_ms(delay);
		set_color();
		if(Center_white_line>40 || Right_white_line>40)
		{
			return;
		}
		stop();
		right();
		_delay_ms(2*delay);
		set_color();
		if(Center_white_line>40 || Left_white_line>40)
		{
			return;
		}
	}
	
}
void noNatak()
{
	velocity(150,150);
	//buzzer_on();
	//lcd_print(2,1,7,1);
	set_color();
//	lcd_print(2,1,0,1);
	
	lcd_print(1,5,1,1);
    if(Left_white_line>40)
	{
		//lcd_print(2,1,4,1);
		do
		{
			left();
			_delay_ms(100);
			set_color();
			if(Left_white_line<40){
				return;
			}
		}while(Left_white_line>40);
	}
	else if(Right_white_line>40)
	{
		//lcd_print(2,1,8,1);
		do
		{
			right();
			_delay_ms(100);
			set_color();
			if(Right_white_line<40){
				return;
			}
		}while(Right_white_line>40);
	}
	else
		correct();
	stop();
	//lcd_print(2,1,6,1);
	//buzzer_off();
	
}

void forwardJaa()
{
	velocity(255,255);
	do
	{
		set_color();
		if(Center_white_line>40 && (Right_white_line>40 || Left_white_line>40))
		{
			turnDelay();
			buzzer();			
				return;
		}
		forward();
		
		
	}while(Center_white_line>40 && Right_white_line<40 && Left_white_line<40);
	
	noNatak();
	forwardJaa();
}

void turnDelay()
{	
	forward();
	_delay_ms(4000);

}

void nodeFront()
{
	forwardJaa();
}
void nodeRight()
{
	turnDelay();
	right_degrees(100);
	noNatak();
}
void nodeLeft()
{
	turnDelay();
	left_degrees(100);
	noNatak();	
}

void buzzer()
{
	
	buzzer_on();
	_delay_ms(1000);
	buzzer_off();
}
void nodeInd()
{
//	lcd_print(2,1,0,1);
	
	turnDelay();
	//noNatak();
	
	right();
	angle_rotate(100);
	_delay_ms(3000);
	buzzer();
	
	right();
	
	angle_rotate(190);
	_delay_ms(1000);
	buzzer();
	
	right();
	angle_rotate(80);
	buzzer();
	forward();
	_delay_ms(5000);
}
void onNode()
{
	static unsigned int nodeCount=0;
	nodeCount++;
	if(nodeCount==1)
	{
		//lcd_print(2,1,3,1);
		forward();
		_delay_ms(5000);
	}
	else if(nodeCount==2 || nodeCount==3)
	{
		lcd_print(2,1,4,1);
		nodeInd();
	}
	else if(nodeCount==4)
	{
		nodeLeft();
	}
	else if(nodeCount==5)
	{
		nodeRight();
	}
	else if(nodeCount==6)
	{
		stop();
		buzzer_on();
		while(1);
	}
}

//bot will always face towards the inside of arena or away
//inside for now

char adjC(unsigned char CT)
{
	
	if(CT==1)
	return 2;
	if(CT==2)
	return 1;
	if(CT==3)
	return 4;
	if(CT==4)
	return 3;
	else return 0;
}
void travel(int CT,int nxTerm)
{
	
	forwardJaa();
	lcd_print(2,15,1,1);
	//swapEncounterdAction
	/*lcd_print(1,11,(CT==1 && (nxTerm == 3 || nxTerm== 4)),1);// ||
	lcd_print(1,12,(CT==4 && (nxTerm == 1 || nxTerm== 2)),1);
	lcd_print(1,13,(CT==2 && (nxTerm == 3 || nxTerm== 4)),1); // ||
	lcd_print(1,14,(CT==3 && (nxTerm == 1 || nxTerm== 2)),1);
	*/
	lcd_print(1,1,1,1);
	if((CT==1 && (nxTerm == 2 || nxTerm== 4)) || (CT==4 && (nxTerm == 1 || nxTerm== 3)))
	{
		nodeLeft();
		forwardJaa();
		_delay_ms(1000);
	
		forwardJaa();
		if(nxTerm==1 || nxTerm==4)
			nodeRight();
		else
			nodeLeft();
	}
	else if((CT==2 && (nxTerm == 3 || nxTerm== 1)) || (CT==3 && (nxTerm == 4 || nxTerm== 2)))
	{
		nodeRight();
		stop();
		forwardJaa();
		stop();
		_delay_ms(500);
		lcd_print(1,2,1,1);
		forwardJaa();
		//swapEncounterdAction
		if(nxTerm==2 || nxTerm==3)
			nodeLeft();
		else
			nodeRight();
	}
	else
	{
		_delay_ms(1000);
	}
	lcd_print(1,3,1,1);
	forwardJaa();	
	lcd_print(2,15,2,1);
	stop();
	buzzer();
	right();
	_delay_ms(2000);
	stop();
	buzzer();
}


//Main Function
int main()
{
	char m[5]="Ahmed";
	init_devices();
	lcd_set_4bit();
	lcd_init();
	lcd_wr_command(0x01);
	lcd_cursor(2,1);
	lcd_string(m);

	//lcd_print(1,1,1,1);
	travel(3,1);
	travel(1,3);
	//travel(1,3);
	//buzzer();
	while(1)
	{
	}	
}