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
unsigned char ADC_Conversion(unsigned char);
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
unsigned char threshold=10000;
unsigned char RED=0,GREEN=1,BLUE=2,BLACK=3,EMPTY=4;
unsigned char CT=0;
unsigned char adjCount=0,farCount=0;
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


// forward jaa plz

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
	//swapEncounterdAction
	lcd_print(1,11,(CT==1 && (nxTerm == 3 || nxTerm== 4)),1);// ||
	lcd_print(1,12,(CT==4 && (nxTerm == 1 || nxTerm== 2)),1);
	lcd_print(1,13,(CT==2 && (nxTerm == 3 || nxTerm== 4)),1); // ||
	lcd_print(1,14,(CT==3 && (nxTerm == 1 || nxTerm== 2)),1);
	if((CT==1 && (nxTerm == 3 || nxTerm== 4)) || (CT==4 && (nxTerm == 1 || nxTerm== 2)))
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
	else if((CT==2 && (nxTerm == 3 || nxTerm== 4)) || (CT==3 && (nxTerm == 1 || nxTerm== 2)))
	{
		nodeRight();
		forwardJaa();
		_delay_ms(500);
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
	solveNode //v r inside d term wid one box of its color and an empty arm and MAYBE 1 box at sort
	adjCount=0;
	farCount=0;

	//belongs to near
	if indi[CT]==term[adj][0])
		adjCount++;
	
	if indi[CT]==term[adj][1]
		adjCount++;

	//belongs to far
	if !(indi[CT]==term[CT][0] && indi[adj]==term[CT][0] )
		farCount++;
	
	if !(indi[CT]==term[CT][1] && indi[adj]==term[CT][1])
		farCount++;

}
//SORTMAN
//MAIN SORTING FUNCTION
//R-0 L-1
void sortMan()
{
	
	int pSwap=((CT==2||CT==3)?1:0);
	int pFar=(term[CT][0]==indi[adj]);		//wont work for empty node
	if(Sort[CT]==indi[CT])
	{
		if(farCount==2)
		{
			if(term[CT][0]==term[CT][1])
				nxTerm=indi[term[CT][0]]; //the common terminal
			else	//both diff of far
				nxTerm=indi[term[CT][0]];
			pickup(0);
			drop(0);
			gotoSort(CT);
			pickup(pSwap);
			drop(pSwap);
			goBack(CT);
			pickup(1); 
			drop(1);
			//SchPickupAtSort	//CT
			//SchDropAtSort		//far
		}
		if(farCount==1)
		{
			if(adjCount==1)
			{	
				nxTerm=adj;
				pickup(pFar)
				drop(pFar);
				gotoSort(CT);
				pickup(pSwap);
				drop(pSwap);
				goBack(CT);
				pickup(!pFar)
				drop(!pFar);
				//nothing to schedule
			}
			else
			{	//empty node
				//:O :O
				if(vis[adj]==0 || term[adj][0] || term[adj][1]==or not yet completed(has a far box)
					nxTerm=adj;
				
				else	//seq also usable for both empty
					nxTerm=far;
				pickup(pFar);
				drop(pFar);
				gotoSort(CT);
				pickup(pSwap);
				goBack(CT);
				drop(!pFar);
			}
		}
		if(adjCount==1)
		{	
			if(farCount==1)
			{
					nxTerm=adj;
					pickup(pFar)
					drop(pFar);
					gotoSort(CT);
					pickup(pSwap);
					drop(pSwap);
					goBack(CT);
					pickup(!pFar)
					drop(!pFar);
					//nothing to schedule
			}
			else
			{
					//empty node
					drop(pFar);
					gotoSOrt(CT);
					pickup(pSwap);
					goBack(CT);
					pickup(!pFar);
					drop(!pFar);
					nxTerm=adj;
			}
		}
		if(adjCount==2)
		{
			nxTerm=adj;
			pickup(0); 
			drop(0);
			gotoSort(CT);
			pickup(pSwap);
			drop(pSwap);
			goBack(CT);
			pickup(1);
			drop(1);
		}
	}

		
		

else if(Sort[CT]!=indi[CT])	//Sort can be empty
{
	if(farCount==2)
	{	if(term[CT][0]==term[CT][1] && sort[CT] not empty):
		{
			nxTerm=far; //the common terminal
			//SchPickupAtSort
			//SchDropAtSort
			pickup(0);
			drop(0);
			if(arm[0] and arm[1] not empty)
			{
				goto sort
				pickup 
				drop
				goto CT
			}
			pickup
			drop
		}
		
		else if(term[CT][0]==Sort[CT])
		{	nxTerm=To wherever it belongs
			pickup 0
			drop
			//SchPickupAtSort
		}
		else if(term[CT][1]==Sort[CT])
		{	nxTerm=To wherever it belongs
			pickup 1
			drop
			//SchPickupAtSort
		}
		else sort belongs to adj or is empty
		{		pickup // the box which belongs to term with empty node
				drop
				pickup
				//SchPickupAtSort
				
		/* at this point adj will atleast hav one far box and atmost 1 adj box 
		and 1 box not belonging to it*/
		}
	if(farCount==1)
		if(adjCount==1)
			if(Sort[CT]==indi[adj])
				pickup adjwala
				drop
				//SchPickupAtSort
			else
				//Sort of far and CT has 1 far a adj
				if(Sort[CT]==
		else  //====
			//one node is empty
			pickup 
			drop
			//SchDropAtSort
			nxTerm=far //for Sort
	else if(adjCount==2)
		if(Sort[CT]==indi[CT])
			pickup 0
			pickup 1
			SchDropAtSort();
			nxTerm=adj;
		else //===
			pikcup
			drop
			pickup
			nxTerm=adj;
			
	if(adjCount==1)
		if(farCount==1)
			pickUp adj
			drop
			pickup
		else	//1 empty 1 adj
			pickup
			drop
		nxTerm=adj; 
		//one node is empty
			pickup
			drop
			nxTerm=adj //for Sort
	else		//empty
	{
		drop();
		nxTerm=unvisited();
}

void sortNoSwap()
{
	if(farCount==2)
	{
		nxTerm=indi[arm[0]];
		SchDropAtSort(nxTerm);
	}
	else if(adjCount==1)
	{
		if(farCount==1)
		{
			nxTerm=adj;
			SchDropAtSort(nxTerm);
		}
		else	//1 empty block
		{
			pickup;
			nxTerm=adj;	
		}
	}
	else if(farCount==1)
	{
		if(adjCount==1)
		{
			nxTerm=adj;
			SchDropAtSort(nxTerm);
		}
		else	//1 empty block
		{
			if(visited[adj]==0)
				nxTerm=adj;
				SchDropAtSort(nxTerm);
			else
				nxTerm=arm[0];
				
		}
	}
	else if(adjCount==2)
	{
		nxTerm=adj;
		SchDropAtSort(nxTerm);
	}
	else	//empty term
	{
		if(visited[adj]==0)
			nxTerm=adj;
		else
			unvisited();
	}
}

void sorter()
{
	counter();
	pickUp(0);
	pickUp(1);
	sortNoSwap();
	travel(CT,nxTerm);
	sortMan();
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
}