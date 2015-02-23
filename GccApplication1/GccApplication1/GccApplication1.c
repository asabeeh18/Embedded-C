/*
 * GccApplication4.c
 *
 * Created: 2/23/2015 12:45:16 PM
 *  Author: Mohit
 */

#define F_CPU 14745600
#include "strong.c"

void port_init();
void timer5_init();
void velocity(unsigned char, unsigned char);
void motors_delay();

unsigned char ADC_Conversion(unsigned char);
unsigned char ADC_Value;

unsigned char Left_white_line = 0;
unsigned char Center_white_line = 0;
unsigned char Right_white_line = 0;
unsigned char missR=0,missL=0,miss=0;

void set_color()
{
	Left_white_line = ADC_Conversion(3);	//Getting data of Left WL Sensor
	Center_white_line = ADC_Conversion(2);	//Getting data of Center WL Sensor
	Right_white_line = ADC_Conversion(1);	//Getting data of Right WL Sensor

	print_sensor(1,1,3);	//Prints value of White Line Sensor1
	print_sensor(1,5,2);	//Prints Value of White Line Sensor2
	print_sensor(1,9,1);	//Prints Value of White Line Sensor3	
}

void atNode()
{
	buzzer_on();
	forward();
	velocity(0,0);
	_delay_ms(1000);
	buzzer_off();
}

void fixLeft()
{
	missL++;
	set_color();
	velocity(100,100);
	left();
	set_color();
	if(Center_white_line>0x28)
		return;
}

void fixRight()
{
	missR++;
	set_color();
	velocity(100,100);
	right();
	set_color();
	if(Center_white_line>0x28)
		return;
}

void fixPath()
{
	set_color();
	if(Left_white_line>0x28 && Right_white_line>0x28)
	{
		set_color();
		atNode();		
	}
	
	else if(Left_white_line>0x28 && Right_white_line<0x28)
	{
		set_color();
		do 
		{
			fixLeft();
			set_color();
			
		} while (Left_white_line>0x28);
	}
	
	else if(Right_white_line>0x28 && Left_white_line<0x28)
	{
		set_color();
		do
		{
			fixRight();
			set_color();
		}while(Right_white_line>0x28);
	}
	else
	{
		miss=0;
		velocity(100,100);
		set_color();
		if(missR>missL)
		{
			lcd_print(2,1,0,1);
			do 
			{
				set_color();
				right();
				miss++;
			}while(Center_white_line<40 && miss<20);
			stop();
			if(miss>20)
			{
				lcd_print(2,1,1,1);
				miss=0;
				do 
				{
					set_color();
					left();
				} while (Center_white_line<40);	
			}
			stop();
		}
		else
		{
			miss=0;
			lcd_print(2,1,2,1);
			do
			{
				set_color();
				left();
				miss++;
			}while(Center_white_line<40 && miss<20);
			
			stop();
			buzzer_on();
			_delay_ms(90);
			buzzer_off();
			
			if (miss>=20)
			{
				miss=0;
				lcd_print(2,1,3,1);
				//buzzer_on();
				//_delay_ms(1000);
				velocity(150,150);
				do
				{
					set_color();
					right();
				} while (Center_white_line<40 &&  miss<40);
				stop();
				if(miss>=40)
				{
					lcd_print(2,1,8,10);
				}
			}
			stop();
			lcd_print(2,1,9,1);
		}
	
	}

}
void goForward()
{
	set_color();
	do
	{
		set_color();
		forward();
		velocity(255,255);
		if(Left_white_line>0x28 && Right_white_line>0x28)
		{
			set_color();
			atNode();
		}
	}while(Center_white_line>0x28 && Left_white_line<0x28 && Right_white_line<0x28);
	
	set_color();
	fixPath();
	goForward();
}

int main()
{
	init_devices();
	lcd_set_4bit();
	lcd_init();
	
	set_color();
	goForward();
}