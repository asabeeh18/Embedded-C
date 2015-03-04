/*
 * CargoSorting.c
 *
 * Created: 3/2/2015 7:37:28 AM
 *  Author: Ahmed
 */ 

/*--Variables--*/
#define F_CPU 14745600
#include "strong.c"
int indicator[4];//indicator[0] contains the terminal no. associated vid red color;this vil be used in scan();
int color[4];//color[0] contains the color no. associated vid terminal 0;
int term[4][2];//initialise to -2;
int visited[4];//initialise to 0; when terminal visted make it 1;
int sort[2];//initialise to -1;
int ct, adj, nt;
int armCount = 2; //indicates no. of free arms
int sortCount = 2;//indicated no. of free sort;
int arm[2];//initialise to -1;
int total = 8;
int sorted = 0;
int count = 0;
int cost;
int flag=0,flag1=0,angle;
int visitedCount = 0;
int ot = 3, dir = 0;
int i=0;
int sf=90;
unsigned char cur_angle=90;
const int RED=0,GREEN=1,BLUE=2,BLACK=3,EMPTY=-1;
const int turn_v=150,line_v=240,correct_v=200;
int ff=1;

int threshold;
unsigned char Left_white_line = 0;
unsigned char Center_white_line = 0;
unsigned char Right_white_line = 0;
unsigned char lf=0;


//***UTILITY

void buzzer()
{
	buzzer_on();
	_delay_ms(100);
	buzzer_off();
}

//******************COLOR****************************

void set_color()
{
	Left_white_line = ADC_Conversion(3);	//Getting data of Left WL Sensor
	Center_white_line = ADC_Conversion(2);	//Getting data of Center WL Sensor
	Right_white_line = ADC_Conversion(1);	//Getting data of Right WL Sensor
}
int scan()//return the color no.
{
	red_read();
	blue_read();
	green_read();
	//_delay_ms(100);
	if(!(ADC_Conversion(11)>55 && ADC_Conversion(11)<100))
	{
		//buzzer();
		return EMPTY;
	}
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


//******************END COLOR****************************

//******************SERVO****************************

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
//*******************END SERVO*********************


//**************TURNS*******************

void node()
{
	//buzzer();
	//lcd_print(1,1,1,1);
	velocity(turn_v,turn_v);
	forward_mm(70);
	stop();
}
void turnRight()	//turns the robo right
{
	if ((dir == 3 && (ot == 0 || ot == 1)) || (dir == 1 && (ot == 2 || ot == 3)))
	{
		velocity(turn_v, turn_v);
		right_degrees(90);
	}
	else
	{
		//forward_mm(30);
		velocity(turn_v, turn_v);
		right_degrees(30);
		
		while (ADC_Conversion(2)<50)
			right();
		stop();	
	}
	//lcd("Right turn");
	//_delay_ms(2000);
	dir = (dir + 1) % 4;
	//printf("Turn Right \n");
	angle += 90;
	//buzzer();
}
void turnLeft()	//turns the robo left
{
	
	if ((dir == 1 && (ot == 0 || ot == 1)) || (dir == 3 && (ot == 2 || ot == 3)))
	{
		velocity(turn_v, turn_v);
		left_degrees(90);
	}
	else
	{
		//forward_mm(30);
		left_degrees(30);
		velocity(turn_v,turn_v);
		while (ADC_Conversion(2)<50)
			left();
		//_delay_ms(200);
		stop();
	}
	
	//lcd("Left turn");
	//_delay_ms(2000);
	dir = (dir + 3) % 4;
	//printf("Turn Left\n");
	angle += 90;
//	buzzer();
}
void turn()	//turn robo by 180 degree
{
	/*
	if ((dir == 0 && (ot == 2 || ot == 3))||(dir == 2 && (ot == 0 || ot == 1)))
	{
		velocity(turn_v, turn_v);
		left_degrees(180);
	}
	else if 
	{
		velocity(turn_v, turn_v);
		left_degrees(180);
	}
	*/
	if (dir == 0 && (ot == 2 || ot == 3))
	{
		velocity(turn_v, turn_v);
		left_degrees(180);
	}
	else if (dir == 2 && (ot == 0 || ot == 1))
	{
		//forward_mm(30);
		velocity(turn_v, turn_v);
		left_degrees(180);
	}
	else
	{
		velocity(turn_v, turn_v);
		left_degrees(180);
		while (ADC_Conversion(2)<60)
			left();
		stop();
	}
	//lcd("turn");
	//_delay_ms(2000);
	dir = (dir + 2) % 4;
	//printf("Turn\n");
	angle += 180;
}

//**********END TURNS****************

//**********BlackLine Follower **************

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
	
		lf=1;
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
	
		lf=1;
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
void front()
{
	forwardJaa();
	cost++;
	//printf("Front\n");
}
//**************END SORT AREA*******************

//**************END BlackLine Follower*******************






//**************SORT LOGIC*******************


void traverseToSort(int a, int b)
{
	//buzzer();
	if (flag == 1)
		flag = 0;
	if (a == 4 || a == 5)
	{
		//buzzer();
		if ((a == 4 && dir == 0) || (a == 5 && dir == 2))
			turnRight();
		else turnLeft();
		front();
		front();
		if (a == 4)
			ot = 5;
		else ot = 4;
		cost = cost + 2;
	}
	else{
		if ((dir == 1 && (ot == 0 || ot == 1)) || (dir == 3 && (ot == 2 || ot == 3)))
		{
			turnRight();
		}
		else if ((dir == 3 && (ot == 0 || ot == 1)) || (dir == 1 && (ot == 2 || ot == 3)))
		{
			turnLeft();
		}
		if ((a <= 1 && dir == 0) || (a >= 2 && dir == 2))
			turn();
		front();
		ot = ct % 2 + 4;
	}
}

int adjCount(int ct)
{
	return (ct + 2) % 4;
}
void terminalCheck1()
{
	//????
	//lcd_wr_command(0x01);
	//lcd_print(1,1,888,3);
	velocity(turn_v,turn_v);
	forward_mm(10);
	flag = 1;
	if (ct != ot)
	{
		if (dir == 1 || dir == 3)
		{
			if (ot == 3 || ot == 0)
				turnRight();
			else turnLeft();
		}
		if (((ct == 0 || ct == 1) && dir == 2) || ((ct == 2 || ct == 3) && dir == 0))
			turn();
		front();
		ot = ct;
	}
	//????
	right_degrees(30);
	velocity(turn_v, turn_v);
	
	dir=(dir+1)%4;
	//lcd_print(1,1,dir,1);
	while (ADC_Conversion(2)<50)	//earlier 3
		right();
	//_delay_ms(140);
	stop();
	//lcd_print(1,1,9,1);
	
	term[ct][0] = scan();
	for(i=0;i<=term[ct][0];i++)
	{
		buzzer_on();
		_delay_ms(100);
		buzzer_off();
		_delay_ms(500);
	}

// 	if(term[ct][0]==color[ct])
// 	{
// 		buzzer_on();
// 		_delay_ms(500);
// 		buzzer_off();
// 	}
// 	if(term[ct][0]==-1)
// 		buzzer();

	_delay_ms(1000);
	//printf("Enter term[%d][%d]\n", ct, 0);
	//scanf("%d", &term[ct][0]);

	if (term[ct][0] == -1 || term[ct][0] == color[ct])
		total--;
}
void terminalCheck2()
{
	velocity(turn_v,turn_v);
	/*if (flag == 0)
	{
		if (dir == 0)
			if (ot == 0 || ot == 1)
				forward_mm(30);
			else back_mm(30);
		else if (ot == 0 || ot == 1)
			back_mm(30);
		else forward_mm(30);
		flag = 1;
	}*/

	if (((ct == 0 || ct == 1) && dir == 0) || ((ct == 2 || ct == 3) && dir == 2))
	{
		left_degrees(30);
		velocity(turn_v, turn_v);
		while (ADC_Conversion(3)<50)
		left();
		//_delay_ms(100);
		stop();
		dir=(dir+3)%4;
	}
	else if (((ct == 0 || ct == 1) && dir == 2) || ((ct == 2 || ct == 3) && dir == 0))
		{
			right_degrees(30);
			velocity(turn_v, turn_v);
			while (ADC_Conversion(3)<50)
				right();
			//_delay_ms(100);
			stop();	
			dir=(dir+1)%4;
		}
	else {
		left_degrees(150);
		velocity(turn_v, turn_v);
		while (ADC_Conversion(3)<50)
		left();
		_delay_ms(100);
		stop();
		dir=(dir+2)%4;
		//lcd((char *)dir);
		}
	_delay_ms(400);
	//printf("Enter term[%d][%d]\n", ct, 1);
	//scanf("%d", &term[ct][1]);
// 	left();
// 	_delay_ms(100);
	stop();
	term[ct][1] = scan();
	for(i=0;i<=term[ct][1];i++)
	{
		buzzer_on();
		_delay_ms(100);
		buzzer_off();
		_delay_ms(500);
	}
// 	if(term[ct][1]==color[ct])
// 	{
// 		buzzer_on();
// 		_delay_ms(500);
// 		buzzer_off();
// 	}
// 	if(term[ct][1]==-1)
// 		buzzer();
// 	
	//_delay_ms(1000);
	if (term[ct][1] == -1 || term[ct][1] == color[ct])
		total--;
	visited[ct] = 1;
	visitedCount++;
	//lcd_print(2,15,dir,1);
	//_delay_ms(1000);
// 	if(ff=1)
// 	{
// 		
// 	left();
// 	_delay_ms(1000);
// 	stop();
// 	}
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
	armCount--;
	_delay_ms(1000);
// 	if(armCount==0)
// 	{
// 		servo_1_free();
// 		servo_3_free();
// 	}
}

void position(int armNo, int side)
{
	int a;
	if (ct == ot)
	{
		if (((ct == 0 || ct == 1) && dir == 0) || ((ct == 2 || ct == 3) && dir == 2))
		{
			if (armNo != side)
				turn();
		}
		else	if (((ct == 0 || ct == 1) && dir == 2) || ((ct == 2 || ct == 3) && dir == 0))
		{
			if (armNo == side)
				turn();
		}
		else if (((ct == 0 || ct == 1) && dir == 1) || ((ct == 2 || ct == 3) && dir == 3))
		{
			if (armNo != side)
			turnRight();
			else turnLeft();
		}
		else	if (((ct == 0 || ct == 1) && dir == 3) || ((ct == 2 || ct == 3) && dir == 1))
					if (armNo == side)
						turnRight();
					else turnLeft();
		if(flag==1 && (dir==0 || dir==2))
		{
			back_mm(50);
			flag=0;
		}	
	}
	else
	{
		if (dir == 1 || dir == 3)
			if (ct == 0 || ct == 3)
				turnRight();
			else turnLeft();
		if (dir == 0 || dir == 2)
		{
			if ((dir == 0 && (ct == 0 || ct == 1)) || (dir == 2 && (ct == 2 || ct == 3)))
			{
				if (ot != 4 && ot != 5)
				{
					turnLeft();
					front();
				}
				if (armNo == side)
				{
					front();
				}
				else
				{
					front();
					turn();
				}
			}
			else
			{
				if (ot != 4 && ot != 5)
				{
					turnLeft();
					front();
				}
				if (armNo == side)
				{
					turn();
					front();
				}
				else
				{
					turn();
					front();
					turn();
				}
			}
		}
		else
		{
			if (armNo == side)
			{
				if (ct == 0 || ct == 3)
				turnRight();
				else turnLeft();
				front();
			}
			else
			{
				if (ct == 0 || ct == 3)
				turnRight();
				else turnLeft();
				front();
			}
		}
		ot = ct;
	}
}

void pickNode(int armNo, int side)
{
	arm[armNo] = term[ct][side];
	position(armNo, side);
	pick(armNo);
	term[ct][side] = -1;
	//printf("Arm %d picked %d from term[%d][%d]\n", armNo, arm[armNo], ct, side);
}

void pickSort(int armNo, int sortNo)
{
	arm[armNo] = sort[sortNo];
	if (((armNo == sortNo) && dir == 0) || ((armNo != sortNo) && dir == 2))
		turn();
	else
	{
		if (dir == 1 || dir == 3)
			if (armNo == 0)
				turnLeft();
			else turnRight();
	}
	back_mm(30);
	pick(armNo);
	forward_mm(30);
	sort[sortNo] = -1;
	//printf("Arm %d picked %d from sort[%d]\n", armNo, arm[armNo], sortNo);
}
void pickup()
{
	int arm0 = 0, arm1 = 1;
	if (((ct == 0 || ct == 1) && dir == 2) || ((ct == 2 || ct == 3) && dir == 0))
	{
		arm0 = 1;
		arm1 = 0;
	}
	if (visited[ct] == 0)
		if (term[ct][0] == -2)
		{
			terminalCheck1();
			terminalCheck2();
		}
		else	terminalCheck2();
		if ((term[ct][0] != color[ct] && term[ct][0] != -1) || (term[ct][1] != color[ct] && term[ct][1] != -1))
		{
			if (armCount == 2)
			{
				if (term[ct][0] == color[adj] || term[ct][1] == color[adj])
					if (term[ct][0] == color[adj])
						pickNode(arm0, 1);
					else	pickNode(arm1, 0);
				else	if ((term[ct][1] != color[ct]) && term[ct][1] != -1)
				{
					if (term[ct][0] != color[ct] && term[ct][0] != -1)
					{
						pickNode(arm1, 0);
						pickNode(arm0, 1);
					}
					else if (sort[ct % 2] == -1 || ((term[adj][0] == color[adj] || term[adj][0] == -1) || (term[adj][1] == color[adj] || term[adj][1] == -1)) || (term[ct][0] == color[ct] || term[ct][1] == color[ct]))
						pickNode(arm0, 1);
				}
				else	if (term[ct][0] != color[ct] && term[ct][0] != -1)
				{
					if (sort[ct % 2] == -1 || ((term[adj][0] == color[adj] || term[adj][0] == -1) || (term[adj][1] == color[adj] || term[adj][1] == -1)))
						pickNode(arm1, 0);
				}

			}
			if (armCount == 1)
			{
				int a1, a2;
				if (arm[arm0] == -1)
					a1 = arm0;
				else a1 = arm1;
				a2 = (a1 + 1) % 2;

				if (sort[ct % 2] == -1)
				{
					if (term[ct][a1] != -1 && term[ct][a1] != color[ct])
						pickNode(a1, a1);
					else if (term[ct][a2] != -1 && term[ct][a2] != color[ct])
						pickNode(a1, a2);
				}
				else
				{
					if (term[ct][0] == color[adj] || term[ct][1] == color[adj])
					{
						if ((visited[adj] == 1 && (term[adj][0] == -1 || term[adj][1] == -1)))
							if (term[ct][a1] == color[adj])
								pickNode(a1, a1);
							else pickNode(a1, a2);
					}
					else {
						if (sort[ct % 2] != color[ct] && sort[ct % 2] != color[adj])
							if ((visited[adj] == 1 && ((term[adj][0] != color[ct]) && (term[adj][1] != color[ct]))))
								if (term[ct][a1] != -1 && term[ct][a1] != color[ct])
									pickNode(a1, a1);
								else	if (term[ct][a2] != -1 && term[ct][a2] != color[ct])
									pickNode(a1, a2);
					}
				}
			}
		}
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
 	armCount++;
}
void nodeDrop(int armNo, int side)
{
	term[ct][side] = arm[armNo];
	position(armNo, side);
	drop(armNo);
	arm[armNo] = -1;
	//printf("Arm %d dropped %d on term[%d][%d]\n", armNo, term[ct][side], ct, side);
	sorted++;
}
void sortDrop(int armNo, int sortNo)
{
	sort[sortNo] = arm[armNo];
	if (((armNo == sortNo) && dir == 0) || ((armNo != sortNo) && dir == 2))
		turn();
	else
	{
		if (dir == 1 || dir == 3)
			if (armNo == 0)
				turnLeft();
			else turnRight();
	}
	drop(armNo);
	arm[armNo] = -1;
	//printf("Arm %d dropped %d on sort[%d]\n", armNo, sort[sortNo], sortNo);
}

void dropPickup(int a1)
{
	int a2 = (a1 + 1) % 2;
	if ((term[ct][a2] == color[ct] || term[ct][a2] != color[adj]) && (term[ct][a1] != color[ct] && term[ct][a1] != -1))
		pickNode(a2, a1);
	else		if (term[ct][a2] != color[ct] && term[ct][a2] != -1)
		pickNode(a2, a2);
}

void canDrop()
{
	int arm0 = 0, arm1 = 1;
	if (((ct == 0 || ct == 1) && dir == 2) || ((ct == 2 || ct == 3) && dir == 0))
	{
		arm0 = 1;
		arm1 = 0;
	}

	if (armCount == 2 || (arm[0] != color[ct] && arm[1] != color[ct]))
		return;
	else
	{
		if (visited[ct] == 0)
		{
			terminalCheck1();
			terminalCheck2();
		}
		if (armCount == 0)
		{
			if (arm[arm0] == color[ct])
				if (term[ct][0] == -1)
					nodeDrop(arm0, 0);
				else nodeDrop(arm0, 1);
			else if (arm[arm1] == color[ct])
				if (term[ct][0] != -1)
					nodeDrop(arm1, 1);
				else nodeDrop(arm1, 0);
		}
		if (arm[0] == color[ct] || arm[1] == color[ct])
		{
			if (term[ct][0] != -1 && term[ct][1] != -1)
			{
				if (arm[arm0] == color[ct])
					dropPickup(arm0);
				else dropPickup(arm1);
			}

			if (arm[arm0] == color[ct] && term[ct][0] == -1)
				nodeDrop(arm0, 0);
			else if (arm[arm0] == color[ct] && term[ct][1] == -1)
				nodeDrop(arm0, 1);
			else if (arm[arm1] == color[ct] && term[ct][0] == -1)
				nodeDrop(arm1, 0);
			else if (arm[arm1] == color[ct] && term[ct][1] == -1)
				nodeDrop(arm1, 1);

		}
	}
}

void newSort()
{
	int t1, t2, a2 = -2, a1 = -2;
	if (arm[1] != -1)
	a2 = indicator[arm[1]];
	if (arm[0] != -1)
	a1 = indicator[arm[0]];

	t1 = (ct + 1) % 2;
	t2 = t1 + 2;
	if (arm[1] != -1 || arm[0] != -1)
	{
		ct = a2;
		if (arm[1] == arm[0] && (visited[a2] == 0 || (term[a2][0] != -1 && term[a2][1] != -1)))
		{
			if (a2 == 0 || a2 == 3)
			sortDrop(0, t1);
		}
		else if (arm[1] != -1 && (term[a2][0] == -1 || term[a2][1] == -1))
		{
			if (arm[0] == -1)
			{
				if (sort[t1] == color[t1] || sort[t1] == color[t2])
				pickSort(0, t1);
			}
		}
		else if (arm[0] != -1 && (term[a1][0] == -1 || term[a1][1] == -1))
		{
			ct = a1;
			if (term[a1][0] == -1 || term[a1][1] == -1)
			{
				if (arm[1] == -1)
				{
					if (sort[t1] == color[t1] && sort[t1] == color[t2])
					pickSort(0, t1);
				}
			}
			else	if (sort[t1] == -1 && arm[1] != -1)
			sortDrop(0, t1);
		}
		else	if (arm[1] != -1 && (sort[t1] == -1 && arm[0] != -1))
		sortDrop(0, t1);
		else if (arm[1] != -1)
		ct = arm[1];
		else ct = arm[0];
	}
	else
	{
		if ((sort[t1] == color[t1] || sort[t1] == color[t2]) && sort[t1] != -1)
		ct = indicator[sort[t1]];
		else if (sort[t1] != -1)
		{
			pickSort(1, t1);
			ct = (t1 + 1) % 2 + 4;
		}
		else if (visited[t1] == 1 && (term[t1][0] == color[t1] || term[t1][0] == -1) && (term[t1][1] == color[t1] || term[t1][1] == -1))
		ct = t2;
		else ct = t1;

	}
	adj = adjCount(ct);
}
void sortCheck()
{
	//forward_mm(70);
	int a1, a2;
	int arm0 = 0, arm1 = 1;
	if (((ct == 0 || ct == 1) && dir == 2) || ((ct == 2 || ct == 3) && dir == 0))
	{
		arm0 = 1;
		arm1 = 0;
	}
	if ((sort[ct % 2] == color[ct] || sort[ct % 2] == color[adj]))
	{
		if (armCount>0)
		{
			ct = indicator[sort[ct % 2]];
			if (arm[arm0] == -1)
				pickSort(arm0, ct % 2);
			else if (arm[arm1] == -1)
				pickSort(arm1, ct % 2);
			if (armCount != 1 && term[ct][0] != -1 && term[ct][1] != -1 && sort[ct % 2] == -1)
			{
				if (arm[arm0] == color[ct])
					sortDrop(arm1, ct % 2);
				else if (arm[arm1] == color[ct])
					sortDrop(arm0, ct % 2);
			}
		}
		else ct = adj;
	}
	else if (arm[arm0] != color[adj] && arm[arm1] != color[adj])
	{
		if (armCount == 0 || (armCount == 1 && sort[ct % 2] != -1) || ((armCount == 2 || armCount == 1) && (term[adj][0] == color[adj] || term[adj][0] == -1) && (term[adj][1] == color[adj] || term[adj][1] == -1)))
		{
			if (armCount>0)
			{
				if (sort[ct % 2] != -1)
					if (arm[arm0] == -1)
						pickSort(arm0, ct % 2);
					else if (arm[arm1] == -1)
						pickSort(arm1, ct % 2);
			}
			traverseToSort(ct % 2 + 4, (ct + 1) % 2 + 4);
			newSort();
			if (ct == 4 || ct == 5)
				newSort();
			return;
		}
		else ct = adj;
	}
	else
	{
		if (armCount == 0 && ((visited[adj] == 1 && (term[adj][0] != -1 && term[adj][1] != -1)) || visited[adj] == 0) && sort[ct % 2] == -1)
			if (arm[arm0] == color[adj])
				sortDrop(arm1, ct % 2);
			else sortDrop(arm0, ct % 2);
		else if (armCount == 1 && (term[adj][0] == -1 || term[adj][1] == -1) && sort[ct % 2] != -1)
			if (arm[arm0] == -1)
				pickSort(arm0, ct % 2);
			else	pickSort(arm1, ct % 2);
			ct = adj;
	}
	adj = adjCount(ct);
}

void predict()
{
	if (term[0][0] == color[0] && term[0][1] == color[0] && term[1][0] == color[1] && term[1][1] == color[1] && term[ct][0] == color[ct] && term[ct][1] == color[ct] && sort[0] == -1 && sort[1] == -1 && arm[0] == -1 && arm[1] == -1)
		sorted = total;

}

/*************************GULLA CODE**Blackline Forward***********/

/*************************END GULLA CODE*************/

/*
	//variable 'i' scales at 13,14 for sharp sensor for velocitty 240 240
	//u turn 1600ms at 200,200 velocity
	velocity(200,200);
	left();
	_delay_ms(1600);
	stop();
	while(1);
	
	threshold=6000;
	
	right();
	while(ADC_Conversion(11)<65)
	{
		i++;
		lcd_print(1,11,i,3);
	}
	stop();
	lcd_print(2,11,scan(),1);
	stop();
	while(1);
	*/
//**************END SORT LOGIC*******************

//**************INITIALISATION*******************

//**************END INITIALISATION*******************



void setIndicatorAndColor()
{
	int i;
	for (i = 0; i<4; i++)
	{
		term[i][0] = term[i][1] = -2;
		visited[i] = 0;
		color[i]=i;
	}
	for (i = 0; i<4; i++)
		indicator[color[i]] = i;
	sort[0] = sort[1] = -1;
	arm[0] = arm[1] = -1;
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
	while(ADC_Conversion(2)<40);
	stop();
	color[3]=scan();
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
	color[2]=scan();
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
	color[1]=scan();
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
	color[0]=scan();
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
	
	//buzzer_on();	
}

void __init__()
{
	init_devices();
	lcd_set_4bit();
	lcd_init();
	color_sensor_scaling();
	
	threshold=8000;
	servo_1(80);
	servo_2(90);
	servo_3(0);
	//servo_1(0);
//	servo_3(0);
	_delay_ms(200);
 	servo_3_free();
 	servo_2_free();
 	servo_1_free();
	_delay_ms(200);
	ct = 3;
	adj = 1;
	dir = 2;
	ot = 3;
	//buzzer();
}
int main(void)
{
	__init__();
// 	buzzer();
//  	 	forward();
//  	 	_delay_ms(500);
//  	 	stop();
//  		//buzzer();
//  	 	indicator_set();
	setIndicatorAndColor();
// 	servo_1(0); //Left
// 	servo_3(90);
// 	_delay_ms(2000);
// 	servo_1(80);
// 	servo_3(0);
// 	while(1);
// 	while(1)
// 	{
// 		// 		_delay_ms(1000);
// 		// 		servo_1(90);
// 		// 		_delay_ms(1000);
// 		// 		servo_1(0);
// 		
// 	//	_delay_ms(500);
// 		pick(1);
// 	//	_delay_ms(500);
// 		drop(1);
// 	//	_delay_ms(500);
// 		pick(0);
// 	//	_delay_ms(500);
// 		drop(0);
// 	}
	forwardJaa();
	while (sorted<total)
	{
		canDrop();
		//buzzer_on();
		//_delay_ms(500);
		//buzzer_off();
		if (visitedCount == 3)
			predict();
		if (sorted == total)
			break;
		pickup();
		if (sorted == total)
			break;
		traverseToSort(ct, ct % 2 + 4);
		sortCheck();
	}
	buzzer_on();
	while(1);
}