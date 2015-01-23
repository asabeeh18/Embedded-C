#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <math.h> //included to support power function
#include "lcd.c"
#include "color.c"

void port_init();
void timer5_init();
void velocity(unsigned char, unsigned char);
void motors_delay();
void onNode();
void open(unsigned char);
unsigned char ADC_Conversion(unsigned char);
void sortMan();
void sortFree();

unsigned char ADC_Value;
unsigned char flag = 0;
unsigned char Left_white_line = 0;
unsigned char Center_white_line = 0;
unsigned char Right_white_line = 0;
//SOrt
unsigned char swap[2]={0};
unsigned char visited[4]={0};
unsigned char term[4][2];
unsigned char indi[4]={0};  //index is color value is terminal
unsigned char schD[2];	//values: 0 1 or 2, access by CT%2
unsigned char schP[2];
unsigned char threshold=1000;
unsigned char RED=1,GREEN=2,BLUE=3,BLACK=4,EMPTY=0;
unsigned char CT=0;
unsigned char arm[2]={0,0};
unsigned char adjCount=0,farCount=0;
unsigned char i;
//
unsigned long int ShaftCountLeft = 0; //to keep track of left position encoder
unsigned long int ShaftCountRight = 0; //to keep track of right position encoder
unsigned int Degrees; //to accept angle in degrees for turning

//++++++===Servo __init__ ========++++++++
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
//Servo control
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

//+++===Sensors ir white sharp
void MOSFET_switch_config (void)
{
	DDRH = DDRH | 0x0C; //make PORTH 3 and PORTH 1 pins as output
	PORTH = PORTH & 0xF3; //set PORTH 3 and PORTH 1 pins to 0

	DDRG = DDRG | 0x04; //make PORTG 2 pin as output
	PORTG = PORTG & 0xFB; //set PORTG 2 pin to 0
}

void turn_on_all_proxy_sensors (void) // turn on Sharp 2, 3, 4, red LED of the white line sensors
// Sharp 1, 5 and IR proximity sensor
{
	PORTH = PORTH & 0xF3; //set PORTH 3 and PORTH 1 pins to 0
	PORTG = PORTG & 0xFB; //set PORTG 2 pin to 0
}//make it efficient
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
		lcd_print(2,1,5,1);
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
	_init_color();
	adc_init();
	left_position_encoder_interrupt_init();
	right_position_encoder_interrupt_init();
	DDRC = DDRC | 0x08;		//Setting PORTC 3 as output
	PORTC = PORTC & 0xF7;
	timer5_init();
	sei();   //Enables the global interrupts
}

void set_color()
{
	Left_white_line = ADC_Conversion(3);	//Getting data of Left WL Sensor
	Center_white_line = ADC_Conversion(2);	//Getting data of Center WL Sensor
	Right_white_line = ADC_Conversion(1);	//Getting data of Right WL Sensor

}

void correct()
{
	unsigned int i=0;
	Degrees=5;
	for(;i<3;i++)
	{
		left(); //Left wheel backward, Right wheel forward
		lcd_print(2,7,777,3);
		angle_rotate(Degrees);
		stop();
		set_color();
		if(Center_white_line>40)
		return;
	}
	//normal
	right();
	while(Center_white_line<40)
	{
		//	lcd_print(2,1,7,1);
		set_color();
		
	}
	return;
}
void noNatak()
{
	//buzzer_on();
	//lcd_print(2,1,7,1);
	correct();
	
	stop();
	//lcd_print(2,1,6,1);
	//buzzer_off();
	
}

void forwardJaa()
{
	do
	{
		set_color();
		if(Center_white_line>40 && (Left_white_line>40 || Right_white_line>40) )
		{
			forward();
			_delay_ms(4000);
			return;
		}
		/*
		print_sensor(1,1,3);	//Prints value of White Line Sensor1
		print_sensor(1,5,2);	//Prints Value of White Line Sensor2
		print_sensor(1,9,1);	//Prints Value of White Line Sensor3
		*/
		forward();
		velocity(200,200);
		
	}while(Center_white_line>0x28);
	
	noNatak();
	forwardJaa();
}

void turnDelay()
{
	forward();
	_delay_ms(6000);

}

void nodeFront()
{
	forwardJaa();
}
void nodeRight()
{
	turnDelay();
	right();
	angle_rotate(90);
}
void nodeLeft()
{
	turnDelay();
	left();
	angle_rotate(100);
}

void buzzer()
{
	
	buzzer_on();
	_delay_ms(1000);
	buzzer_off();
}
void nodeInd()
{
	lcd_print(2,1,0,1);
	
	turnDelay();
	noNatak();
	
	right();
	angle_rotate(90);
	_delay_ms(1000);
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
		lcd_print(2,1,3,1);
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
void uTurn()
{
	buzzer();
	right();
	angle_rotate(190);
	_delay_ms(1000);
}
//bot will always face towards the inside of arena or away
//inside for now
//change it to facing outside
char adjC()
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

//servo_1 states
//init servo at 90 deg
void elevate(unsigned char angle,unsigned char side)
{
	if(side==0)
	servo_1(angle);
	else if(side==1)
	servo_1((angle==45)?0:45);
}
void open(unsigned char side)
{
	if(side==0)
	servo_2(180);
	else if(side==1)
	servo_3(180);
}
void close(unsigned char side)
{
	if(side==0)
	servo_2(0);
	else if(side==1)
	servo_3(0);
}
void pickupSeQ(unsigned char side)
{
	elevate(0,side);//lower
	open(side);
	close(side);
	elevate(45,side);//mid
}
void dropSeQ(unsigned char side)
{
	elevate(0,side);//lower
	open(side);
	elevate(45,side);//mid
	close(side);
}

unsigned char pickup(unsigned char side,unsigned char mode)	//1=swap 0=term
{
	if(mode==0)
	{
		if(arm[side]==0)
		{
			if(term[CT][side]!=indi[CT] && term[CT][side]!=0)
			{
				arm[side]=term[CT][side];
				term[CT][side]=0;
				pickupSeQ(side);
				return 1;
			}
		}
		else if(arm[!side]==0)
		{
			if(term[CT][!side]!=indi[CT] && term[CT][!side]!=0)
			{
				arm[!side]=term[CT][side];
				term[CT][side]=0;
				uTurn();
				pickupSeQ(!side);
				uTurn();
				return 1;
			}
		}
	}
	if(mode==1)
	{
		if(arm[side]==0)
		{
			arm[side]=swap[CT%2];
			swap[CT%2]=0;
			pickupSeQ(side);
			return 1;
		}
		else if(arm[!side]==0)
		{
			arm[!side]=swap[CT%2];
			swap[CT%2]=0;
			pickupSeQ(!side);
			return 1;
		}
	}
	return 0;
}
unsigned char drop(unsigned char side,unsigned char mode)
{
	if(mode==0)
	{
		if(arm[side]!=0)
		{
			if(term[CT][side]==0 && arm[side]==indi[CT])
			{
				term[CT][side]=arm[side];
				arm[side]=0;
				dropSeQ(side);
				return 1;
			}
		}
		else if(arm[!side]!=0)
		{
			if(term[CT][!side]==0 && arm[!side]==indi[CT])
			{
				term[CT][side]=arm[side];
				arm[!side]=0;
				uTurn();
				dropSeQ(!side);
				uTurn();
				return 1;
			}
		}
	}
	if(mode==1)
	{
		if(swap[CT%2]!=0)
		return 0;
		if(arm[side]!=0)
		{
			swap[CT%2]=arm[side];
			arm[side]=0;
			pickupSeQ(side);
			return 1;
		}
		else if(arm[!side]!=0)
		{
			swap[CT%2]=arm[!side];
			arm[!side]=0;
			pickupSeQ(!side);
			return 1;
		}
	}
	return 0;
}


void swapMan(unsigned char sw,unsigned char nxTerm)		//make efficient
{
	unsigned char pSwap=((CT==2||CT==3)?1:0);
	if(nxTerm==indi[swap[sw]])
	{
		if((arm[1]==0 && arm[0]==0) || ((arm[1]==0 || arm[0]==0) && (term[nxTerm][0]==0 || term[nxTerm][1]==1)) )
			i=pickup(pSwap,(unsigned char)1);
	}
	if(nxTerm%2!=CT%2)
	{
		if(nxTerm%2==sw)
		{
			if(arm[0]>0 && arm[1]>0)
			{
				drop((arm[0]==indi[nxTerm]),1);
			}
		}
	}
}
void travel(unsigned char CT,unsigned char nxTerm)
{
	
	forwardJaa();
	//swapEncounterdAction
	swapMan(CT%2,nxTerm);
	/*
	lcd_print(1,11,(CT==1 && (nxTerm == 3 || nxTerm== 4)),1);// ||
	lcd_print(1,12,(CT==4 && (nxTerm == 1 || nxTerm== 2)),1);
	lcd_print(1,13,(CT==2 && (nxTerm == 3 || nxTerm== 4)),1); // ||
	lcd_print(1,14,(CT==3 && (nxTerm == 1 || nxTerm== 2)),1);
	*/
	if((CT==1 && (nxTerm == 3 || nxTerm== 4)) || (CT==4 && (nxTerm == 1 || nxTerm== 2)))
	{
		nodeLeft();
		forwardJaa();
		_delay_ms(1000);
		forwardJaa();
		CT=nxTerm;	//changed for drop,pickup
		swapMan(nxTerm%2,nxTerm);
		if(nxTerm==1 || nxTerm==4)
			nodeRight();
		else
			nodeLeft();
	}
	else if((CT==2 && (nxTerm == 3 || nxTerm== 4)) || (CT==3 && (nxTerm == 1 || nxTerm== 2)))
	{
		nodeRight();
		forwardJaa();
		_delay_ms(500);
		forwardJaa();
		CT=nxTerm;	//changed for drop,pickup
		swapMan(nxTerm%2,nxTerm);
		if(nxTerm==2 || nxTerm==3)
			nodeLeft();
		else
			nodeRight();
	}
	else
	{
		_delay_ms(1000);
	}
	forwardJaa();
	stop();
	stop();
	buzzer();
}
void pick_color(unsigned char node)	//0 or 1
{
	read_color();
	if(red>threshold)
	{
		term[CT][node]=RED;
	}
	else if(green>threshold)
	{
		term[CT][node]=GREEN;
	}
	else if(blue>threshold)
	{
		term[CT][node]=BLUE;
	}
	else if(red>threshold)
	{
		term[CT][node]=BLACK;
	}
	else
	{
		term[CT][node]=EMPTY;
	}
	visited[CT]=1;
	
}
void termPick()
{
	nodeRight();
	pick_color(0);
	lcd_print(2,1,term[CT][0],1);
	uTurn();
	pick_color(1);
}
void counter()
{
	char adj=adjC(); //v r inside d term wid one box of its color and an empty arm and MAYBE 1 box at sort
	adjCount=0;
	farCount=0;

	//belongs to near
	if (indi[CT]==term[adj][0])
		adjCount++;
	
	if (indi[CT]==term[adj][1])
		adjCount++;

	//belongs to far
	if (!(indi[CT]==term[CT][0] && indi[adj]==term[CT][0]))
		farCount++;
	
	if (!(indi[CT]==term[CT][1] && indi[adj]==term[CT][1]))
		farCount++;
}
unsigned char unvisited()
{
	for(i=1;i<=4;i++)
		if(visited[i]==0)
			return i;
	return 0;
}


void gotoSort(unsigned char CT)
{
	forwardJaa();
}

void goBack(unsigned char CT)
{
	if(Center_white_line>40)
	{	
		back();
		set_color();
		while(Center_white_line>40 && (Left_white_line>40 || Right_white_line>40))
		{	return;}
	}
	else
	{
		noNatak();
		goBack(CT);
	}
}


unsigned char checkForCompletion()
{
	for(i=1;i<5;i++)
		if(term[i][0]==indi[i] && term[i][1]==indi[i]);
		else
			return 0;
	return 1;
}
//SORTMAN
//MAIN SORTING FUNCTION
//R-0 L-1
void sortMan()
{
	unsigned char nxTerm=0;
	unsigned char adj=adjC();
	unsigned char pSwap=((CT==2||CT==3)?1:0);
	unsigned char pFar=(term[CT][0]==indi[adj]);		//wont work for empty node
	if(swap[CT%2]==indi[CT])
	{
		if(farCount==2)
		{
			if(term[CT][0]==term[CT][1])
				nxTerm=indi[term[CT][0]]; //the common terminal
			else	//both diff of far
			{
				nxTerm=indi[term[CT][0]];
				pickup(0,0);
				drop(0,0);
				gotoSort(CT);
				pickup(pSwap,1);
				drop(pSwap,1);
				goBack(CT);
				pickup(1,0);
				drop(1,0);
				//SchPickupAtSort	//CT
				//SchDropAtSort		//far
			}
		}
		else if(farCount==1)
		{
			if(adjCount==1)
			{
				nxTerm=adj;
				pickup(pFar,0);
				drop(pFar,0);
				gotoSort(CT);
				pickup(pSwap,1);
				drop(pSwap,1);
				goBack(CT);
				pickup(!pFar,0);
				drop(!pFar,0);
				//nothing to schedule
			}
			else
			{	//empty node
				//:O :O
				if(visited[adj]==0 || term[adj][0]!=indi[adj] || term[adj][1]!=indi[adj])
					nxTerm=adj;
				
				else	//seq also usable for both empty
				{
					nxTerm=indi[term[CT][pFar]];
					pickup(pFar,0);
					drop(pFar,0);
					gotoSort(CT);
					pickup(pSwap,1);
					goBack(CT);
					drop(!pFar,0);
				}
			}
		}
		else if(adjCount==1)
		{
			if(farCount==1)
			{
				nxTerm=adj;
				pickup(pFar,0);
				drop(pFar,0);
				gotoSort(CT);
				pickup(pSwap,1);
				drop(pSwap,1);
				goBack(CT);
				pickup(!pFar,0);
				drop(!pFar,0);
				//nothing to schedule
			}
			else
			{
				//empty node
				drop(pFar,0);
				gotoSort(CT);
				pickup(pSwap,1);
				goBack(CT);
				pickup(!pFar,0);
				drop(!pFar,0);
				nxTerm=adj;
			}
		}
		else if(adjCount==2)
		{
			nxTerm=adj;
			pickup(0,0);
			drop(0,0);
			gotoSort(CT);
			pickup(pSwap,1);
			drop(pSwap,1);
			goBack(CT);
			pickup(1,0);
			drop(1,0);
	}
	if(arm[0]==0 && arm[1]==0)
	{
		travel(CT,nxTerm);
		sortFree();
	}
	else
	{
		travel(CT,nxTerm);
		sortMan();
	}
		
}

	
	else if(swap[CT%2]!=indi[CT])	//Sort can be empty
	{
		if(farCount==2)
		{
			if(term[CT][0]==term[CT][1])	//and sort not empty why ?? :/
			{
				nxTerm=indi[term[CT][0]]; //the common terminal
				//SchPickupAtSort
				//SchDropAtSort
				pickup(0,0);
				drop(0,0);
				pickup(1,0);
				drop(1,0);
			}
			else if(swap[CT%2]==term[CT][0])
			{
				pickup(0,0);
				drop(0,0);
				nxTerm=indi[arm[0]];
			}
			else if(swap[CT%2]==term[CT][1])
			{
				pickup(1,0);
				drop(1,0);
				nxTerm=indi[arm[1]];
			}
		}
		else if(farCount==1)
		{
			if(adjCount==1)
			{
				if(swap[CT%2]==0)
				{
					pickup(!pFar,0); //adjwala
					drop(!pFar,0);
					pickup(pFar,0);
					nxTerm=adj;
					//SchDropAtSort
				}
				else
				{
					//Sort of far and CT has 1 far a adj
					pickup(!pFar,0);
					drop(!pFar,0);
					nxTerm=adj;
				}
			}
			else  //====
			{	//one node is empty
				nxTerm=indi[term[CT][pFar]];
				pickup(pFar,0);
				drop(pFar,0);
				//SchDropAtSort
			}
		}
		
		else if(adjCount==1)
		{
			if(farCount==1)
			{
				if(swap[CT%2]==0)
				{
					pickup(!pFar,0); //adjwala
					drop(!pFar,0);
					pickup(pFar,0);
					nxTerm=adj;
					//SchDropAtSort
				}
				else
				{
					//Sort of far and CT has 1 far a adj
					pickup(!pFar,0);
					drop(!pFar,0);
					nxTerm=adj;
				}
			}
			else	//1 empty 1 adj
			{
				pickup(!pFar,0);
				drop(!pFar,0);
				nxTerm=adj;
			}
		}
		else if(adjCount==2)
		{
			if(swap[CT%2]>0) //smthin is at Sort
			{
				pickup(0,0);
				drop(0,0);
			}
			else
			{
				pickup(0,0);
				drop(0,0);
				pickup(1,0);
			}
			nxTerm=adj;
		}
		else		//empty
		{
			drop(0,0);
			drop(1,0);
			nxTerm=unvisited();
			if(nxTerm==0)
			{
				if(checkForCompletion())
				{
					//Exit Sequence
				}
			}
		}
	}
}
void sortFree()
{
	unsigned char nxTerm=0;
	unsigned char adj=adjC();
	counter();
	if(farCount==2)
	{
		nxTerm=indi[arm[0]];
		//SchDropAtSort(nxTerm);
	}
	else if(adjCount==1)
	{
		if(farCount==1)
		{
			nxTerm=adj;
			//SchDropAtSort(nxTerm);
		}
		else	//1 empty block
		{
			nxTerm=adj;
		}
	}
	else if(farCount==1)
	{
		if(adjCount==1)
		{
			nxTerm=adj;
			//SchDropAtSort(nxTerm);
		}
		else	//1 empty block
		{
			if(visited[adj]==0)
			{
				nxTerm=adj;
				//SchDropAtSort(nxTerm);
			}
			else
				nxTerm=arm[0];
				
		}
	}
	else if(adjCount==2)
	{
		nxTerm=adj;
		//SchDropAtSort(nxTerm);
	}
	else	//empty term
	{
		if(visited[adj]==0)
			nxTerm=adj;
		else
			unvisited();
	}
	if(arm[0]==0 && arm[1]==0)
	{
		travel(CT,nxTerm);
		sortFree();
	}
	else
	{
		travel(CT,nxTerm);
		sortMan();
	}
}

	void sorter()
	{
		pickup(0,0);
		pickup(1,0);
		sortFree();
	}
	void init_sort()
	{
		sorter();
	}

	//Main Function
	int main()
	{
		init_devices();
		lcd_set_4bit();
		lcd_init();
		init_sort();
		//travel(1,3);
		//buzzer();
		while(1)
		{
		}
		return 1;
	}