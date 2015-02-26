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
int visitedCount = 0;
int ot = 0, dir = 0;
const int RED=0,GREEN=1,BLUE=2,BLACK=3,EMPTY=-1;
int threshold,i=0;
unsigned char Left_white_line = 0;
unsigned char Center_white_line = 0;
unsigned char Right_white_line = 0;
unsigned int v=100;
unsigned char lf=0;
void goForward();
void lcd(char*);

//whenever v find a blank space or box belonging to dat space total dec by 1;and whenever v place box in terminal sort++;
/*--functions--*/
void set_color()
{
	Left_white_line = ADC_Conversion(3);	//Getting data of Left WL Sensor
	Center_white_line = ADC_Conversion(2);	//Getting data of Center WL Sensor
	Right_white_line = ADC_Conversion(1);	//Getting data of Right WL Sensor
	/*lcd_print(1,1,Left_white_line,3);	//Prints value of White Line Sensor1
	lcd_print(1,5,Center_white_line,3);	//Prints Value of White Line Sensor2
	lcd_print(1,9,Right_white_line,3);	//Prints Value of White Line Sensor3
	*/
}

void show_color()
{
	print_sensor(1,1,3);	//Prints value of White Line Sensor1
	print_sensor(1,5,2);	//Prints Value of White Line Sensor2
	print_sensor(1,9,1);	//Prints Value of White Line Sensor3
}
/*
void turnLeft()
{
	do{
		soft_right();
	}while((Center_white_line<40) && (Left_white_line<40 || Right_white_line<40));
	goForward();
}
*/
/*************************GULLA CODE*******
**********Blackline Forward***********------------
------------------------------
------------------------------------------------*/
/*
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
			lcd("bww");
			
			while(Center_white_line<40 && Left_white_line>40 && Right_white_line<40)
			{
				left();
				set_color();
			}
			lcd("-");
		}
		else if(Right_white_line>40 && Left_white_line<40)	//wwb
		{
			
			lcd("wwb");
			while(Center_white_line<40 && Left_white_line<40 && Right_white_line>40)
			{
				right();
				set_color();
			}
			lcd("-");
			
		}
	}
}
void correct()
{
	unsigned int d=2;
	unsigned int i=30;
	Degrees=5;
	lcd("cor");
	stop();
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
	lcd("-");
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
			while(Center_white_line<40 && Left_white_line>40 && Right_white_line<40)
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
			while(Center_white_line<40 && Left_white_line<40 && Right_white_line>40)
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
		}
		velocity(240,240);
		//velocity(v+vi,v+vi);
		//i+=20;
	}while(Center_white_line>0x28 && Left_white_line<40 && Right_white_line<40);	//wbw
	
	noNatak();
	forwardJaa();
	return;
}

void lcd(char *str)
{
	/*
	lcd_wr_command(0x01);
	lcd_cursor(2,11);
	lcd_string(str);
	*/
}
int calcThresh()
{
	int c[4],i;
	lcd("calcThresh");
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

/*******MOVEMENT**********/
void front()
{
	goForward();
	cost++;
	//..printf("Front\n");
}
void backward()	//TODO
{
	
	cost++;
	//..printf("Back\n");
}
void turnRight()	//turns the robo right
{
	dir = (dir + 1) % 4;
	//..printf("Turn Right \n");
}
void turnLeft()	//turns the robo left
{
	dir = (dir + 3) % 4;
	//..printf("Turn Left\n");
}
void turn()	//turn robo by 180 degree
{
	dir = (dir + 2) % 4;
	//..//..printf("Turn\n");
}
/*********END MOVEMENT***********/
void traverseToSort(int a, int b)
{
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
		if ((a <= 1 && dir == 0) || (a >= 2 && dir == 2))
			backward();
		else front();
		ot = ct % 2 + 4;
	}
}

/*******COLOR******/
int scan()//return the color no.
{
	
	red_read();
	blue_read();
	green_read();
	if (red<threshold && green < threshold && blue < threshold)
		return BLACK;
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
/********END COLOR*******/
int adjCount(int ct)
{
	return (ct + 2) % 4;
}
void terminalCheck1()
{
	if (ct != ot)
	{
		if (dir == 1 || dir == 3)
		{
			if (ct == 3 || ct == 0)
				turnRight();
			else turnLeft();
			front();
		}
		if (((ct == 0 || ct == 1) && dir == 2) || ((ct == 2 || ct == 3) && dir == 0))
			backward();
		ot = ct;
	}
	if (((ct == 0 || ct == 1) && dir == 0) || ((ct == 2 || ct == 3) && dir == 2))
		turnRight();
	else turnLeft();
	//..printf("Enter term[%d][%d]\n", ct, 0);
	//..scanf("%d", &term[ct][0]);
	term[ct][0]=scan();
	if (term[ct][0] == -1 || term[ct][0] == color[ct])
		total--;
}
void terminalCheck2()
{
	if (((ct == 0 || ct == 1) && dir == 0) || ((ct == 2 || ct == 3) && dir == 2))
		turnLeft();
	else if (((ct == 0 || ct == 1) && dir == 2) || ((ct == 2 || ct == 3) && dir == 0))
		turnRight();
	else turn();
	//..printf("Enter term[%d][%d]\n", ct, 1);
	//..scanf("%d", &term[ct][1]);
	term[ct][1]=scan();
	if (term[ct][1] == -1 || term[ct][1] == color[ct])
		total--;
	visited[ct] = 1;
	visitedCount++;
}
void elevate(unsigned char angle, unsigned char side)
{
	if (side == 0)
	servo_1(angle);
	else if (side == 1)
	servo_1((angle == 45) ? 0 : 45);
}
void open(unsigned char side)
{
	if (side == 0)
	servo_2(180);
	else if (side == 1)
	servo_3(180);
}
void close(unsigned char side)
{
	if (side == 0)
	servo_2(0);
	else if (side == 1)
	servo_3(0);
}
void pick(int side)	//TODO delay
{
	elevate(0, side);//lower
	open(side);
	close(side);
	elevate(45, side);//mid
	armCount--;
}

void pickNode(int armNo, int side)
{
	lcd("pickNode");
	arm[armNo] = term[ct][side];
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
		}
		else	if (((ct == 0 || ct == 1) && dir == 3) || ((ct == 2 || ct == 3) && dir == 1))
			if (armNo == side)
				turnRight();
		pick(armNo);
		term[ct][side] = -1;
	}
	else
	{
		ot = ct;
		if (dir == 0 || dir == 2)
		{
			if ((dir == 0 && (ct == 0 || ct == 1)) || (dir == 2 && (ct == 2 || ct == 3)))
			{
				if (armNo == side)
					front();
				else
				{
					front();
					turn();
				}
			}
			else
			{
				if (armNo == side)
				{
					turn();
					front();
				}
				else
				{
					backward();
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
					turnLeft();
				else turnRight();
				backward();
			}
		}
		pick(armNo);
		term[ct][side] = -1;
	}
	//..printf("Arm %d picked %d from term[%d][%d]\n", armNo, arm[armNo], ct, side);
}

void pickSort(int armNo, int sortNo)
{
	lcd("pickSort");
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
	//..printf("Arm %d picked %d from sort[%d]\n", armNo, arm[armNo], sortNo);
}
void pickup()
{
	lcd("pickup");
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

void drop(int side)	//TODO delay
{
	lcd("drop");
	elevate(0, side);//lower
	open(side);
	elevate(45, side);//mid
	close(side);
	armCount++;
}
void nodeDrop(int armNo, int side)
{
	lcd("nodeDrop");
	term[ct][side] = arm[armNo];
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
		}
		else	if (((ct == 0 || ct == 1) && dir == 3) || ((ct == 2 || ct == 3) && dir == 1))
			if (armNo == side)
				turnRight();
		drop(armNo);
		arm[armNo] = -1;
	}
	else
	{
		ot = ct;
		if (dir == 0 || dir == 2)

			if ((dir == 0 && (ct == 0 || ct == 2)) || (dir == 2 && (ct == 1 || ct == 3)))
				if (armNo == side)
					front();
				else
				{
					front();
					turn();
				}
			else
			{
				if (armNo == side)
				{
					turn();
					front();
				}
				else
				{
					backward();
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
					turnLeft();
				else turnRight();
				backward();
			}
		}
		drop(armNo);
		arm[armNo] = -1;
	}

	ot = ct;
	//..printf("Arm %d dropped %d on term[%d][%d]\n", armNo, term[ct][side], ct, side);
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
	//..printf("Arm %d dropped %d on sort[%d]\n", armNo, sort[sortNo], sortNo);
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
	lcd("canDrop");
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
	int t1, t2, a1, a2;
	if (arm[0] != -1)
		a1 = indicator[arm[0]];
	if (arm[1] != -1)
		a2 = indicator[arm[1]];

	t1 = (ct + 1) % 2;
	t2 = t1 + 2;
	if (arm[0] != -1 || arm[1] != -1)
	{
		ct = a1;
		if (arm[0] != -1 && (term[a1][0] == -1 || term[a1][1] == -1))
		{
			if (arm[1] == -1)
			{
				if (sort[t1] == color[t1] || sort[t1] == color[t2])
					pickSort(1, a1);
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
	//int a1, a2;
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
	init_devices();
	lcd_set_4bit();
	lcd_init();
	color_sensor_scaling();/*
	threshold= calcThresh();
	setIndicatorAndColor();
	ct = 0; adj = 2;
	lcd("Begin");
	while (sorted<total)
	{
		canDrop();
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
	*/
	
	forwardJaa();
	return 1;
}