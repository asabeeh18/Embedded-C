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

#define F_CPU 14745600
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "strong.c"

unsigned int threshold = 10;
unsigned char RED = 1, GREEN = 2, BLUE = 3, BLACK = 4, EMPTY = 0;
unsigned char Center_white_line;
unsigned char Right_white_line;
unsigned char Left_white_line;
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
//whenever v find a blank space or box belonging to dat space total dec by 1;and whenever v place box in terminal sort++;
/*--functions--*/
void turnRight()	//turns the robo right
{
	
}
void turnLeft()	//turns the robo left
{
	;
}
void Uturn()	//turn robo by 180 degree
{
	soft_right();
	_delay_ms(2400);	//for 255,255;; more for 200,200
	stop();
}
/*
void travel(int a, int b)//travels from current terminal to next terminal
{
	int i;
	if (count == 25)
	{
		for (i = 0; i<4; i++)
			//..printf("%d %d\n",term[i][0],term[i][1]);
			//..printf("Sort 0=%d Sort 1=%d\nArm 0=%d Arm 1=%d ArmC=%d Sorted=%d\n",sort[0],sort[1],arm[0],arm[1],armCount,sorted);
			while (1);
	}
	else count++;
	if ((a == 4 || a == 5) && (b == 4 || b == 5))
		cost += 4;
	else cost += 1;
	//..printf("Move from %d to %d\n",a,b);
}
*/

int scan()//return the color no.
{
	red_read();
	blue_read();
	green_read();
	if (!(red>threshold && green > threshold && blue > threshold))
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


//????????
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

int adjCount(int ct)
{
	return (ct + 2) % 4;
}
void terminalCheck()
{
	turnRight();
	//..printf("Enter term[%d][%d]\n",ct,0);
	//..scanf("%d",&term[ct][0]);
	if (term[ct][0] == -1 || term[ct][0] == color[ct])
		total--;
	Uturn();
	//..printf("Enter term[%d][%d]\n",ct,1);
	//..scanf("%d",&term[ct][1]);
	if (term[ct][1] == -1 || term[ct][1] == color[ct])
		total--;
	visited[ct] = 1;
	visitedCount++;
}

void nodeRight()
{
	soft_right();
	_delay_ms(1400);	//for 255,255;; more for 200,200
	stop();
}
void nodeLeft()
{
	soft_left();
	_delay_ms(1400);	//for 255,255;; more for 200,200
	stop();
}

void buzzer()
{

	buzzer_on();
	////_delay_ms(1000);
	buzzer_off();
}

void set_color()
{
	Left_white_line = ADC_Conversion(3);	//Getting data of Left WL Sensor
	Center_white_line = ADC_Conversion(2);	//Getting data of Center WL Sensor
	Right_white_line = ADC_Conversion(1);	//Getting data of Right WL Sensor

}

void correct()
{
	unsigned int i = 0;
	Degrees = 5;
	for (; i < 3; i++)
	{
		left(); //Left wheel backward, Right wheel forward
		lcd_print(2, 7, 777, 3);
		angle_rotate(Degrees);
		stop();
		set_color();
		if (Center_white_line>40)
			return;
	}
	//normal
	right();
	while (Center_white_line < 40)
	{
		//	lcd_print(2,1,7,1);
		set_color();

	}
	return;
}
void noNatak()
{
	//buzzer_on();
	//lcd_print(2,1,7,1);
	correct();

	stop();
	//lcd_print(2,1,6,1);
	//buzzer_off();

}

void forwardJaa()
{
	do
	{
		set_color();
		if (Center_white_line > 40 && (Left_white_line > 40 || Right_white_line > 40))
		{
			forward();
			//_delay_ms(4000);
			return;
		}
		/*
		print_sensor(1,1,3);	//Prints value of White Line Sensor1
		print_sensor(1,5,2);	//Prints Value of White Line Sensor2
		print_sensor(1,9,1);	//Prints Value of White Line Sensor3
		*/
		forward();
		velocity(200, 200);

	} while (Center_white_line > 0x28);

	noNatak();
	forwardJaa();
}

void travel(unsigned char CT,unsigned char nxTerm)
{

	forwardJaa();
	if ((CT == 1 && (nxTerm == 2 || nxTerm == 4)) || (CT == 4 && (nxTerm == 1 || nxTerm == 3)))
	{
		nodeLeft();
		forwardJaa();
		//_delay_ms(1000);
		forwardJaa();
		if (nxTerm == 1 || nxTerm == 4)
			nodeRight();
		else
			nodeLeft();
	}
	else if ((CT == 2 && (nxTerm == 3 || nxTerm == 1)) || (CT == 3 && (nxTerm == 4 || nxTerm == 2)))
	{
		nodeRight();
		forwardJaa();
		//_delay_ms(500);
		forwardJaa();
		if (nxTerm == 2 || nxTerm == 3)
			nodeLeft();
		else
			nodeRight();
	}
	else
	{
		//_delay_ms(1000);
	}
	forwardJaa();
	stop();
	buzzer();
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
void pick(int side)
{
	armCount--;
	elevate(0, side);//lower
	open(side);
	close(side);
	elevate(45, side);//mid
}

void pickNode(int armNo, int side)
{
	//..printf("Arm %d picked %d from term[%d][%d]\n",armNo,term[ct][side],ct,side);
	arm[armNo] = term[ct][side];
	if (armNo != side)
		Uturn();
	pick(armNo);
	if (armNo != side)
		Uturn();
	term[ct][side] = -1;
}

void pickSort(int armNo, int sortNo)
{
	//..printf("Arm %d picked %d from sort[%d]\n",armNo,sort[sortNo],sortNo);
	arm[armNo] = sort[sortNo];
	if (armNo == 0)
		turnLeft();
	else turnRight();
	pick(armNo);
	if (armNo == 0)
		turnRight();
	else turnLeft();
	sort[sortNo] = -1;
}
void pickup()
{
	if ((term[ct][0] != color[ct] && term[ct][0] != -1) || (term[ct][1] != color[ct] && term[ct][1] != -1))
	{
		if (armCount == 2)
		{

			if (term[ct][0] == color[adj] || term[ct][1] == color[adj])
				if (term[ct][0] == color[adj])
					pickNode(0, 0);
				else	pickNode(1, 1);
			else	if ((term[ct][1] != color[ct]) && term[ct][1] != -1)
			{
				if (term[ct][0] != color[ct] && term[ct][0] != -1)
				{
					pickNode(1, 1);
					pickNode(0, 0);
				}
				else if (sort[ct % 2] == -1 || ((term[adj][0] == color[adj] || term[adj][0] == -1) || (term[adj][1] == color[adj] || term[adj][1] == -1)))
					pickNode(1, 1);
			}
			else	if (term[ct][0] != color[ct] && term[ct][0] != -1)
			{
				if (sort[ct % 2] == -1 || ((term[adj][0] == color[adj] || term[adj][0] == -1) || (term[adj][1] == color[adj] || term[adj][1] == -1)))
					pickNode(0, 0);
			}
			else	if (term[ct][0] != color[ct] && term[ct][0] != -1)
				pickNode(0, 0);

		}
		if (armCount == 1)
		{
			int a1, a2;
			if (arm[0] == -1)
				a1 = 0;
			else a1 = 1;
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
					{
						if (term[ct][a1] == color[adj])
							pickNode(a1, a1);

						else
							pickNode(a1, a2);
					}
				}
				else {
					if (sort[ct % 2] != color[ct] && sort[ct % 2] != color[adj])
					{
						if ((visited[adj] == 1 && ((term[adj][0] != color[ct]) && (term[adj][1] != color[ct]))) && arm[a2] != color[adj] || arm[a2] != color[adj])
							if (term[ct][a1] != -1 && term[ct][a1] != color[ct])
								pickNode(a1, a1);
							else if (term[ct][a2] != -1 && term[ct][a2] != color[ct])
								pickNode(a1, a2);
					}
				}
			}
		}
	}
}

void drop(int side)
{
	armCount++;
	elevate(0, side);//lower
	open(side);
	elevate(45, side);//mid
	close(side);
}
void nodeDrop(int armNo, int side)
{
	//..printf("Arm %d dropped %d on term[%d][%d]\n",armNo,arm[armNo],ct,side);
	term[ct][side] = arm[armNo];
	if (armNo != side)
		Uturn();
	drop(armNo);
	if (armNo != side)
		Uturn();
	arm[armNo] = -1;
	sorted++;
}
void sortDrop(int armNo, int sortNo)
{
	//..printf("Arm %d dropped %d on sort[%d]\n",armNo,arm[armNo],sortNo);
	sort[sortNo] = arm[armNo];
	if (armNo == 0)
		turnLeft();
	else turnLeft();
	drop(armNo);
	if (armNo == 0)
		turnRight();
	else turnLeft();
	arm[armNo] = -1;
}

void dropPickup(int a1)
{
	int a2 = (a1 + 1) % 2;
	if (term[ct][a2] == color[ct] && term[ct][a2] != color[adj])
	{
		if (term[ct][a1] != color[ct] && term[ct][a1] != -1)
			pickNode(a2, a1);
	}
	else		if (term[ct][a2] != color[ct] && term[ct][a2] != -1)
		pickNode(a2, a2);
}
void canDrop()
{
	if (visited[ct] == 0)
		terminalCheck();
	if (armCount == 2 || (arm[0] != color[ct] && arm[1] != color[ct]))
		return;
	else if (armCount == 0)
	{
		if (arm[0] == color[ct] && term[ct][0] == -1)
			nodeDrop(0, 0);
		else if (arm[1] == color[ct] && term[ct][1] == -1)
			nodeDrop(1, 1);
		else if (arm[0] == color[ct] && term[ct][1] == -1)
			nodeDrop(0, 1);
		else if (arm[1] == color[ct] && term[ct][0] == -1)
			nodeDrop(1, 0);
	}

	if (arm[0] == color[ct] || arm[1] == color[ct])
	{
		if (term[ct][0] != -1 && term[ct][1] != -1)
		{
			if (arm[0] == color[ct])
				dropPickup(0);
			else dropPickup(1);
		}
		if (arm[0] == color[ct] && term[ct][0] == -1)
			nodeDrop(0, 0);
		else if (arm[0] == color[ct] && term[ct][1] == -1)
			nodeDrop(0, 1);
		else if (arm[1] == color[ct] && term[ct][0] == -1)
			nodeDrop(1, 0);
		else if (arm[1] == color[ct] && term[ct][1] == -1)
			nodeDrop(1, 1);
	}
	else return;
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
		else	if (arm[0] != -1 && (sort[t1] == -1 && arm[1] != -1))
			sortDrop(1, t1);
		else if (arm[1] != -1)
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
	travel((t1 + 4) % 6, ct);
}
void sortCheck()
{
	int term1 = ct, a1, a2;
	if ((sort[ct % 2] == color[ct] || sort[ct % 2] == color[adj]))
	{
		if (armCount>0)
		{
			ct = indicator[sort[ct % 2]];
			if (arm[0] == -1)
				pickSort(0, ct % 2);
			else if (arm[1] == -1)
				pickSort(1, ct % 2);
			if (armCount != 1 && term[ct][0] != -1 && term[ct][1] != -1 && sort[ct % 2] == -1)
				if (arm[0] == color[ct])
					sortDrop(1, ct % 2);
				else if (arm[0] == color[ct])
					sortDrop(0, ct % 2);
		}
		else ct = adj;
	}
	else if (arm[0] != color[adj] && arm[1] != color[adj])
	{
		if (armCount == 0 || (armCount == 1 && sort[ct % 2] != -1) || ((armCount == 2 || armCount == 1) && (term[adj][0] == color[adj] || term[adj][0] == -1) && (term[adj][1] == color[adj] || term[adj][1] == -1)))
		{
			if (armCount>0)
			{
				if (sort[ct % 2] != -1)
					if (arm[0] == -1)
						pickSort(0, ct % 2);
					else if (arm[1] == -1)
						pickSort(1, ct % 2);
			}
			travel(ct % 2 + 4, (ct + 1) % 2 + 4);
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
			if (arm[0] == color[adj])
				sortDrop(1, ct % 2);
			else sortDrop(0, ct % 2);
		else if (armCount == 1 && (term[adj][0] == -1 || term[adj][1] == -1) && sort[ct % 2] != -1)
			if (arm[0] == -1)
				pickSort(0, ct % 2);
			else	pickSort(1, ct % 2);
			ct = adj;
	}
	adj = adjCount(ct);
	travel(term1 % 2 + 4, ct);
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
	color_sensor_scaling();
	setIndicatorAndColor();
	ct = 0; adj = 2;
	while (sorted<total)
	{
		canDrop();
		if (visitedCount == 3)
			predict();
		if (sorted == total)
			break;
		pickup();
		travel(ct, ct % 2 + 4);
		sortCheck();
	}
	return 0;
}