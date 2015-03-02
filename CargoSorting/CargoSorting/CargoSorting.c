/*
 * CargoSorting.c
 *
 * Created: 3/2/2015 7:37:28 AM
 *  Author: Ahmed
 */ 


#include "strong.c"

const int RED=0,GREEN=1,BLUE=2,BLACK=3,EMPTY=-1;
const int turn_v=150,line_v=240,correct_v=200;
int threshold;
int indicator[4];	//indicator[0] contains the terminal no. associated vid red color;this vil be used in scan();
unsigned char Left_white_line = 0;
unsigned char Center_white_line = 0;
unsigned char Right_white_line = 0;
unsigned char ct;
unsigned char adj;


void set_color()
{
	Left_white_line = ADC_Conversion(3);	//Getting data of Left WL Sensor
	Center_white_line = ADC_Conversion(2);	//Getting data of Center WL Sensor
	Right_white_line = ADC_Conversion(1);	//Getting data of Right WL Sensor
}


void turnLeft()	//turns the robo left
{
	
	forward_mm(60);
	left();
	while(ADC_Conversion(2)<40);
	stop();
}

char Delay(int tim)
{
	int i;
	for(i=0;i<tim && ADC_Conversion(2)<0x28;i++)
	{
		
		_delay_ms(1);
	}
	stop();
	if(i<tim)
		return 1;
	else return 0;
}

void semiCorrect_Indicator()
{
	
	
	if(Center_white_line>40 && (Left_white_line>40 || Right_white_line>40)) //2 bbw wbb
	{
		return;
	}
	if(Center_white_line<40)
	{
		if(Left_white_line>40 && Right_white_line<40) //bww
		{
			while((Center_white_line<0x28))
			{		
				left();
				set_color();
			}
		}
		else if(Right_white_line>40 && Left_white_line<40)	//wwb
		{
			while((Center_white_line<0x28))
			{
				right();
				set_color();
			}			
		}
	}
	
}
void correct()
{
	unsigned int i=50;
	stop();
	while(1)
	{
		left();
		if(Delay(i))
			return;
		stop();
	
		semiCorrect_Indicator();
		if(ADC_Conversion(2)>40)
			break;
	
		i+=50;
		right();
		if(Delay(i))
			return;
		stop();
		
		semiCorrect_Indicator();
		if(ADC_Conversion(2)>40)
			break;
		i+=50;
		
	}
	stop();
	return;
}
void initialCorrect_Indicator()
{
	int flag=0;
	
	velocity(correct_v,correct_v);
	if(Center_white_line<40)
	{
		if(Left_white_line>40 && Right_white_line<40) //bww
		{
			flag=1;
			while(!(Center_white_line>0x28 && Left_white_line<40 && Right_white_line<40))
			{
				left();
				set_color();
			}
		}
		else if(Right_white_line>40 && Left_white_line<40)	//wwb
		{
			flag=1;			while(!(Center_white_line>0x28 && Left_white_line<40 && Right_white_line<40))
			{
				right();
				set_color();
			}			
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
			return;
		}
		else if(Left_white_line<40 && Right_white_line>40)	//wbb
		{
			return;
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
	return;
}
void followLine_Indicator()
{
	do
	{
		forward();
		set_color();
		if(Center_white_line>40 && (Left_white_line>40 || Right_white_line>40)) //2 bbw wbb
		{
			return;
		}
		velocity(240,240);
		
		
	}while(Center_white_line>0x28 && Left_white_line<40 && Right_white_line<40);	//wbw
	
	initialCorrect_Indicator();
	followLine_Indicator();
	return;
}
/*************************END GULLA CODE*************/

int scan()//return the color no.
{
	red_read();
	blue_read();
	green_read();
	
	if(!(ADC_Conversion(11)>55 && ADC_Conversion(11)<100))
		return EMPTY;
	
	if (red<threshold && green < threshold && blue < threshold)
	{
		return BLACK;
	}
	else
	{
		if (red > blue)
		{
			if (red > green)
			return RED;
			else
			return GREEN;
		}
		else if (blue > green)
		return BLUE;
		else return GREEN;
	}

}

void setIndicator()
{
	unsigned char i;
	followLine_Indicator();
	
	//turn efficiency
	forward_mm(160);
	//scan 4
	soft_right_2();
	_delay_ms(700);
	while(ADC_Conversion(3)<40);
	stop();
	indicator[3]=scan();
	for(i=0;i<indicator[3];i++)
	{
		buzzer_on();
		_delay_ms(100);
		buzzer_off();
		_delay_ms(500);
	}
	
	//back to line
	soft_left();
	_delay_ms(1000);
	while(ADC_Conversion(2)<40);
	stop();
	
	//scan 3
	soft_left_2();
	_delay_ms(500);
	while(ADC_Conversion(2)<40);
	stop();
	indicator[2]=scan();
	for(i=0;i<indicator[2];i++)
	{
		buzzer_on();
		_delay_ms(100);
		buzzer_off();
		_delay_ms(500);
	}
	//back to line
	soft_right();
	_delay_ms(500);
	while(ADC_Conversion(2)<40);
	stop();
	
	//=====
	
	followLine_Indicator();
	
	//turn efficiency
	forward_mm(160);
	//scan 4
	soft_right_2();
	_delay_ms(500);
	while(ADC_Conversion(3)<40);
	stop();
	indicator[1]=scan();
	for(i=0;i<indicator[1];i++)
	{
		buzzer_on();
		_delay_ms(100);
		buzzer_off();
		_delay_ms(500);
	}
	
	//back to line
	soft_left();
	_delay_ms(1000);
	while(ADC_Conversion(2)<40);
	stop();
	
	//scan 3
	soft_left_2();
	_delay_ms(500);
	while(ADC_Conversion(2)<40);
	stop();
	indicator[0]=scan();
	for(i=0;i<indicator[0];i++)
	{
		buzzer_on();
		_delay_ms(100);
		buzzer_off();
		_delay_ms(500);
	}
	//back to line
	soft_right();
	_delay_ms(500);
	while(ADC_Conversion(2)<40);
	stop();
	
	
	followLine_Indicator();
	turnLeft();
	forward();
	_delay_ms(500);
	followLine_Indicator();
	turnLeft();
	followLine_Indicator();
}


void setIndicatorAndColor()
{
	int i;
	for (i = 0; i<4; i++)
	{
		color[i] = i;
		term[i][0] = term[i][1] = -2;
		visited[i] = 0;
	}
	for (i = 0; i<4; i++)
	indicator[color[i]] = i;
	sort[0] = sort[1] = -1;
	arm[0] = arm[1] = -1;
}

void __init__()
{
	ct = 2; adj = 0;
	init_devices();
	lcd_set_4bit();
	lcd_init();
	color_sensor_scaling();
	setIndicatorAndColor();
	servo_1(0);
	servo_2(90);
	servo_3(0);
	
}
int main(void)
{
	__init__();
	setIndicator();
    while(1)
    {
        //TODO:: Please write your application code 
    }
}