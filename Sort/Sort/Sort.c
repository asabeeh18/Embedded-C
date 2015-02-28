/*

* Team Id: 		10000
* Author List: 		Amiraj Dhawan, e-Yantra Team
* Filename: 		fibonacci.c
* Theme: 		Cargo Sorting – eYRC Specific
* Functions: 		print_fibonacci_series(int) , main()
* Global Variables:	NONE
*
*/

#include<stdio.h>

/*
* Function Name:	print_fibonacci_series
* Input:		num_elements -> integer which stores the number of elements of the fibonacci
*			series to be printed
* Output:		prints the first num_elements of the fibonacci series
* Logic:		The next element of the Fibonacci series is given by
*			next = current_element + prev_element. The code loops for num_elements and
*			prints out the next element
* Example Call:		print_fibonacci_series(10);
*
*/
#define F_CPU 14745600
//#include <avr/io.h>
//#include <avr/interrupt.h>
//#include <util/delay.h>


#include <math.h> //included to support power function
//#include "strong.c"
void onNode();
void open(unsigned char);
void sortMan();
void sortFree();

unsigned char Center_white_line;
unsigned char Right_white_line;
unsigned char Left_white_line;
//SOrt
unsigned int count = 0;

unsigned char swap[2] = { 0 };
unsigned char visited[5] = { 0, 0, 0, 0, 0 };
unsigned const char RED = 1, GREEN = 2, BLUE = 3, BLACK = 4, EMPTY = 0;
unsigned char term[5][2] = { { 0, 0 }, { 3, 1 }, { 4, 2 }, { 3, 1 }, { 2, 4 } };
unsigned char indi[5] = { 0, 1, 2, 3, 4 };  //index is color value is terminal
unsigned char schD[2];	//values: 0 1 or 2, access by CT%2
unsigned char schP[2];
int threshold = 1000;
unsigned char CT = 1;
int noOfSteps = 0;
int uTurnLiya = 0;
unsigned char arm[2] = { 0, 0 };
unsigned char adjCount = 0, farCount = 0;
unsigned char i;
int Degrees;
//
void printCost(char c[])
{
	printf("cost : %d  - %s U-turn:%d\n", noOfSteps, c, uTurnLiya);
}
//++++++===Servo __init__ ========++++++++
void set_color()
{
	Left_white_line = 3;//ADC_Conversion(3);	//Getting data of Left WL Sensor
	Center_white_line = 3;//ADC_Conversion(2);	//Getting data of Center WL Sensor
	Right_white_line = 3;//ADC_Conversion(1);	//Getting data of Right WL Sensor

}

/*
void correct()
{
unsigned int i=0;
Degrees=5;
for(;i<3;i++)
{
left(); //Left wheel backward, Right wheel forward
lcd_print(2,7,777,3);
angle_rotate(Degrees);
stop();
set_color();
if(Center_white_line>40)
return;
}
//normal
right();
while(Center_white_line<40)
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
if(Center_white_line>40 && (Left_white_line>40 || Right_white_line>40) )
{
forward();
//_delay_ms(4000);
return;
}

print_sensor(1,1,3);	//Prints value of White Line Sensor1
print_sensor(1,5,2);	//Prints Value of White Line Sensor2
print_sensor(1,9,1);	//Prints Value of White Line Sensor3

forward();
velocity(200,200);

}while(Center_white_line>0x28);

noNatak();
forwardJaa();
}
*/
void turnDelay()
{
	//forward();
	//_delay_ms(6000);
	printf("\nGoing Straight\n");

}

void nodeFront()
{
	noOfSteps++;
	printCost("Going Forward");
	//forwardJaa();

	//printf("\nGoing Straight\n");
}
void nodeRight()
{
	//turnDelay();
	//right();
	//angle_rotate(90);
	printf("\nRight Ghetla\n");
}
void nodeLeft()
{
	//turnDelay();
	//left();
	//angle_rotate(100);

	printf("\nLeft Ghetla\n");
}

void buzzer()
{

	//	buzzer_on();
	////_delay_ms(1000);
	//buzzer_off();
}
/*
void nodeInd()
{
lcd_print(2,1,0,1);

turnDelay();
noNatak();

right();
angle_rotate(90);
////_delay_ms(1000);
buzzer();

right();

angle_rotate(190);
////_delay_ms(1000);
buzzer();

right();
angle_rotate(80);
buzzer();
forward();
//_delay_ms(5000);
}
void onNode()
{
static unsigned int nodeCount=0;
nodeCount++;
if(nodeCount==1)
{
lcd_print(2,1,3,1);
forward();
//_delay_ms(5000);
}
else if(nodeCount==2 || nodeCount==3)
{
lcd_print(2,1,4,1);
nodeInd();
}
else if(nodeCount==4)
{
nodeLeft();
}
else if(nodeCount==5)
{
nodeRight();
}
else if(nodeCount==6)
{
stop();
buzzer_on();
while(1);
}
}

*/
void uTurn()
{
	//	buzzer();
	//	right();
	//	angle_rotate(190);
	//_delay_ms(1000);

	uTurnLiya++;
}
//bot will always face towards the inside of arena or away
//inside for now
//change it to facing outside
char adjC(unsigned int CT)
{

	if (CT == 1)
		return 3;
	if (CT == 2)
		return 4;
	if (CT == 3)
		return 1;
	if (CT == 4)
		return 2;
	else return 0;
}

//servo_1 states
//init servo at 90 deg
/*
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
*/
unsigned char pickup(unsigned char side) //1=swap 0=term
{
	if (arm[side] == 0)
	{
		if (indi[term[CT][side]] != CT && term[CT][side] != 0)
		{
			arm[side] = term[CT][side];
			term[CT][side] = 0;
			//pickupSeQ(side);
			return 1;
		}
	}
	else if (arm[!side] == 0)
	{
		if (indi[term[CT][side]] != CT && term[CT][side] != 0)
		{
			arm[!side] = term[CT][side];
			term[CT][side] = 0;
			uTurn();
			//	pickupSeQ(!side);
			uTurn();
			return 1;
		}
	}
	return 0;
}

unsigned char drop(unsigned char side)
{
	if (term[CT][side] == 0)
		if (indi[arm[side]] == CT)
		{
		term[CT][side] = arm[side];
		arm[side] = 0;
		//	dropSeQ(side);
		count++;
		return 1;
		}
		else if (indi[arm[!side]] == CT)
		{
			term[CT][side] = arm[!side];
			arm[!side] = 0;
			uTurn();
			//	dropSeQ(!side);
			uTurn();
			count++;
			return 1;
		}
	return 0;
}

int dropS(unsigned char side)	//side is arm since arena side can b calculated from CT
{
	if (swap[CT % 2] != 0)
		return 0;
	swap[CT % 2] = arm[side];
	arm[side] = 0;
	//dropSeQ(side);
	return 1;
}


int pickupS(unsigned char side)	//side is arm since arena side can b calculated from CT
{
	if (arm[side] == 0)
	{
		arm[side] = swap[CT % 2];
		swap[CT % 2] = 0;
		//	pickupSeQ(side);
		return 1;
	}
	else if (arm[!side] == 0)	//Uturn sequence follows
	{
		arm[!side] = swap[CT % 2];
		swap[CT % 2] = 0;
		//	pickupSeQ(!side);
		return 1;
	}
	return 0;
}

void swapMan(unsigned char sw, unsigned char nxTerm)		//make efficient
{
	unsigned char pSwap = ((CT == 2 || CT == 3) ? 1 : 0);
	if (CT == nxTerm)	//reached farther end
	{
		if (arm[0]>0 && arm[1]>0 && visited[CT] == 0) //based on the fact that a visited term atleast had 1 box taken from it
		{
			if (arm[0] == arm[1] || indi[arm[1]] == CT)
				dropS(0);
			else dropS(1);
		}
	}
	else if (CT % 2 != nxTerm % 2)	//going to far reached 1st swap
	{
		if (indi[swap[CT % 2]] == nxTerm || indi[swap[CT % 2]] == adjC(nxTerm))
		{
			pickupS(CT % 2);
		}
	}
	else if (CT % 2 == nxTerm % 2)	//to adj
	{
		if (arm[1] == 0 && arm[0] == 0)
		{
			pickupS(CT % 2);
		}
		else if (arm[1] == 0 || arm[0] == 0)
		{
			if (indi[arm[1]] == nxTerm || indi[arm[0]] == nxTerm && visited[nxTerm] == 1)
				pickupS(CT % 2);
			else if (!(indi[arm[1]] == nxTerm || indi[arm[0]] == nxTerm))
			{
				if (arm[0] > 0)
				{
					pickupS(1);
					dropS(0);
				}
				else
				{
					pickupS(0);
					dropS(1);
				}
			}
		}
		if (arm[1] > 0 && arm[0] > 0 && visited[nxTerm] == 0)
		{
			if (indi[arm[0]] == nxTerm)
				dropS(1);
			else dropS(0);
		}
	}
}
void travel(unsigned char nxTerm)
{

	//forwardJaa();
	noOfSteps++;
	printCost("Goint To Sort");
	//swapEncounterdAction
	swapMan(CT % 2, nxTerm);
	/*
	lcd_print(1,11,(CT==1 && (nxTerm == 3 || nxTerm== 4)),1);// ||
	lcd_print(1,12,(CT==4 && (nxTerm == 1 || nxTerm== 2)),1);
	lcd_print(1,13,(CT==2 && (nxTerm == 3 || nxTerm== 4)),1); // ||
	lcd_print(1,14,(CT==3 && (nxTerm == 1 || nxTerm== 2)),1);
	*/
	if ((CT == 1 && (nxTerm == 2 || nxTerm == 4)) || (CT == 4 && (nxTerm == 1 || nxTerm == 3)))
	{
		nodeLeft();
		noOfSteps += 2;
		noOfSteps += 2;

		printCost("Going Opposite Side");
		//forwardJaa();
		//_delay_ms(1000);
		//forwardJaa();
		CT = nxTerm;	//changed for drop,pickup
		swapMan(nxTerm % 2, nxTerm);
		if (nxTerm == 1 || nxTerm == 4)
			nodeRight();
		else
			nodeLeft();
	}
	else if ((CT == 2 && (nxTerm == 1 || nxTerm == 3)) || (CT == 3 && (nxTerm == 2 || nxTerm == 4)))
	{
		nodeRight();
		noOfSteps += 2;

		noOfSteps += 2;

		printCost("Going Opposite Side");
		//forwardJaa();
		//_delay_ms(500);
		//forwardJaa();
		CT = nxTerm;	//changed for drop,pickup
		swapMan(nxTerm % 2, nxTerm);
		if (nxTerm == 2 || nxTerm == 3)
			nodeLeft();
		else
			nodeRight();
	}
	else
	{
		//_delay_ms(1000);
	}
	//forwardJaa();
	noOfSteps++;
	CT = nxTerm;

	printCost("Going To Terminal");
	//stop();
	//stop();
	//buzzer();
}


void counter()
{
	char adj = adjC(CT); //v r inside d term wid one box of its color and an empty arm and MAYBE 1 box at sort
	adjCount = 0;
	farCount = 0;

	if (indi[term[CT][0]] == adj)
		adjCount++;

	else if (term[CT][0] != 0 && indi[term[CT][0]] != CT && indi[term[CT][0]] != adj)
		farCount++;

	if (indi[term[CT][1]] == adj)
		adjCount++;

	else if (term[CT][1] != 0 && indi[term[CT][1]] != CT && indi[term[CT][1]] != adj)
		farCount++;


}
unsigned char unvisited()
{
	for (i = 1; i <= 4; i++)
		if (visited[i] == 0)
			return i;
	return 0;
}


void gotoSort()
{
	//forwardJaa();
	noOfSteps++;

	printCost("Going From Terminal to Sort");
}

void goBack()
{
	/*
	if(Center_white_line>40)
	{
	back();
	set_color();
	while(Center_white_line>40 && (Left_white_line>40 || Right_white_line>40))
	{	return;}
	}
	else
	{
	noNatak();
	goBack();
	}

	*/


	noOfSteps++;

	printCost("Going from Sort to Terminal");
}

unsigned char checkForCompletion()
{
	int j = 0;
	for (i = 1; i < 5; i++)
	{
		if ((indi[term[i][0]] == i) && (indi[term[i][1]] == i))
		{
			if (visited[i] == 1)
				j += 2;
			else
				continue;
		}
		else
			return 0;
	}
	return j;
}

unsigned char checkForCompletion1()
{
	int j = 0;
	for (i = 1; i < 5; i++)
		if ((term[i][0] == 0 || indi[term[i][0]] == i) && (term[i][1] == 0 || indi[term[i][1]] == i))
			j += 2;
		else
			return 0;
	return j;
}
//SORTMAN
//MAIN SORTING FUNCTION
//R-0 L-1
/*
int colorCalc()
{
red_read();
blue_read();
green_read();
if(!(red>threshold && green>threshold && blue>threshold))
return BLACK;
else
{
if(red>blue)
{
if(red>green)
return RED;
else
return GREEN;
}
else if(blue>green)
return BLUE;
else return GREEN;
}
}



int pick_color(unsigned char node)	//0 or 1
{
//read_color();
unsigned char color;
if(node=0)
nodeRight();
else
nodeLeft();
color=colorCalc();
if(node=1)
nodeRight();
else
nodeLeft();
return color;
}*/
void termPick()
{
	//pick_color(0);
	//lcd_print(2,1,term[CT][0],1);
	//pick_color(1);
	visited[CT] = 1;
}
void sortMan()
{
	unsigned char nxTerm = 0;
	unsigned char adj = adjC(CT);
	if (visited[CT] == 0)
		termPick();
	counter();
	unsigned char pSwap = ((CT == 2 || CT == 3) ? 1 : 0);
	unsigned char pFar = (indi[term[CT][0]] == adj | indi[term[CT][0]] == CT | indi[term[CT][0]] == 0);		//wont work for empty node
	if (indi[swap[CT % 2]] == CT)
	{
		if (farCount == 2)
		{
			if (term[CT][0] == term[CT][1])
			{
				nxTerm = indi[term[CT][0]]; //the common terminal
				pickup(0);
				drop(0);
				gotoSort();
				pickupS(1);
				dropS(0);
				goBack();
				pickup(1);
				drop(1);
			}
			else	//both diff of far
			{
				nxTerm = indi[term[CT][0]];
				pickup(0);
				drop(0);
				gotoSort();
				pickupS(0);
				dropS(1);
				goBack();
				pickup(1);
				drop(1);
				//SchPickupAtSort	//CT
				//SchDropAtSort		//far
			}
		}
		else if (farCount == 1)
		{
			if (adjCount == 1)
			{
				nxTerm = adj;
				pickup(pFar);
				drop(pFar);
				gotoSort();
				pickupS(pSwap);
				dropS(pSwap);
				goBack();
				pickup(!pFar);
				drop(!pFar);
				//nothing to schedule
			}
			else
			{	//empty node
				//:O :O
				if (visited[adj] == 0 || term[adj][0] != indi[adj] || term[adj][1] != indi[adj])
					nxTerm = adj;

				else	//seq also usable for both empty
				{
					nxTerm = indi[term[CT][pFar]];
					pickup(pFar);
					drop(pFar);
					gotoSort();
					pickupS(pSwap);
					goBack();
					drop(!pFar);
				}
			}
		}
		else if (adjCount == 1)
		{
			if (farCount == 1)
			{
				nxTerm = adj;
				pickup(pFar);
				drop(pFar);
				gotoSort();
				pickupS(pSwap);
				dropS(pSwap);
				goBack();
				pickup(!pFar);
				drop(!pFar);
				//nothing to schedule
			}
			else
			{
				//empty node
				drop(pFar);
				gotoSort();
				pickupS(pSwap);
				goBack();
				pickup(!pFar);
				drop(!pFar);
				nxTerm = adj;
			}
		}
		else if (adjCount == 2)
		{
			nxTerm = adj;
			pickup(0);
			drop(0);
			gotoSort();
			pickupS(pSwap);
			dropS(pSwap);
			goBack();
			pickup(1);
			drop(1);
		}
		if (arm[0] == 0 && arm[1] == 0)
		{
			travel(nxTerm);
			sortFree();
		}
		else
		{
			travel(nxTerm);
			sortMan();
		}

	}


	else if (indi[swap[CT % 2]] != CT)	//Sort can be empty
	{
		if (farCount == 2)
		{
			if (term[CT][0] == term[CT][1])	//and sort not empty why ?? :/
			{
				nxTerm = indi[term[CT][0]]; //the common terminal
				//SchPickupAtSort
				//SchDropAtSort
				pickup(0);
				drop(0);
				pickup(1);
				drop(1);
			}
			else if (swap[CT % 2] == term[CT][0])
			{
				pickup(0);
				drop(0);
				nxTerm = indi[arm[0]];
			}
			else if (swap[CT % 2] == term[CT][1])
			{
				pickup(1);
				drop(1);
				nxTerm = indi[arm[1]];
			}
			else
			{
				if (visited[indi[term[CT][0]]] == 1)
				{
					nxTerm = indi[term[CT][0]];
				}
				else
					nxTerm = indi[term[CT][1]];
				pickup(0);
				drop(0);
				pickup(1);
			}

		}
		else if (farCount == 1)
		{
			if (adjCount == 1)
			{
				if (swap[CT % 2] == 0)
				{
					pickup(!pFar); //adjwala
					drop(!pFar);
					pickup(pFar);
					nxTerm = adj;
					//SchDropAtSort
				}
				else
				{
					//Sort of far and CT has 1 far a adj
					pickup(!pFar);
					drop(!pFar);
					pickup(pFar);
					nxTerm = adj;
				}
			}
			else  //====
			{	//one node is empty
				if (visited[adj] == 1)
					nxTerm = indi[term[CT][pFar]];
				else nxTerm = adj;
				pickup(pFar);
				drop(pFar);
				//SchDropAtSort
			}
		}

		else if (adjCount == 1)
		{
			if (farCount == 1)
			{
				if (swap[CT % 2] == 0)
				{
					pickup(!pFar); //adjwala
					drop(!pFar);
					pickup(pFar);
					nxTerm = adj;
					//SchDropAtSort
				}
				else
				{
					//Sort of far and CT has 1 far a adj
					pickup(!pFar);
					drop(!pFar);
					nxTerm = adj;
				}
			}
			else	//1 empty 1 adj
			{
				pickup(!pFar);
				drop(!pFar);
				nxTerm = adj;
			}
		}
		else if (adjCount == 2)
		{
			if (swap[CT % 2] > 0) //smthin is at Sort
			{
				pickup(0);
				drop(0);
			}
			else
			{
				pickup(0);
				drop(0);
				pickup(1);
			}
			nxTerm = adj;
		}
		else	//empty term nore precisely we arent pickiing up anything so we have NO CLUE WHERE TO GO
		{
			drop(0);
			drop(1);
			if (visited[1] + visited[2] + visited[3] + visited[4] > 3 && checkForCompletion() >= 6)
			{
				if (nxTerm == 0 && swap[0] == 0 && swap[1] == 0 && arm[0] == 0 && arm[1] == 0)	//panic or complete
					while (1);
			}
			else
			{
				if (visited[adj] == 0)
					nxTerm = adj;
				else
				{
					if (arm[0] > 0)
						nxTerm = indi[arm[0]];
					else if (arm[1] > 0)
						nxTerm = indi[arm[1]];
					else if (swap[CT % 2]>0)
						nxTerm = indi[swap[CT % 2]];
					else if (swap[(CT + 1) % 2] > 0)
						nxTerm = indi[swap[(CT + 1) % 2]];

					else
						nxTerm = unvisited();
				}

			}
			if (nxTerm == 0)
			{
				while (1);
			}
		}
	}
	if (arm[0] == 0 && arm[1] == 0)
	{
		travel(nxTerm);
		sortFree();
	}
	else
	{
		travel(nxTerm);
		sortMan();
	}
}
void sortFree()

{
	unsigned char adj = adjC(CT);
	unsigned char pFar = (indi[term[CT][0]] == adj | indi[term[CT][0]] == CT | indi[term[CT][0]] == 0);		//wont work for empty node
	unsigned char nxTerm = 0;
	if (visited[CT] == 0)
		termPick();
	counter();
	pickup(0);
	pickup(1);
	if (farCount == 2)
	{
		nxTerm = indi[arm[0]];
		//SchDropAtSort(nxTerm);
	}
	else if (adjCount == 1)
	{
		if (farCount == 1)
		{
			nxTerm = adj;
			//SchDropAtSort(nxTerm);
		}
		else	//1 empty block
		{
			nxTerm = adj;
		}
	}
	else if (farCount == 1)
	{
		if (adjCount == 1)
		{
			nxTerm = adj;
			//SchDropAtSort(nxTerm);
		}
		else	//1 empty block
		{
			if (visited[adj] == 0)
			{
				nxTerm = adj;
				//SchDropAtSort(nxTerm);
			}
			else
				nxTerm = indi[arm[pFar]];

		}
	}
	else if (adjCount == 2)
	{
		nxTerm = adj;
		//SchDropAtSort(nxTerm);
	}
	else	//empty term
	{
		if (visited[1] + visited[2] + visited[3] + visited[4] >= 3 && checkForCompletion() >= 6)
		{
			if (nxTerm == 0 && swap[0] == 0 && swap[1] == 0 && arm[0] == 0 && arm[1] == 0)	//panic or complete
				while (1);
		}
		else
		{
			if (visited[adj] == 0)
				nxTerm = adj;
			else
			{
				if (arm[0] > 0)
					nxTerm = indi[arm[0]];
				else if (arm[1] > 0)
					nxTerm = indi[arm[1]];
				else if (swap[CT % 2]>0)
					nxTerm = indi[swap[CT % 2]];
				else if (swap[(CT + 1) % 2] > 0)
					nxTerm = indi[swap[(CT + 1) % 2]];

				else
					nxTerm = unvisited();
			}

		}
		if (nxTerm == 0)
		{
			while (1);
		}
	}
	if (arm[0] == 0 && arm[1] == 0)
	{
		travel(nxTerm);
		sortFree();
	}
	else
	{
		travel(nxTerm);
		sortMan();
	}
}
void init_sort()
{
	sortFree();
}

//Main Function
int main()
{
	i = 0;
	/*
	printf("Enter Terminal Colors\n");

	for(i;i<4;i++)
	{
	printf("T%d : ",i);
	scanf("%d %d",&terminalColors[i][0],&terminalColors[i][1]);
	printf("\n");
	}
	*/


	noOfSteps = 4;	// Initial Cost Till BlackHole

	printCost("Initial Cost Till BlackHole");
	noOfSteps = noOfSteps + 2 + 1;
	printCost("Going to Terminal One");


	init_sort();

	if (checkForCompletion1())
	{
		printf("Sorted");
	}
	else  printf("Not Sorted");
	while (1);
	return 1;
}
