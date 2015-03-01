#include "strong.c"
unsigned char ADC_Value;
unsigned char flag = 0;
unsigned char Left_white_line = 0;
unsigned char Center_white_line = 0;
unsigned char Right_white_line = 0;

void onNode();
//unsigned int Degrees; //to accept angle in degrees for turning


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
			lcd_print(2,1,i,1);
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
		lcd_print(2,1,7,1);
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
	return;
}

void forwardJaa()
{
	do
	{
		set_color();
		if(Center_white_line>40 && (Left_white_line>40 || Right_white_line>40) )
		{
			buzzer();
		}
		print_sensor(1,1,3);	//Prints value of White Line Sensor1
		print_sensor(1,5,2);	//Prints Value of White Line Sensor2
		print_sensor(1,9,1);	//Prints Value of White Line Sensor3
		
		forward();
		velocity(200,200);
		
	}while(Center_white_line>0x28);
	
	noNatak();
	return;
}

void nodeFront()
{
	forwardJaa();
}
void nodeRight()
{
	right();
	angle_rotate(90);
}
void nodeLeft()
{
	left();
	angle_rotate(90);
}

void buzzer()
{
	
	buzzer_on();
	_delay_ms(100);
	buzzer_off();
}
void nodeInd()
{
	lcd_print(2,1,0,1);
	
	forward();
	_delay_ms(8000);
	noNatak();
	
	right();
	angle_rotate(90);
	_delay_ms(500);
	buzzer();
	
	right();
	
	angle_rotate(190);
	_delay_ms(800);
	buzzer();
	
	right();
	angle_rotate(90);
	buzzer();
	forward();
	_delay_ms(800);
}
void onNode()
{
	static unsigned int nodeCount=0;
	nodeCount++;
	if(nodeCount==1)
	{
		lcd_print(2,1,3,1);
		forward();
		_delay_ms(700);
	}
	else if(nodeCount==2 || nodeCount==3)
	{
		lcd_print(2,1,4,1);
		nodeInd();
	}
}

//Main Function
int main()
{
	init_devices();
	lcd_set_4bit();
	lcd_init();
	
	while(1)
	{
		forwardJaa();	
	}	
}