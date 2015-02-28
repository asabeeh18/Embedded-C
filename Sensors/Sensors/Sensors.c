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
		print_sensor(1,1,11);
	}
}/*
bbb node	
bwb impossible
wbb DO
bbw DO
bww DOne
wwb DOne
wbw OK!
www Death 
*/
void node()
{
	buzzer_on();
	_delay_ms(100);
	buzzer_off();
}
char Delay(int tim)
{
	int i;
	for(i=0;i<tim && ADC_Conversion(2)<0x28;i++)// && ADC_Conversion(1)<40 && ADC_Conversion(3)<40));i++)
	{
		//set_color();
		_delay_ms(1);
	}
	stop();
	if(i<tim)
		return 1;
	else return 0;
}

void semiCorrect()
{
	
	lcd_print(1,2,1,1);
	if(Center_white_line<40)
	{
		if(Left_white_line>40 && Right_white_line<40) //bww
		{
			lcd("bww");
			
			while((Center_white_line<0x28))// && Left_white_line<40 && Right_white_line<40))
			{
				//lcd_print(1,2,1,1);
				left();
				set_color();
			}
			lcd("-");
		}
		else if(Right_white_line>40 && Left_white_line<40)	//wwb
		{
			
			lcd("wwb");
			while((Center_white_line<0x28))// && Left_white_line<40 && Right_white_line<40))
			{
				//lcd_print(1,2,2,1);
				right();
				set_color();
			}
			lcd("-");
			
		}
	}
	lcd_print(1,2,0,1);
	
}
void correct()
{
	unsigned int d=2;
	unsigned int i=20;
	Degrees=5;
	lcd("cor");
	stop();
//	if(lf==1)
	{
		lf=0;
		while(1)
		{
			right();
			if(Delay(i))
					return;
			stop();
			lcd_print(1,1,1,1);
			//set_color();
			semiCorrect();
			if(ADC_Conversion(2)>40)// && ADC_Conversion(1)<40 && ADC_Conversion(3)<40)
				break;
			i+=20;
			left();
			if(Delay(i))
				return;
			stop();
			lcd_print(1,1,2,1);
			semiCorrect();
			if(ADC_Conversion(2)>40)// && ADC_Conversion(1)<40 && ADC_Conversion(3)<40)
				break;
			//d*=2;
			//set_color();
			//i+=2;
			i+=20;
		}
	}
/*	else
	{
		lf=1;
		while(1)
		{
			left();
			if(Delay(i+2))
			return;
			stop();
			lcd_print(1,1,3,1);
			//set_color();
			semiCorrect();
			if(ADC_Conversion(2)>40 && ADC_Conversion(1)<40 && ADC_Conversion(3)<40)
				break;
			
			right();
			if(Delay(i))
				return;
			stop();
			lcd_print(1,1,4,1);
			semiCorrect();
			if(ADC_Conversion(2)>40 && ADC_Conversion(1)<40 && ADC_Conversion(3)<40)
				break;
			
			//d*=2;
			//set_color();
			//i+=2;
			i+=20;
		}
	}
*/	lcd("-");
	stop();
	return;
}
void noNatak()
{
	int flag=0;
	//buzzer_on();
	//lcd_print(2,1,7,1);
	velocity(150,150);
	if(Center_white_line<40)
	{
		if(Left_white_line>40 && Right_white_line<40) //bww
		{
			lcd("bww");
			flag=1;
			while(!(Center_white_line>0x28 && Left_white_line<40 && Right_white_line<40))
			{
				left();
				set_color();
			}
			lcd("-");
		}
		else if(Right_white_line>40 && Left_white_line<40)	//wwb
		{
			flag=1;
			lcd("wwb");
			while(!(Center_white_line>0x28 && Left_white_line<40 && Right_white_line<40))
			{
				right();
				set_color();
			}
			lcd("-");
			
		}
		else
		{
			flag=1;
			correct();
		}
	}
	else
	{
		if(Left_white_line>40 && Right_white_line<40)	//bbw
		{
			flag=1;
			node();
			/*lcd("bbw");
			
			while(Center_white_line>40 && Left_white_line>40 && Right_white_line<40)
			{
				soft_right_2();
				set_color();
			}
			lcd("-");*/
		}
		else if(Left_white_line<40 && Right_white_line>40)	//wbb
		{
			flag=1;
			node();
			
			/*flag=1;
			lcd("wbb");
			while(Center_white_line>40 && Left_white_line<40 && Right_white_line>40)
			{
				soft_left_2();
				set_color();
			}
			lcd("-");*/
		}
		else
		{
			flag=1;
			correct();
		}
	}
	if(flag==0)	
		correct();
	
	stop();
	//lcd_print(2,1,6,1);
	//buzzer_off();
	return;
}
