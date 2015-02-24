/*
 * ShreyaBlackLine.c
 *
 * Created: 2/24/2015 12:43:43 PM
 *  Author: Ahmed
 */ 

#define F_CPU 14745600
#include <avr/io.h>
#include <stdio.h>
#include "strong.c"
unsigned char Left_white_line = 0;
unsigned char Center_white_line = 0;
unsigned char Right_white_line = 0;

unsigned int prev, curr;

void set_color()
{
	Left_white_line = ADC_Conversion(3);	//Getting data of Left WL Sensor
	Center_white_line = ADC_Conversion(2);	//Getting data of Center WL Sensor
	Right_white_line = ADC_Conversion(1);	//Getting data of Right WL Sensor
	print_sensor(1,1,3);	//Prints value of White Line Sensor1
	print_sensor(1,5,2);	//Prints Value of White Line Sensor2
	print_sensor(1,9,1);	//Prints Value of White Line Sensor3
}

void goForward()
{
	set_color();
		while(Center_white_line>50)
		{
			set_color();
			forward();
			velocity(200,200);
			_delay_ms(100);
			set_color();
		}
		
		prev = Center_white_line;
		forward();
		velocity(200,180);
		_delay_ms(100);
		
		set_color();
		curr = Center_white_line;
		
		if (prev<curr || prev==curr)
		{
			while(Center_white_line<70 && Right_white_line<70)
			{		
				set_color();						
				forward();
				velocity(180,200);
				set_color();
			}
			if(Right_white_line>=70)
			{
				while(Center_white_line<70)
				{
					set_color();
					forward();
					velocity(200,150);
					set_color();
				}
				forward();
				velocity(150,200);
				_delay_ms(100);
			}
			else
			{
				forward();
				velocity(200,180);
				_delay_ms(100);
			}
			
		}
		else
		{
			while (Center_white_line<70)
			{
				forward();
				velocity(200,180);
				set_color();
			}
			forward();
			velocity(180,200);
			_delay_ms(100);
		}
	forward();
	velocity(200,200);
	set_color();
}

int main(void)
{
	init_devices();
	lcd_set_4bit();
	lcd_init();
	set_color();
    goForward();

}