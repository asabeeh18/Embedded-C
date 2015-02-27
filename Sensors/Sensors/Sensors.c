#define F_CPU 14745600
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <math.h> //included to support power function
#include "strong.c"
unsigned int Left_white_line,Center_white_line,Right_white_line;
void port_init();
void timer5_init();
void velocity(unsigned char, unsigned char);
void motors_delay();
void onNode();
//Servo Test
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


// forward jaa plz

void set_color()
{
	Left_white_line = ADC_Conversion(3);	//Getting data of Left WL Sensor
	Center_white_line = ADC_Conversion(2);	//Getting data of Center WL Sensor
	Right_white_line = ADC_Conversion(1);	//Getting data of Right WL Sensor
	//read_color();
}

//Main Function
int main()
{
	int i=0;
	init_devices();
	lcd_set_4bit();
	lcd_init();
	color_sensor_scaling();
	//travel(1,3);
	//buzzer();
	while(1)
	{
		
		print_sensor(2,1,4);
		print_sensor(2,6,8);
	}
}