//MyAlgo
/*--Indicators--*/
/*Red=0;
Green=1;
Blue=2;
Black=3;
Null=-1;

terminal nos.
termN=n-1; //0--right;1--left;
Sort0=4;//i.e. 0+4;
Sort1=5;//i.e 1+4;
*/
/*--Variables--*/
#define F_CPU 14745600
#include<stdio.h>
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
int flag=0,angle;
int visitedCount = 0;
int ot = 0, dir = 0;
int i=0;
unsigned char cur_angle=90;
const int RED=0,GREEN=1,BLUE=2,BLACK=3,EMPTY=-1;
const int turn_v=150,line_v=240,correct_v=200;


int threshold;
unsigned char Left_white_line = 0;
unsigned char Center_white_line = 0;
unsigned char Right_white_line = 0;
unsigned char lf=0;

void buzzer();
int scan();
void backJaa();
void forwardJaa();
void terminalCheck1();
void terminalCheck2();
void position();
void set_color();
void node();
void lcd(char *str);

/**************************************************
******************MOVEMENT*************************
**************************************************/

/*******MOVEMENT
void nodeLeft()
{
	node();
	unsigned int a=0;
	left();
	_delay_ms(1100);
	stop();
	//...buzzer_on();
	_delay_ms(1000);
	//...buzzer_off();
	
}
void nodeRight()
{
	node();
	unsigned int a=0;
	right();
	_delay_ms(1100);
	stop();
	//...buzzer_on();
	_delay_ms(1000);
	//...buzzer_off();
}

void Uturn()
{
	velocity(turn_v,turn_v);
	left_degrees(180);
}
void backward()	//TODO
{
	backJaa();
	cost++;
	//..printf("Back\n");
}



********END MOVEMENT**********

*************************************************
******************BLACK*LINE***********************
**************************************************/
void node()
{
	forward_mm(60);
}
/*************************GULLA CODE**Blackline Forward***********/
char Delay(int tim)
{
	int i;
	for(i=0;i<tim && ADC_Conversion(2)<=40;i++)
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
	if(Center_white_line<40)
	{
		if(Left_white_line>40 && Right_white_line<40) //bww
		{			
			while(Center_white_line<40 && Left_white_line>40 && Right_white_line<40)
			{
				left();
				set_color();
			}
		}
		else if(Right_white_line>40 && Left_white_line<40)	//wwb
		{
			while(Center_white_line<40 && Left_white_line<40 && Right_white_line>40)
			{
				right();
				set_color();
			}
			
		}
	}
}
void correct()
{
	unsigned int d=2;
	unsigned int i=30;
	Degrees=5;	stop();
	if(lf==1)
	{
		lf=0;
		while(1)
		{
		
			right();
			if(d==2)
			{
				if(Delay(i))
					return;
			}
			else
			{
				if(Delay(2*i+d))
					return;
			}
			stop();
		
			//set_color();
			if(ADC_Conversion(2)>40)
				break;
			semiCorrect();
			left();
			if(d==2)
			{
				if(Delay(2*i))
					return;
			}
			else
			{
				if(Delay(2*i+d))
					return;
			}
			stop();
			if(ADC_Conversion(2)<40)
				break;
			semiCorrect();
			//d*=2;
			//set_color();
			//i+=2;
			d=d+20;
		}
	}
	else
	{
		lf=1;
		while(1)
		{
			left();
			if(d==2)
			{
				if(Delay(i))
				return;
			}
			else
			{
				if(Delay(2*i+d))
				return;
			}
			stop();
			
			//set_color();
			if(ADC_Conversion(2)>40)
			break;
			semiCorrect();
			right();
			if(d==2)
			{
				if(Delay(2*i))
				return;
			}
			else
			{
				if(Delay(2*i+d))
				return;
			}
			stop();
			if(ADC_Conversion(2)<40)
			break;
			semiCorrect();
			//d*=2;
			//set_color();
			//i+=2;
			d=d+20;
		}
	}
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
			flag=1;
			while(Center_white_line<40 && Left_white_line>40 && Right_white_line<40)
			{
				left();
				set_color();
			}

		}
		else if(Right_white_line>40 && Left_white_line<40)	//wwb
		{
			flag=1;
			
			while(Center_white_line<40 && Left_white_line<40 && Right_white_line>40)
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
			flag=1;
			node();
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
			node();
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

void forwardJaa()
{
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
	
	noNatak();
	forwardJaa();
	return;
}

void backJaa()
{
	unsigned char numberOfNodes=0;
	do
	{
		back();
		//_delay_ms(200);
		set_color();
		if(Center_white_line>40 && (Left_white_line>40 || Right_white_line>40)) //2 bbw wbb
		{
			//_delay_ms(1000);
			back();
			_delay_ms(500);
			numberOfNodes++;
			if(numberOfNodes==2)
				return;
		}
	}while(1);
}
/*************************END GULLA CODE*************/
/**************************************************

************************END GULLA CODE*************/
/**************************************************
******************COLOR****************************
**************************************************/

void set_color()
{
	Left_white_line = ADC_Conversion(3);	//Getting data of Left WL Sensor
	Center_white_line = ADC_Conversion(2);	//Getting data of Center WL Sensor
	Right_white_line = ADC_Conversion(1);	//Getting data of Right WL Sensor
}

void show_color()
{
	print_sensor(1,1,3);	//Prints value of White Line Sensor1
	print_sensor(1,5,2);	//Prints Value of White Line Sensor2
	print_sensor(1,9,1);	//Prints Value of White Line Sensor3
}
int calcThresh()
{
	int c[4],i;
	
	red_read();
	blue_read();
	green_read();
	for(i=0;i<4;i++)
	c[i]=0;
	for(i=0;i<4;i++)
	{
		//read();
		if(red>green && red>blue)
		if(c[0]>red)
		{
			if(c[3]<red)
			c[3]=red;
		}
		else
		{
			if(c[3]<c[0])
			c[3]=c[0];
			c[0]=red;
		}
		else if(blue>green && red<blue)
		if(c[1]>blue)
		{
			if(c[3]<blue)
			c[3]=blue;
		}
		else
		{
			if(c[3]<c[1])
			c[3]=c[1];
			c[1]=blue;
		}
		else
		if(c[2]>green)
		{
			if(c[3]<green)
			c[3]=green;
		}
		else
		{
			if(c[3]<c[2])
			c[3]=c[2];
			c[2]=green;
		}
		
	}
	int t=c[3];
	
	
	
	lcd_cursor(1,1);  //set the cursor on row 1, column 1
	lcd_string("Threshold"); // Display "Blue Pulses" on LCD
	t+=200;
	lcd_print(2,1,t,5);
	_delay_ms(10000);
	lcd_wr_command(0x01); //Clear the LCD
	lcd((char*)t);
	return t;
}

/**************************************************
******************SERVO****************************
**************************************************/


void lower(unsigned char side)
{
	if (side == 1)
	{
		for(i=90;i>=60;i--)
		{
			servo_2(i);
			_delay_ms(10);
		}
		cur_angle=60;
	}
	else
	{
		for(i=90;i<=120;i++)
		{
			servo_2(i);
			_delay_ms(10);
		}
		cur_angle=120;
	}
}
void elevate()
{
	if(cur_angle==60)
	{
		for(i=60;i<=90;i++)
		{
			servo_2(i);
			_delay_ms(10);
		}
	}
	else
	{
		for(i=120;i>=90;i--)
		{
			servo_2(i);
			_delay_ms(10);
		}
	}
	cur_angle=90;
}
void open(unsigned char side)
{
	if (side == 0)
	for(i=55;i>0;i--)
	{
		servo_3(i);
		_delay_ms(10);
	}
	
	else if (side == 1)
	for(i=55;i>0;i--)
	{
		servo_1(i);
		_delay_ms(10);
	}
}
void close(unsigned char side)
{
	if (side == 0)
	for(i=0;i<55;i++)
	{
		servo_3(i);
		_delay_ms(10);
	}
	else if (side == 1)
	for(i=0;i<55;i++)
	{
		servo_1(i);
		_delay_ms(10);
	}
}


/**************************************************
******************SORT*****************************
**************************************************/
//whenever v find a blank space or box belonging to dat space total dec by 1;and whenever v place box in terminal sort++;
/*--functions--*/


void buzzer()
{
	buzzer_on();
	_delay_ms(100);
	buzzer_off();
}
void lcd(char *str)
{
	
	lcd_wr_command(0x01);
	lcd_cursor(1,1);
	lcd_string(str);
	buzzer();
	_delay_ms(1000);
}
void front()
{
	forwardJaa();
	cost++;
	//printf("Front\n");
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
		right_degrees(30);
		velocity(turn_v, turn_v);
		while (ADC_Conversion(2)<70)
			right();
	//	_delay_ms(100);
		stop();	
	}
	lcd("Right turn");
	//_delay_ms(2000);
	dir = (dir + 1) % 4;
	//printf("Turn Right \n");
	angle += 90;
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
		left_degrees(30);
		velocity(turn_v,turn_v);
		while (ADC_Conversion(2)<70)
			left();
	//	_delay_ms(100);
		stop();
	}
	lcd("Left turn");
	//_delay_ms(2000);
	dir = (dir + 3) % 4;
	//printf("Turn Left\n");
	angle += 90;
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
		velocity(turn_v, turn_v);
		left_degrees(180);
	}
	else
	{
		velocity(turn_v, turn_v);
		left_degrees(150);
		while (ADC_Conversion(2)<70)
			left();
		stop();
	}
	lcd("turn");
	//_delay_ms(2000);
	dir = (dir + 2) % 4;
	//printf("Turn\n");
	angle += 180;
}

void traverseToSort(int a, int b)
{

	if (flag == 1)
		flag = 0;
	if (a == 4 || a == 5)
	{
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
		if ((dir == 1 && (ot == 0 || ot == 1)) || (dir == 3 && (ot == 2 || ot == 2)))
		{
			turnRight();
		}
		else if ((dir == 3 && (ot == 0 || ot == 1)) || (dir == 1 && (ot == 2 || ot == 2)))
		{
			turnLeft();
		}
		if ((a <= 1 && dir == 0) || (a >= 2 && dir == 2))
			turn();
		front();
		ot = ct % 2 + 4;
	}
}
int scan()//return the color no.
{
	red_read();
	blue_read();
	green_read();
	_delay_ms(100);
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

int adjCount(ct)
{
	return (ct + 2) % 4;
}
void terminalCheck1()
{
	//????
	forward_mm(30);
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
	right_degrees(50);
	velocity(turn_v, turn_v);
	while (ADC_Conversion(1)<70)
		right();
	//	_delay_ms(100);
	stop();
	dir = (dir + 1) % 4;
	term[ct][0] = scan();
	if(term[ct][0]==-1)
		lcd_print(2,11,9, 1);
	else
		lcd_print(2,11, term[ct][0], 1);
	_delay_ms(1000);
	//printf("Enter term[%d][%d]\n", ct, 0);
	//scanf("%d", &term[ct][0]);

	if (term[ct][0] == -1 || term[ct][0] == color[ct])
		total--;
}
void terminalCheck2()
{
	if (flag == 0)
	{
		if (dir == 0)
			if (ot == 0 || ot == 1)
				forward_mm(30);
			else back_mm(30);
		else if (ot == 0 || ot == 1)
			back_mm(30);
		else forward_mm(30);
		flag = 1;
	}

	if (((ct == 0 || ct == 1) && dir == 0) || ((ct == 2 || ct == 3) && dir == 2))
	{
		left_degrees(30);
		velocity(turn_v, turn_v);
		while (ADC_Conversion(1)<70)
		left();
		//	_delay_ms(100);
		stop();
	}
	else if (((ct == 0 || ct == 1) && dir == 2) || ((ct == 2 || ct == 3) && dir == 0))
		{
			right_degrees(30);
			velocity(turn_v, turn_v);
			while (ADC_Conversion(1)<70)
				right();
			//	_delay_ms(100);
			stop();	
		}
	else {
		left_degrees(150);
		velocity(turn_v, turn_v);
		while (ADC_Conversion(1)<70)
		left();
		//	_delay_ms(100);
		stop();
		dir = (dir + 3) % 4;
			}
	//printf("Enter term[%d][%d]\n", ct, 1);
	//scanf("%d", &term[ct][1]);
	term[ct][1] = scan();
	if(term[ct][1]==-1)
		lcd_print(2,11,9, 1);
	else
		lcd_print(2,11, term[ct][1], 1);
	_delay_ms(1000);
	if (term[ct][1] == -1 || term[ct][1] == color[ct])
		total--;
	visited[ct] = 1;
	visitedCount++;
}

void pick(int side)
{	
	lower(side);//lower
	open(side);
	close(side);
	elevate();//mid
	armCount--;
	if (side == 0)
		lcd("pickRight");
	else
		lcd("pickLeft");
}

void position(int armNo, int side)
{
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
		if (flag == 1 && (dir == 0 || dir == 2))
		{
			back_mm(30);
			flag = 0;
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
	pick(armNo);
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
						pickNode(arm0, 0);
					else	pickNode(arm1, 1);
				else	if ((term[ct][1] != color[ct]) && term[ct][1] != -1)
				{
					if (term[ct][0] != color[ct] && term[ct][0] != -1)
					{
						pickNode(arm1, 1);
						pickNode(arm0, 0);
					}
					else if (sort[ct % 2] == -1 || ((term[adj][0] == color[adj] || term[adj][0] == -1) || (term[adj][1] == color[adj] || term[adj][1] == -1)) || (term[ct][0] == color[ct] || term[ct][1] == color[ct]))
						pickNode(arm1, 1);
				}
				else	if (term[ct][0] != color[ct] && term[ct][0] != -1)
				{
					if (sort[ct % 2] == -1 || ((term[adj][0] == color[adj] || term[adj][0] == -1) || (term[adj][1] == color[adj] || term[adj][1] == -1)))
						pickNode(arm0, 0);
				}
				else	if (term[ct][0] != color[ct] && term[ct][0] != -1)
					pickNode(arm0, 0);

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
	open(side);
	elevate();//mid
	
	armCount++;
	if(side==0)
	lcd("dropRight");
	else
	lcd("dropLeft");
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
			terminalCheck1();
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
				if (visited[ct] == 0)
					terminalCheck2();
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
	int t1, t2, a1=-2, a2=-2;
	if (arm[0] != -1)
		a1 = indicator[arm[0]];
	if (arm[1] != -1)
		a2 = indicator[arm[1]];

	t1 = (ct + 1) % 2;
	t2 = t1 + 2;
	if (arm[0] != -1 || arm[1] != -1)
	{
		ct = a1;
		if(arm[0]==arm[1] && (visited[a1]==0 ||(term[a1][0] != -1 && term[a1][1] != -1)))
		{
			if(a1== 0 || a1==3)
				sortDrop(1,t1);
		}
		else if (arm[0] != -1 && (term[a1][0] == -1 || term[a1][1] == -1))
		{
			if (arm[1] == -1)
			{
				if (sort[t1] == color[t1] || sort[t1] == color[t2])
					pickSort(1, t1);
			}
		}
		else if (arm[1] != -1 && (term[a2][0] == -1 || term[a2][1] == -1))
		{
			ct = a2;
			if (term[a2][0] == -1 || term[a2][1] == -1)
			{
				if (arm[0] == -1)
				{
					if (sort[t1] == color[t1] && sort[t1] == color[t2])
						pickSort(1, t1);
				}
			}
			else	if (sort[t1] == -1 && arm[0] != -1)
				sortDrop(1, t1);
		}
		else	if (arm[0] != -1 && (sort[t1] == -1 && arm[1] != -1))
			sortDrop(1, t1);
		else if (arm[0] != -1)
			ct = arm[0];
		else ct = arm[1];
	}
	else
	{
		if ((sort[t1] == color[t1] || sort[t1] == color[t2]) && sort[t1] != -1)
			ct = indicator[sort[t1]];
		else if (sort[t1] != -1)
		{
			pickSort(0, t1);
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
int main()
{
	int i=0;
	init_devices();
	lcd_set_4bit();
	lcd_init();
	color_sensor_scaling();
	
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
	setIndicatorAndColor();
	
	threshold=6000;
	
	ct = 0; adj = 2;
	//lcd("Begin");
	forwardJaa();
	stop();
	servo_1(0);
	servo_2(90);
	servo_3(0);
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
		traverseToSort(ct, ct % 2 + 4);
		sortCheck();
	}
	for (i = 0; i<4; i++);
		//..printf("%d %d\n", term[i][0], term[i][1]);
	//..printf("Sort 0=%dSort 1=%d\nArm 0=%dArm 1=%d\n", sort[0], sort[1], arm[0], arm[1]);
	//..printf("Cost=%d\nSORTED!!!!!\n", cost + 7);
	//getch();
	return 0;
}