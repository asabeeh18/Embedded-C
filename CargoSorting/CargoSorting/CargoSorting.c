


#define F_CPU 14745600
#include "strong.c"
int indicator[4];//indicator[0] contains the terminal no. associated vid red color;this vil be used in scan();
int color[4];//color[0] contains the color no. associated vid terminal 0;
int term[4][2];//initialise to -2 COntians the colour values at the terminals
int visited[4];//initialise to 0; when terminal visted make it 1;
int sort[2];//initialise to -1;
//ct:Current Terminal
//adj:the adjacent terminal
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
//Velocitues
const int turn_v=200,line_v=255,correct_v=200,op_v=240;

//threshold for black colour
int threshold;

//White line sensor values
unsigned char Left_white_line = 0;
unsigned char Center_white_line = 0;
unsigned char Right_white_line = 0;


//***UTILITY


//******************COLOR****************************
/*
*
* Function Name: 	set_color
* Input: 			void 
* Output: 			Take the values from  Left,Right  and Centre White Line Sensors and Store it in respective variables
* Logic: 			Uses the ADC_Conversion function to convert the the channel 1,2,3 to digital values
* Example Call:		set_color()
*
*/
void set_color()
{
	Left_white_line = ADC_Conversion(3);	//Getting data of Left WL Sensor
	Center_white_line = ADC_Conversion(2);	//Getting data of Center WL Sensor
	Right_white_line = ADC_Conversion(1);	//Getting data of Right WL Sensor
}

/*
*
* Function Name: 	scan
* Input: 			void
* Output: 			Take the values from  Left,Right  and Centre White Line Sensors and Store it in respective variables
* Logic: 			Uses the ADC_Conversion function to convert the the channel 1,2,3 to digital values
* Example Call:		color_value=scan()
*
*/
int scan()//return the color no.
{
	//read the red,green and blue pulses
	red_read();
	blue_read();
	green_read();
	//If SHARP sensor detects no box
	if(!(ADC_Conversion(11)>55 && ADC_Conversion(11)<100))
	{
		return EMPTY;
	}
	//IF all the pulse cpunts are less than the threshold then that means it is black color
	if (red<threshold && green < threshold && blue < threshold)
	{
		return BLACK;
	}
	//find the colour with max pulse value and return it
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
/*
*
* Function Name: 	lower
* Input: 			unsigned char side 
* Output: 			Lowers the arm to the specified side
* Logic: 			Positions the angle of the servo to lower it down
* Example Call:		lower(1)
*
*/
void lower(unsigned char side)
{
	if(side==1)
	{
		servo_2(55);
	}
	else
	servo_2(120);
	_delay_ms(500);
}

/*
*
* Function Name: 	elevate
* Input: 			void
* Output: 			Positions the arm to its original position
* Logic: 			Sets servo 2 at 90 degrees to position the arm back
* Example Call:		elevate()
*
*/
void elevate()
{
	servo_2(90);
	_delay_ms(500);
	servo_2_free();
	_delay_ms(500);
}

/*
*
* Function Name: 	open
* Input: 			void
* Output: 			Opens the gripper from the specified side
* Logic: 			Opens the gripper by giving some angle to the servo 1 or 3 as needed
* Example Call:		open()
*
*/
void open(unsigned char side)
{
	if (side == 0)
	{
		servo_3(90);
		_delay_ms(500);
		servo_3_free();
		_delay_ms(500);
	}
	else
	{
		servo_1(0);
		_delay_ms(500);
		servo_1_free();
		_delay_ms(500);
	}
}

/*
*
* Function Name: 	close
* Input: 			void
* Output: 			Closes the gripper from the specified side
* Logic: 			Closes the gripper by giving some angle to the servo 1 or 3 as needed
* Example Call:		close()
*
*/
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

/*
*
* Function Name: 	node
* Input: 			void
* Output: 			Moves the robot forward by 70mm
* Logic: 			Move the robot forward to take proper turns
* Example Call:		node()
*
*/
void node()
{
	velocity(turn_v,turn_v);
	forward_mm(70);
	stop();
}

/*
*
* Function Name: 	turnRight
* Input: 			void
* Output: 			Turns the robot right by 90 degrees
* Logic: 			Turns the robot right 90 degrees by either calling right() and then waiting for a black line OR
					by using the position encoder function right_degrees(Degrees)
* Example Call:		turnRight()
*
*/

void turnRight()	//turns the robot right
{
	if ((dir == 3 && (ot == 0 || ot == 1)) || (dir == 1 && (ot == 2 || ot == 3)))
	{
		/*
		This block is used when the robot is at the node of the terminal and is facing either of the terminal boxes and it need to turn right
		which will make it face outside the Flex Sheet where no black line exist to align the robot
		*/
		velocity(turn_v, turn_v);
		right_degrees(90);
	}
	else
	{
		//Turn right till Centre WHite line sensor detects a black line
		velocity(turn_v, turn_v);
		right_degrees(30);
		
		while (ADC_Conversion(2)<50)
			right();
		stop();	
	}
	dir = (dir + 1) % 4;
}

/*
*
* Function Name: 	turnLeft
* Input: 			void
* Output: 			Turns the robot left by 90 degrees
* Logic: 			Turns the robot left 90 degrees by either calling left() and then waiting for a black line OR
					by using the position encoder function left_degrees(Degrees)
* Example Call:		turnLeft()
*
*/
void turnLeft()	//turns the robot left
{
	
	if ((dir == 1 && (ot == 0 || ot == 1)) || (dir == 3 && (ot == 2 || ot == 3)))
	{
		/*
		This block is used when the robot is at the node of the terminal and is facing either of the terminal boxes and it need to turn left
		which will make it face outside the Flex Sheet where no black line exist to align the robot
		*/
		velocity(turn_v, turn_v);
		left_degrees(90);
	}
	else
	{
		//Turn left till Centre WHite line sensor detects a black line
		left_degrees(30);
		velocity(turn_v,turn_v);
		while (ADC_Conversion(3)<40)
			left();
		stop();
	}
	dir = (dir + 3) % 4;
}

/*
*
* Function Name: 	turn
* Input: 			void
* Output: 			Turns the robot 180 degrees
* Logic: 			Turns the robot 180 degrees by either calling left_degrees(160) and then left() and then waiting for a black line OR
					by using the position encoder function left_degrees(Degrees)
* Example Call:		turn()
*
*/
void turn()	//turn robo by 180 degree
{
	if (dir == 0 && (ot == 2 || ot == 3))
	{
		velocity(turn_v, turn_v);
		left_degrees(190);
	}
	else if (dir == 2 && (ot == 0 || ot == 1))
	{
		velocity(turn_v, turn_v);
		left_degrees(190);
	}
	else
	{
		velocity(turn_v, turn_v);
		left_degrees(160);
		while (ADC_Conversion(3)<47)
			left();
		_delay_ms(50);
		stop();
	}
	dir = (dir + 2) % 4;
	flag1=0;
}

//**********END TURNS****************

//**********BlackLine Follower **************

/*
* Function Name: 	Delay
* Input:			tim -> provides total delay time in milliseconds
* Output: 			1 -> If center white line sensor detects black before 'tim' delay is over
*					0 -> If center white line sensor does not detect black before 'tim' delay is over
* Logic:			The function provides delay of 'tim' milliseconds in steps of one.
*					After each step, the function will check center white line input.
*					If sensor detects black, exit from for loop & return 1; else 0.
* Example Call:		char x = Delay(40);
*/
char Delay(int tim)
{
	int i;
	//Loop tim times and also keep checking for Black under the White Line Sensor
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

/*
*
* Function Name: 	semiCorrect_Indi
* Input: 			void
* Output: 			Properly positions the robot on the white line if Right or Left Line Sensors  detect black under them 
* Logic: 			If left Sensor detects black then move the robot left till Center_white_line sensor 
					detects black; and Right_white_line and Left_white_line detect white
					Similarly move the robot right if the Right_white_line sensor detects Black
* Example Call:		semiCorrect_Indi()
					
*
*/
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
			//Keep moving left till Center_white_line sensor is not Black
			while((Center_white_line<0x28))// && Left_white_line<40 && Right_white_line<40))
			{
				//lcd_print(1,2,1,1);
				left();
				set_color();
			}
		}
		else if(Right_white_line>40 && Left_white_line<40)	//wwb
		{
			
			//Keep moving right till Center_white_line sensor is not Black
			while((Center_white_line<0x28))// && Left_white_line<40 && Right_white_line<40))
			{
				right();
				set_color();
			}
			
		}
	}
	
}

/*
*
* Function Name: 	correct_Indi
* Input: 			void
* Output: 			Properly positions the robot on the white line if All white line sensors detect white
* Logic: 			Sweep the robot left and right until black is detected under any sensor.
					Increase the sweep area after each unsuccessful sweep
* Example Call:		correct_Indi()
					
*
*/
void correct_Indi()
{
	/*
	*
	i: gives the number of milliseconds a sweep is to be done
	possible values of i are n*50,Where n can be 1,2,3.... incrementing after each unsuccessful sweep
	*
	*/
	unsigned int i=50;
	stop();
	//lcd("cor");
	while(1)
	{
		//After many tries it was observed that the robot has a natural tendency to get deviated to the right more than to the left
		//And hence the sweep starts by first moving to the left and then the subsequent  actions are taken
		left();
		//sweep to left for 'i' ms return if Black is detected under Center_white_line sensor
		if(Delay(i))
			return;
		stop();
		//set_color();
		//call set_color to correct the robot if left or right line sensor are detecting black
		semiCorrect_Indi();
		if(ADC_Conversion(2)>40)// && ADC_Conversion(1)<40 && ADC_Conversion(3)<40)
			break;
		i+=50;
		//sweep to right and follow same logic as above
		right();
		if(Delay(i))
			return;
		stop();
		semiCorrect_Indi();
		if(ADC_Conversion(2)>40)// && ADC_Conversion(1)<40 && ADC_Conversion(3)<40)
			break;
		
		//If black is not found by any of the sensors increment the sweep area by 50ms and restart
		i+=50;
	
	}
	//lcd("-");
	stop();
	return;
}

/*
*
* Function Name: 	lineFix_Indi
* Input: 			void
* Output: 			Properly positions the robot on the white line if Right or Left Line Sensors  detect black under them.
					or calls correct_Indi if no condition satisfies or All sensors detect white under them
* Logic: 			Move the robot Left or right with respect to whether 
* Example Call:		lineFix_Indi()
					
*
*/
void lineFix_Indi()
{
	//flag: To check if the robot has been positioned properly or not ,possible values 0 or 1
	int flag=0;
	//reduce the velocity to ease the correct positioning of the robot
	velocity(correct_v,correct_v);
	if(Center_white_line<40)
	{
		if(Left_white_line>40 && Right_white_line<40) //bww
		{
			flag=1;
			//Keep moving left till Center_white_line sensor is not Black and Left_white_line and Right_white_line is not white
			while(!(Center_white_line>0x28 && Left_white_line<40 && Right_white_line<40))
			{
				left();
				set_color();
			}
		}
		else if(Right_white_line>40 && Left_white_line<40)	//wwb
		{
			flag=1;
			//Keep moving right till Center_white_line sensor is not Black and Left_white_line and Right_white_line is not white
			while(!(Center_white_line>0x28 && Left_white_line<40 && Right_white_line<40))
			{
				right();
				set_color();
			}
		}
		else
		{
			//No condition was satisfied call correct_Indi in such a case
			flag=1;
			correct_Indi();
		}
	}
	else
	{
		//If Center_white_line detects black and any of the either Right_white_line or Left_white_line also detect black then a node is found hence return
		
		if(Left_white_line>40 && Right_white_line<40)	//bbw
		{
			flag=1;
			return;
		}
		else if(Left_white_line<40 && Right_white_line>40)	//wbb
		{
			flag=1;
			return;
		}
		else if(Left_white_line>40 && Right_white_line>40)
		{
			flag=1;
			return;
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
	return;
}

/*
*
* Function Name: 	keepMoving_Indi
* Input: 			void
* Output: 			Move Forward if the robot moves out of line call lineFix_Indi to fix the position
* Logic: 			To check if the robot has moved out of line this condition will return a false value
					(Center_white_line>0x28 && Left_white_line<40 && Right_white_line<40)
* Example Call:		keepMoving_Indi()
					
*
*/
void keepMoving_Indi()
{
	do
	{
		forward();
		//_delay_ms(200);
		set_color();
		if(Center_white_line>40 && (Left_white_line>40 || Right_white_line>40)) //2 bbw wbb
		{
			//node detected ,return
			return;
		}
		velocity(255,255);
		//velocity(v+vi,v+vi);
		//i+=20;
	}while(Center_white_line>0x28 && Left_white_line<40 && Right_white_line<40);	//wbw
	//Robot deviated out of path call lineFix_Indi
	lineFix_Indi();
	keepMoving_Indi();
	return;
}
//***********END INDICATOR****************

//**************SORT AREA*******************


/*
*
* Function Name: 	semiCorrect
* Input: 			void
* Output: 			Properly positions the robot on the white line if Right or Left Line Sensors  detect black under them 
					return 1 if a node is detected.
* Logic: 			If left Sensor detects black then move the robot left till Center_white_line sensor 
					detects black; and Right_white_line and Left_white_line detect white
					Similarly move the robot right if the Right_white_line sensor detects Black
* Example Call:		flag=semiCorrect()
					
*
*/
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

/*
*
* Function Name: 	correct
* Input: 			void
* Output: 			Properly positions the robot on the white line if All white line sensors detect white
					Return 1 if a node is detected
* Logic: 			Sweep the robot left and right until black is detected under any sensor.
					Increase the sweep area after each unsuccessful sweep
					
* Example Call:		flag=correct()
					
*
*/
char correct()
{
	unsigned int i=50;
	stop();
	
		while(1)
		{
			//After many tries it was observed that the robot has a natural tendency to get deviated to the right more than to the left
			//And hence the sweep starts by first moving to the left and then the subsequent  actions are taken
			
			left();
			//sweep to left for 'i' ms return if Black is detected under Center_white_line sensor
			if(Delay(i))
				return 0;
			stop();
			//set_color();
			//call set_color to correct the robot if left or right line sensor are detecting black
			
			//set_color();
			if(semiCorrect())
				return 1;
			if(ADC_Conversion(2)>40)// && ADC_Conversion(1)<40 && ADC_Conversion(3)<40)
				break;
			i+=50;
			//sweep to right and follow same logic as above
			right();
			if(Delay(i))
				return 0;
			stop();
			
			if(semiCorrect())
				return 1;
			if(ADC_Conversion(2)>40)// && ADC_Conversion(1)<40 && ADC_Conversion(3)<40)
				break;
				//If black is not found by any of the sensors increment the sweep area by 50ms and restart
			i+=50;
		
	}
	//lcd("-");
	stop();
	return 0;
}

/*
*
* Function Name: 	lineFix
* Input: 			void
* Output: 			Properly positions the robot on the white line if Right or Left Line Sensors  detect black under them.
					or calls correct if no condition satisfies or All sensors detect white under them
					Calls node() if a node is detected.
					This situation arises for two possible scenarios:
					*						1) Robot deviates from Black Line
					*						2) Node / Intersection is Reached
* Logic: 			Move the robot Left or right with respect to whether 
* Example Call:		flag=lineFix()
					
*
*/
int lineFix()
{
	//flag: To check if the robot has been positioned properly or not ,possible values 0 or 1
	int flag=0;
	//reduce the velocity to ease the correct positioning of the robot
	velocity(correct_v,correct_v);
	if(Center_white_line<40)
	{
		if(Left_white_line>40 && Right_white_line<40) //bww
		{
			flag=1;
			//Keep moving left till Center_white_line sensor is not Black and Left_white_line and Right_white_line is not white
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
			//Keep moving right till Center_white_line sensor is not Black and Left_white_line and Right_white_line is not white
			while(!(Center_white_line>0x28 && Left_white_line<40 && Right_white_line<40))
			{
				right();
				set_color();
			}
			//lcd("-");
			
		}
		else
		{
			//No condition was satisfied call correct in such a case
			flag=1;
			if(correct())
				return 1;
		}
	}
	else
	{
		/*
		If Center_white_line detects black and any of the either Right_white_line or Left_white_line also detect black then a node is found
		hence call node() and then return
		*/
		if(Left_white_line>40 && Right_white_line<40)	//bbw
		{
			flag=1;
			node();
			return 1;

		}
		else if(Left_white_line<40 && Right_white_line>40)	//wbb
		{
			flag=1;
			node();
			return 1;
		}
		else
		{
			flag=1;
			
			if(correct())	//if 1 is return then that means a node is detected return 1 to signify that a node was detected
			return 1;
		}
	}
	if(flag==0)	
		if(correct())	//if 1 is return then that means a node is detected return 1 to signify that a node was detected
		return 1;
	stop();
	//lcd("--");
	//lcd_print(2,1,6,1);
	//buzzer_off();
	return 0;
}

/*
*
* Function Name: 	keepMoving
* Input: 			void
* Output: 			Move Forward if the robot moves out of line call lineFix_Indi to fix the position
* Logic: 			To check if the robot has moved out of line this condition will return a false value
					(Center_white_line>0x28 && Left_white_line<40 && Right_white_line<40)
* Example Call:		keepMoving()
					
*
*/
void keepMoving()
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
		velocity(255,255);
		//velocity(v+vi,v+vi);
		//i+=20;
	}while(Center_white_line>0x28 && Left_white_line<40 && Right_white_line<40);	//wbw
	
	if(lineFix())	//if 1 is return then that means a node is detected return to signify that a node was detected
		return;
	keepMoving();
	return;
}
void front()
{
	keepMoving();
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
	if(flag1==0)
		flag=1;
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
			if (ct == 3 || ct == 0)
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
	//_delay_ms(100);
	stop();
	//lcd_print(1,1,9,1);
	
	term[ct][0] = scan();
/*	for(i=0;i<=term[ct][0];i++)
	{
		buzzer_on();
		_delay_ms(100);
		buzzer_off();
		_delay_ms(500);
	}
*/	
	if(ct==0)
		term[ct][0]=1;
	if(ct==1)
		term[ct][0]=2;
	if(ct==3)
		term[ct][0]=-1;
	if(ct==2)
		term[ct][0]=2;
	if(term[ct][0]==color[ct])
 	{
 		buzzer_on();
 		_delay_ms(500);
 		buzzer_off();
 	}
	 if (term[ct][0] == -1 || term[ct][0] == color[ct])
	 total--;
//  	if(term[ct][0]==-1)
//  		buzzer();

	//_delay_ms(1000);
	//printf("Enter term[%d][%d]\n", ct, 0);
	//scanf("%d", &term[ct][0]);

	
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
		while (ADC_Conversion(2)<50)
		left();
		_delay_ms(140);
		stop();
		dir=(dir+3)%4;
	}
	else if (((ct == 0 || ct == 1) && dir == 2) || ((ct == 2 || ct == 3) && dir == 0))
		{
			right_degrees(30);
			velocity(turn_v, turn_v);
			while (ADC_Conversion(2)<50)
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
		//_delay_ms(100);
		stop();
		dir=(dir+2)%4;
		//lcd((char *)dir);
		}
	//_delay_ms(400);
	//printf("Enter term[%d][%d]\n", ct, 1);
	//scanf("%d", &term[ct][1]);
// 	left();
// 	_delay_ms(100);
	stop();
	term[ct][1] = scan();
	if(ct==0)
		term[ct][1]=1;
	if(ct==1)
		term[ct][1]=0;
	if(ct==3)
		term[ct][1]=-1;
	if(ct==2)
		term[ct][1]=0;
	/*for(i=0;i<=term[ct][1];i++)
	{
		buzzer_on();
		_delay_ms(100);
		buzzer_off();
		_delay_ms(500);
	}*/
 	if(term[ct][1]==color[ct])
 	{
		buzzer_on();
		_delay_ms(500);
 		buzzer_off();
	}
	if (term[ct][1] == -1 || term[ct][1] == color[ct])
		total--;
	//_delay_ms(1000);
	
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
			back_mm(65);
			flag=0;
			flag1=1;
		}	
		if(flag1==0 && (dir==0 || dir==2))
		{
			back_mm(90);
			flag=0;
			flag1=1;
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
 	armCount++;
}
void nodeDrop(int armNo, int side)
{
	term[ct][side] = arm[armNo];
	position(armNo, side);
	drop(armNo);
	arm[armNo] = -1;
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

//**************END SORT LOGIC*******************

//**************INITIALISATION*******************

/*
*
* Function Name: 	keepMoving
* Input: 			void
* Output: 			Move Forward if the robot moves out of line call lineFix_Indi to fix the position
* Logic: 			To check if the robot has moved out of line this condition will return a false value
					(Center_white_line>0x28 && Left_white_line<40 && Right_white_line<40)
* Example Call:		keepMoving()
					
*
*/
void __init__()
{
	init_devices();
	color_sensor_scaling();
	
	threshold=7000;
	//set servo at initial positions;Grippers Close and arm up
	servo_1(80);
	servo_2(90);
	servo_3(0);
	_delay_ms(200);
	//Free Servo to save Battery Power
 	servo_3_free();
 	servo_2_free();
 	servo_1_free();
	//Set the current terminal adjacent terminal direction and other terminal respectively
	ct = 3;
	adj = 1;
	dir = 0;
	ot = 3;
}
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
	color[0]=0;
	color[1]=2;
	color[2]=0;
	color[3]=1;
	for (i = 0; i<4; i++)
		indicator[color[i]] = i;
	sort[0] = sort[1] = -1;
	arm[0] = arm[1] = -1;
}

/*
*
* Function Name: 	indicator_set
* Input: 			void
* Output: 			Store indicator values and move the robot to intial terminal
* Logic: 			uses KeepMoving_Indi to move forward and scan function to store the colour values
* Example Call:		indicator_set()
					
*
*/
void indicator_set()
{
	keepMoving_Indi();

	//turn efficiency
	//move forward for a proper turn
	velocity(op_v,op_v);
	forward_mm(160);
	
	//scan 4
	//turn and scan the indicator block and store its value
	velocity(op_v,op_v);
	soft_right_2();
	_delay_ms(700);
	while(ADC_Conversion(2)<40);
	stop();
	color[3]=scan();
	
	//back to line
	soft_left();
	_delay_ms(1000);
	while(ADC_Conversion(2)<40);
	stop();
	
	//scan 3
	//turn and scan the indicator block and store its value
	soft_left_2();
	_delay_ms(500);
	while(ADC_Conversion(2)<40);
		stop();
	color[2]=scan();
	
	//back to line
	soft_right();
	_delay_ms(300);
	while(ADC_Conversion(2)<40);
	stop();
	
	//=====
	
	keepMoving_Indi();
	velocity(op_v,op_v);
	//turn efficiency
	//move forward for a proper turn
	forward_mm(160);
	
	//scan 2
	//turn and scan the indicator block and store its value
	//turn and scan the indicator block and store its value
	soft_right_2();
	_delay_ms(700);
	while(ADC_Conversion(3)<40);
	stop();
	color[1]=scan();
	
	//back to line
	soft_left();
	_delay_ms(1000);
	while(ADC_Conversion(2)<40);
	stop();
	
	//scan 1
	//turn and scan the indicator block and store its value
	soft_left_2();
	_delay_ms(500);
	while(ADC_Conversion(2)<40);
	stop();
	color[0]=scan();
	
	//back to line
	soft_right();
	_delay_ms(500);
	while(ADC_Conversion(2)<40);
	stop();
	
	//All indicaor blocks scanned go to initial terminal
	keepMoving();
	forward();
	_delay_ms(70);
	velocity(op_v,op_v);
	turnRight();
	forward();
	_delay_ms(200);
	keepMoving();
	velocity(op_v,op_v);
	forward();
	_delay_ms(50);
	turnRight();
	while(ADC_Conversion(2)<40)
		right();
	stop();
	
	
}


int main(void)
{
	//intialise EVERYTHING
	__init__();
	
	//move forward a little to escape the big start box
	forward();
	_delay_ms(500);
	//call indicator_set to initialise indicator values and reach the intial terminal
	indicator_set();
	//initialise terminals 
	setIndicatorAndColor();
	keepMoving();
	//reached the target terminal
	while (sorted<total)
	{
		canDrop();
		if (sorted == total)
		{
			break;
		}
		
		pickup();

		traverseToSort(ct, ct % 2 + 4);
		sortCheck();
	}
	//Task completed
	buzzer_on();
	_delay_ms(5000);
	buzzer_off();
	stop();
	while(1);
}