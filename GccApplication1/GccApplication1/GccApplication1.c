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
const unsigned char mthresh=5;
void buzzer();

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
	buzzer();
}

void lcd(char *str)
{
	lcd_wr_command(0x01);
	lcd_cursor(2,3);
	lcd_string(str);
}
void fixLeft()
{
	missL++;
	set_color();
	//velocity(100,100);
	soft_left();
	set_color();
	if(Center_white_line>0x28)
		return;
}

void fixRight()
{
	missR++;
	set_color();
	//velocity(100,100);
	soft_right();
	set_color();
	if(Center_white_line>0x28)
		return;
}

void fixPath()
{
	velocity(100,100);
	set_color();
	if(Left_white_line>0x28 && Right_white_line>0x28)
	{
		lcd("fixPath.if.lcd");
		set_color();
		atNode();		
	}
	
	else if(Left_white_line>0x28 && Right_white_line<0x28)
	{
		lcd("fixLEft");
		set_color();
		while(1) 
		{
			fixLeft();
			set_color();
			if(Center_white_line>40 && Right_white_line<40 && Left_white_line<40)
				break;
		}
		stop();
		lcd("-");
	}
	
	else if(Right_white_line>0x28 && Left_white_line<0x28)
	{
		lcd("fixRight");
		set_color();
		while(1)
		{
			fixRight();
			set_color();
			if(Center_white_line>40 && Right_white_line<40 && Left_white_line<40)
				break;
		}
		stop();
		lcd("-");
	}
	
	else
	{
		lcd("xxDEATHxx");
		miss=0;
		velocity(150,150);
		set_color();
		if(missR>missL)
		{
			lcd_print(2,1,0,1);
			lcd("xxDEATH-right1");
			do 
			{
				
				set_color();
				right();
				miss++;
			}while(Center_white_line<40 && miss<mthresh);
			lcd("--");
			stop();
			if(miss>mthresh)
			{
				lcd_print(2,1,1,1);
				lcd("xxDEATH-left1");
				miss=0;
				do 
				{
					
					set_color();
					left();
				} while (Center_white_line<(2*mthresh));	
				lcd("--");
			}
			stop();
		}
		else
		{
			miss=0;
			lcd_print(2,1,2,1);
			lcd("xxDEATH-left2");
			do
			{
				
				set_color();
				left();
				miss++;
			}while(Center_white_line<40 && miss<mthresh);
			lcd("--");
			
			stop();
			buzzer();
			
			if (miss>=mthresh)
			{
				miss=0;
				lcd_print(2,1,3,1);
				//buzzer_on();
				//_delay_ms(1000);
				velocity(150,150);
				lcd("xxDEATH-right2");
				do
				{
					
					set_color();
					right();
				} while (Center_white_line<40 &&  miss<(2*mthresh));
				lcd("--");
				stop();
				if(miss>=30)
				{
					lcd_print(2,1,8,10);
				}
			}
			stop();
			lcd("--");
			lcd_print(2,1,9,1);
		}
		stop();
	
	}
	lcd("-");

}
void goForward()
{
	set_color();
	if(!(Center_white_line>0x28 && Left_white_line<0x28 && Right_white_line<0x28))
		fixPath();
	do
	{
		set_color();
		forward();
		velocity(240,240);
		if(Left_white_line>0x28 && Right_white_line>0x28)
		{
			set_color();
			atNode();
		}
	}while(Center_white_line>0x28 && Left_white_line<0x28 && Right_white_line<0x28);
	
	set_color();
	stop();
	fixPath();
	goForward();
}
void buzzer()
{
	
	buzzer_on();
	_delay_ms(90);
	buzzer_off();
}
int main()
{
	init_devices();
	lcd_set_4bit();
	lcd_init();
	
	set_color();
	goForward();
}