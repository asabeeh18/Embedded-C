/*
 * DeathDoor.c
 *
 * Created: 3/3/2015 12:25:20 PM
 *  Author: Ahmed
 */ 


#include "strong.c"


unsigned char Left_white_line = 0;
unsigned char Center_white_line = 0;
unsigned char Right_white_line = 0;
int correct_v=200,turn_v=150;
char f=0;
void turnRight()
{
	node();
	right();
	_delay_ms(700);
	while(ADC_Conversion(2)<=40);
	stop();
}
void turnLeft()
{
	node();
	left();
	_delay_ms(700);
	while(ADC_Conversion(2)<=40);
	stop();
}
void Uturn()
{
	left();
	left();
}
void blankUturn()
{
	left_degrees(180);
}


//**********BlackLine Follower **************

void set_color()
{
	Left_white_line = ADC_Conversion(3);	//Getting data of Left WL Sensor
	Center_white_line = ADC_Conversion(2);	//Getting data of Center WL Sensor
	Right_white_line = ADC_Conversion(1);	//Getting data of Right WL Sensor
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
//*******************INDICATOR********************
void semiCorrect_Indi()
{
	
	//lcd_print(1,2,1,1);
	if(Center_white_line>40 && (Left_white_line>40 || Right_white_line>40)) //2 bbw wbb
	{
		return;
	}
	if(Center_white_line<40)
	{
		if(Left_white_line>40 && Right_white_line<40) //bww
		{
			//		lcd("bww");
			
			while((Center_white_line<0x28))// && Left_white_line<40 && Right_white_line<40))
			{
				//lcd_print(1,2,1,1);
				left();
				set_color();
			}
			//	lcd("-");
		}
		else if(Right_white_line>40 && Left_white_line<40)	//wwb
		{
			
			//lcd("wwb");
			while((Center_white_line<0x28))// && Left_white_line<40 && Right_white_line<40))
			{
				//lcd_print(1,2,2,1);
				right();
				set_color();
			}
			//lcd("-");
			
		}
	}
	//lcd_print(1,2,0,1);
	
}
void correct_Indi()
{
	unsigned int i=50;
	Degrees=5;
	stop();
	//lcd("cor");
	
	/*if(lf==1)
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
			i+=50;
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
			i+=50;
		}
	}*/
	//else
	
		
		while(1)
		{
			left();
			if(Delay(i))
				return;
			stop();
			lcd_print(1,1,1,1);
			//set_color();
			semiCorrect_Indi();
			if(ADC_Conversion(2)>40)// && ADC_Conversion(1)<40 && ADC_Conversion(3)<40)
				break;
			i+=50;
			right();
			if(Delay(i))
				return;
			stop();
			lcd_print(1,1,2,1);
			semiCorrect_Indi();
			if(ADC_Conversion(2)>40)// && ADC_Conversion(1)<40 && ADC_Conversion(3)<40)
				break;
			//d*=2;
			//set_color();
			//i+=2;
			i+=50;
		
	}
	//lcd("-");
	stop();
	return;
}
void noNatak_Indi()
{
	int flag=0;
	//buzzer_on();
	//lcd_print(2,1,7,1);
	//lcd("no");
	velocity(correct_v,correct_v);
	if(Center_white_line<40)
	{
		if(Left_white_line>40 && Right_white_line<40) //bww
		{
		//	lcd("bww");
			flag=1;
			while(!(Center_white_line>0x28 && Left_white_line<40 && Right_white_line<40))
			{
				left();
				set_color();
			}
		//	lcd("-");
		}
		else if(Right_white_line>40 && Left_white_line<40)	//wwb
		{
			flag=1;
			//lcd("wwb");
			while(!(Center_white_line>0x28 && Left_white_line<40 && Right_white_line<40))
			{
				right();
				set_color();
			}
			//lcd("-");
			
		}
		else
		{
			flag=1;
			correct_Indi();
		}
	}
	else
	{
		if(Left_white_line>40 && Right_white_line<40)	//bbw
		{
			flag=1;
			return;
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
			return;
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
			correct_Indi();
		}
	}
	if(flag==0)	
		correct_Indi();
	stop();
	//lcd("--");
	//lcd_print(2,1,6,1);
	//buzzer_off();
	return;
}

void forwardJaa_Indi()
{
	do
	{
		forward();
		//_delay_ms(200);
		set_color();
		if(Center_white_line>40 && (Left_white_line>40 || Right_white_line>40)) //2 bbw wbb
		{
			return;
		}
		velocity(240,240);
		//velocity(v+vi,v+vi);
		//i+=20;
	}while(Center_white_line>0x28 && Left_white_line<40 && Right_white_line<40);	//wbw
	
	noNatak_Indi();
	forwardJaa_Indi();
	return;
}
//***********END INDICATOR****************
void node()
{
	//buzzer();
	//lcd_print(1,1,1,1);
	velocity(turn_v,turn_v);
	forward_mm(70);
	stop();
}
//**************SORT AREA*******************



char semiCorrect()
{
	
	//lcd_print(1,2,1,1);
	if(Center_white_line>40 && (Left_white_line>40 || Right_white_line>40)) //2 bbw wbb
	{
		node();
		return 1;
	}
	if(Center_white_line<40)
	{
		if(Left_white_line>40 && Right_white_line<40) //bww
		{
	//		lcd("bww");
			
			while((Center_white_line<0x28))// && Left_white_line<40 && Right_white_line<40))
			{
				//lcd_print(1,2,1,1);
				left();
				set_color();
			}
		//	lcd("-");
		}
		else if(Right_white_line>40 && Left_white_line<40)	//wwb
		{
			
			//lcd("wwb");
			while((Center_white_line<0x28))// && Left_white_line<40 && Right_white_line<40))
			{
				//lcd_print(1,2,2,1);
				right();
				set_color();
			}
			//lcd("-");
			
		}
	}
	return 0;
	//lcd_print(1,2,0,1);
	
}
char correct()
{
	unsigned int d=2;
	unsigned int i=50;
	Degrees=5;
	stop();
	//lcd("cor");
	
	/*if(lf==1)
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
			i+=50;
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
			i+=50;
		}
	}*/
	//else
	
		
		while(1)
		{
			left();
			if(Delay(i))
				return 0;
			stop();
			
			//set_color();
			if(semiCorrect())
				return 1;
			if(ADC_Conversion(2)>40)// && ADC_Conversion(1)<40 && ADC_Conversion(3)<40)
				break;
			i+=50;
			right();
			if(Delay(i))
				return 0;
			stop();
			
			if(semiCorrect())
				return 1;
			if(ADC_Conversion(2)>40)// && ADC_Conversion(1)<40 && ADC_Conversion(3)<40)
				break;
			//d*=2;
			//set_color();
			//i+=2;
			i+=50;
		
	}
	//lcd("-");
	stop();
	return 0;
}
int noNatak()
{
	int flag=0;
	//buzzer_on();
	//lcd_print(2,1,7,1);
	//lcd("no");
	velocity(correct_v,correct_v);
	if(Center_white_line<40)
	{
		if(Left_white_line>40 && Right_white_line<40) //bww
		{
		//	lcd("bww");
			flag=1;
			while(!(Center_white_line>0x28 && Left_white_line<40 && Right_white_line<40))
			{
				left();
				set_color();
			}
		//	lcd("-");
		}
		else if(Right_white_line>40 && Left_white_line<40)	//wwb
		{
			flag=1;
			//lcd("wwb");
			while(!(Center_white_line>0x28 && Left_white_line<40 && Right_white_line<40))
			{
				right();
				set_color();
			}
			//lcd("-");
			
		}
		else
		{
			flag=1;
			if(correct())
				return 1;
		}
	}
	else
	{
		if(Left_white_line>40 && Right_white_line<40)	//bbw
		{
			flag=1;
			node();
			return 1;
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
			return 1;
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
			if(correct())
			return 1;
		}
	}
	if(flag==0)	
		if(correct())
		return 1;
	stop();
	//lcd("--");
	//lcd_print(2,1,6,1);
	//buzzer_off();
	return 0;
}
void forwardJaa()
{
	//buzzer();
	unsigned int vi=0;
	do
	{
		forward();
		//_delay_ms(200);
		set_color();
		if(Center_white_line>40 && (Left_white_line>40 || Right_white_line>40)) //2 bbw wbb
		{
			node();
			return;
		}
		velocity(240,240);
		//velocity(v+vi,v+vi);
		//i+=20;
	}while(Center_white_line>0x28 && Left_white_line<40 && Right_white_line<40);	//wbw
	
	if(noNatak())
		return;
	forwardJaa();
	return;
}
//**************END SORT AREA*******************

//**************END BlackLine Follower*******************


void lower(unsigned char side)
{
	// 	if(armCount==0)
	// 	{
	// 		if(side==1)
	// 			servo_1_free();
	// 		else
	// 			servo_3_free();_delay_ms(500);
	// 	}
	if(side==1)
	{
		servo_2(55);
	}
	else
	servo_2(120);
	_delay_ms(500);
}
void elevate()
{
	servo_2(90);
	_delay_ms(500);
	servo_2_free();_delay_ms(500);
}
void open(unsigned char side)
{
	if (side == 0)
	{
		servo_3(90);
		_delay_ms(500);
		servo_3_free();_delay_ms(500);
	}
	else
	{
		servo_1(0);
		_delay_ms(500);
		servo_1_free();_delay_ms(500);
	}
	//_delay_ms(500);
}
void close(unsigned char side)
{
	if (side == 0)
	servo_3(0);
	else if (side == 1)
	servo_1(80);
	_delay_ms(500);
}

void pick(int side)
{
	open(side);
	lower(side);
	//_delay_ms(1000);
	close(side);
	//_delay_ms(3000);
	elevate();//mid
	///_delay_ms(1000);
	//armCount--;
	_delay_ms(1000);
	// 	if(armCount==0)
	// 	{
	// 		servo_1_free();
	// 		servo_3_free();
	// 	}
}

void drop(int side)
{
	lower(side);//lower
	//_delay_ms(1000);
	open(side);
	//_delay_ms(1000);
	elevate();//mid
	//_delay_ms(1000);

	// 	if(side==1)
	// 		servo_1_free();
	// 	else
	// 		servo_3_free();
	// 	lower(side);
	// 	if(armCount==0)
	// 	servo_2_free();
	// 	open(side);
	// 	elevate();
	//armCount++;
}



void indicator_set()
{
	forwardJaa_Indi();
	//buzzer();
	//turn efficiency
	forward_mm(160);
	//buzzer();
	//scan 4
	soft_right_2();
	_delay_ms(700);
	while(ADC_Conversion(3)<40);
	stop();
	//color[3]=scan();
	// 	for(i=0;i<color[3];i++)
	// 	{
	// 		buzzer_on();
	// 		_delay_ms(100);
	// 		buzzer_off();
	// 		_delay_ms(500);
	// 	}
	
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
	//color[2]=scan();
	// 	for(i=0;i<color[2];i++)
	// 	{
	// 		buzzer_on();
	// 		_delay_ms(100);
	// 		buzzer_off();
	// 		_delay_ms(500);
	// 	}
	//back to line
	soft_right();
	_delay_ms(500);
	while(ADC_Conversion(2)<40);
	stop();
	
	//=====
	
	forwardJaa_Indi();
	
	//turn efficiency
	forward_mm(160);
	//scan 4
	soft_right_2();
	_delay_ms(700);
	while(ADC_Conversion(3)<40);
	stop();
	//color[1]=scan();
	// 	for(i=0;i<color[1];i++)
	// 	{
	// 		buzzer_on();
	// 		_delay_ms(100);
	// 		buzzer_off();
	// 		_delay_ms(500);
	// 	}
	
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
	//color[0]=scan();
	// 	for(i=0;i<color[0];i++)
	// 	{
	// 		buzzer_on();
	// 		_delay_ms(100);
	// 		buzzer_off();
	// 		_delay_ms(500);
	// 	}
	//back to line
	soft_right();
	_delay_ms(500);
	while(ADC_Conversion(2)<40);
	stop();
	
	//lcd_print(1,1,color[0],1);
	//lcd_print(1,3,color[1],1);
	//lcd_print(1,5,color[2],1);
	//lcd_print(1,7,color[3],1);
	
	forwardJaa();
	turnRight();
	forward();
	_delay_ms(500);
	forwardJaa();
	turnRight();
	forwardJaa();
	//buzzer_on();
}

void newSenseSequence()
{
	//node 1
	velocity(turn_v,turn_v);
	forward_mm(60);
	while (ADC_Conversion(2)<50)	//earlier 3
		right();
	//_delay_ms(140);
	stop();
	
	//node 2
	left_degrees(150);
	velocity(turn_v, turn_v);
	while (ADC_Conversion(2)<50)
		left();
	//_delay_ms(50);
	stop();
	if(f=1)
	{
		buzzer_on();
		_delay_ms(500);
		buzzer_off();
	}
	
	//on line
	left();
	while(ADC_Conversion(2)<=40);
	stop();
}













void __init__()
{
	init_devices();
	lcd_set_4bit();
	lcd_init();
	color_sensor_scaling();
	
	//threshold=6000;
	servo_1(80);
	servo_2(90);
	servo_3(0);
	//servo_1(0);
	//	servo_3(0);
	_delay_ms(200);
	servo_3_free();_delay_ms(500);
	servo_2_free();_delay_ms(500);
	servo_1_free();_delay_ms(500);
	
	//buzzer();
}
int main(void)
{
    __init__();
	forward();
	_delay_ms(1000);
	indicator_set();
	
	
	//reached Terminal
	
	newSenseSequence();
	
	
	
	back_mm(60);
	pick(0);
	pick(1);
	
	forwardJaa();
	turnLeft();
	forwardJaa();
	
	//at sort
	turnLeft();
	back_mm(60);
	drop(0);
	Uturn();
	
	//at term 1
	f=1;
	forwardJaa();
	newSenseSequence();
	f=0;
	drop(1);
	
	forwardJaa();
	pick(0);
	
	forwardJaa();
	
	//At Terminal 3
	newSenseSequence();
	back_mm(60);
	pick(1);
	blankUturn();
	//back maybe
	drop(0);
	Uturn();
	//may be back_mm()
	pick(0);
	
	forwardJaa();
	turnRight();
	forwardJaa();
	forwardJaa();
	
	//at Sort
	turnRight();
	forwardJaa();
	
	//At term 4
	
	forward_mm(60);
	drop(0);
	drop(1);
	Uturn();
	forwardJaa();
	forwardJaa();
	
	//at term 2
	f=1;
	newSenseSequence();
	f=1;
	back_mm(60);
	pick(1);
	
	forwardJaa();
	turnRight();
	forwardJaa();
	forwardJaa();
	turnLeft();
	
	forwardJaa();
	forward_mm(60);
	drop(1);
	buzzer_on();
	while(1);
	return 0;
}