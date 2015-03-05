/*
*
* Team Id: eYRC#1348-CA
* Author List: Saurabh Parkhedkar, Rishikesh Madan, Nilesh Modak, Rajanikant Tenguria,
* Filename: final.c
* Theme: Cargo Alignment
* Functions: arm, CurPos, exit1, exit2, IRCorridor, left_wls, lf, lift, main, rf, right_wls, Sharp_Detection  SharpLeft, SharpRight, SharpSide, twist
* Global Variables: ShaftCountLeft, ShaftCountRight, Degrees, ADC_Conversion, ADC_Value, Left_white_line, Center_white_line, Right_white_line, Left_Sharp_Sensor, Right_Sharp_Sensor, distance, adc_reading, Sensor_read, BATT_Voltage, BATT_V, Blk_Pos[7][5], Cur_Pos[2];
*/

//#include "predef.c"


/*
*
* Function Name: Sharp_Detection
* Input: void
* Output: void
* Logic: <Description of the function performed and the logic used
* in the function>
* Example Call: Sharp_Detection();
*
*/
void Sharp_Detection(void)
{
	unsigned char value, value2;
	Sensor_read = ADC_Conversion(10);
	value = Sharp_GP2D12_estimation(Sensor_read);				//Stores Distance calculated in a variable "value".
	Sensor_read = ADC_Conversion(12);
	value2 = Sharp_GP2D12_estimation(Sensor_read);
	lcd_print(1, 7, value, 3);
	lcd_print(2, 7, value2, 3);
	uint8_t tmp = Cur_Pos[1] + 1;
	uint8_t tmp2 = tmp + 1;
	/*
	if(150<value&&value<250)
	{
	Blk_Pos[Cur_Pos[1]][2]=1;
	buzzer_on();_delay_ms(2000);buzzer_off();
	}
	else if(350<value&&value<450)
	{
	Blk_Pos[Cur_Pos[1]][3]=1;
	buzzer_on();_delay_ms(2000);buzzer_off();
	}
	else if(550<value&&value<650)
	{
	Blk_Pos[Cur_Pos[1]][4]=1;
	buzzer_on();_delay_ms(2000);buzzer_off();
	}*/
	if (700<value2&&value2 <= 790) //Positioned correctly
	{
		count1++;
		buzzer_on(); _delay_ms(500); buzzer_off();
	}
	else if (550<value2&&value2 <= 600)
	{
		Blk_Pos[tmp2][0] = 1;
	}
	else if (150<value2&&value2<190)
	{
		Blk_Pos[tmp][1] = 1;
	}
	else if (200<value2&&value2<240)
	{
		count1++;
		buzzer_on(); _delay_ms(500); buzzer_off();
	}
	if (700<value&&value <= 790)
	{
		Blk_Pos[tmp2][4] = 1;
	}
	else if (550<value&&value <= 600)
	{
		count1++;
		buzzer_on(); _delay_ms(500); buzzer_off();
	}
	else if (150<value&&value<190)
	{
		count1++;
		buzzer_on(); _delay_ms(500); buzzer_off();
	}

	else if (200<value&&value<240)
	{
		Blk_Pos[Cur_Pos[1] + 1][3] = 1;
	}
}

/*
*
* Function Name: SharpSide
* Input: void
* Output: void
* Logic: <Description of the function performed and the logic used
* in the function>
* Example Call: SharpSide();
*
*/	void SharpSide(void)
{
	unsigned char value2;
	Sensor_read = ADC_Conversion(12);
	value2 = Sharp_GP2D12_estimation(Sensor_read);
	lcd_print(2, 7, value2, 3);
	if (700<value2&&value2 <= 790)
	{
		if ((Cur_Pos[0]<3))
		{
			Blk_Pos[Cur_Pos[1] + 2][Cur_Pos[0] + 2] = 2;
			buzzer_on(); _delay_ms(2000); buzzer_off();
		}
	}
	else if (550<value2&&value2 <= 600)
	{
		if ((Cur_Pos[0]<3))
		{
			Blk_Pos[Cur_Pos[1] + 2][Cur_Pos[0] + 2] = 1;
			buzzer_on(); _delay_ms(2000); buzzer_off();
			count2++;
		}
	}
	else if (200<value2&&value2<240)
	{
		Blk_Pos[Cur_Pos[1] + 1][Cur_Pos[0] + 1] = 2;
		buzzer_on(); _delay_ms(2000); buzzer_off();
		count2++;
	}
	else if (150<value2&&value2<190)
	{
		Blk_Pos[Cur_Pos[1] + 1][Cur_Pos[0] + 1] = 1;
		buzzer_on(); _delay_ms(2000); buzzer_off();
	}
}
/*
*
* Function Name: <Function Name>
* Input: <Inputs (or Parameters) list with description if any>
* Output: <Return value with description if any>
* Logic: <Description of the function performed and the logic used
* in the function>
* Example Call: <Example of how to call this function>
*
*/
/*uint8_t SharpTwist(void)
{
if()
{
if(if front analog sensor is detecting)
{
rf(1);
left_wls();
forward_wls(1);
}
else
{
forward_wls(1);
}
}
else
{
left_degrees(45);
unsigned char value;
uint8_t ret=0;
Sensor_read = ADC_Conversion(10);
value = Sharp_GP2D12_estimation(Sensor_read);
lcd_print(1, 7, value, 3);
if(150<value&&value<250)
{
Blk_Pos[Cur_Pos[1]][2]=1;
buzzer_on();_delay_ms(2000);buzzer_off();
ret=2;
}
else if(350<value&&value<450)
{
Blk_Pos[Cur_Pos[1]][3]=1;
buzzer_on();_delay_ms(2000);buzzer_off();
ret=3;
}
else if(550<value&&value<650)
{
Blk_Pos[Cur_Pos[1]][4]=1;
buzzer_on();_delay_ms(2000);buzzer_off();
ret=4;
}
if(ret==0)
{
right_degrees(45);
}
else
{
left_degrees(45);
}
return ret;
}
}
*/

/*
*
* Function Name: print_sensor
* Input: <Inputs (or Parameters) list with description if any>
* Output: <Return value with description if any>
* Logic: This function prints the function
* in the function>
* Example Call: print
*
*/
/*
*
* Function Name: <Function Name>
* Input: void
* Output: void
* Logic: <Description of the function performed and the logic used
* in the function>
* Example Call: <Example of how to call this function>
*
*/
/*
*
* Function Name: twist
* Input: <Inputs (or Parameters) list with description if any>
* Output: <Return value with description if any>
* Logic: <Description of the function performed and the logic used
* in the function>
* Example Call: <Example of how to call this function>
*
*/
int twist(int x)
{
	if (x % 2 != 0)	//first section makes the servo rotate by 180 degrees
	{
		servo_1(0);
		_delay_ms(500);
		servo_1(180);
		_delay_ms(1000);
		servo_1_free();
	}
	else		//second section makes the servo rotate back by 180 degrees
	{
		servo_1(180);
		_delay_ms(500);
		servo_1(0);
		_delay_ms(1000);
		servo_1_free();
	}
	return x + 1;	//value of x is incremented by 1 i.e every time x is made either even or odd
}

/*
* Function Name:	 lift
* Input:			 int x and y -> Stores the even and odd values.
*					 int k and l -> Represents the degrees  from 0 to 90
* Output:			 Servo turns through 90 degrees
* Logic:			 this function consists of two sections,first section makes the servo rotate from 0 to 90 degrees and in second section
*					 it makes the servo rotate back from 90 to 0 degree.By using odd and even number concept both the sections are called alternately.
*
* Example Call:		 y=twist(y);
*
*/

int lift(int x)
{
	if (x % 2 != 0)	//first section makes the servo rotate by 90 degrees
	{

		unsigned char k = 0;

		for (k = 0; k <90; k++)	//k is slowly incremented from 0 to 90 degrees by introducing delay of 30 milliseconds after each increment.
		{
			servo_2(k);
			_delay_ms(30);
		}
		_delay_ms(500);
		servo_2_free();
	}
	else	//second section makes the servo rotate back to 0 degrees
	{
		unsigned char l = 90;

		for (l = 90; l >0; l--)	//l is slowly decremented from 90 to 0 degrees by introducing delay of 30 milliseconds after each decrement.
		{
			servo_2(l);
			_delay_ms(30);
		}
		_delay_ms(500);
		servo_2_free();

	}
	return x + 1;	//value of x is incremented by 1 i.e every time x is made either even or odd.
}

/*
* Function Name:	 arm
* Input:			 integer x and y -> Stores the even and odd values.
* Output:			 Controls twist and lift functions.
* Logic:			 this function controls the twist and lift functions, first it calls the lift function which lifts the arm and then twist function is called
*					 which rotates the claw by 180 degrees, and finally again lift function is called to lift the arm back to initial position.
* Example Call:		 arm(0);_delay_ms(500);
*
*/

void arm(int i)
{
	static int x, y;
	y = lift(y);
	_delay_ms(500);
	x = twist(x);
	_delay_ms(500);
	y = lift(y);
	_delay_ms(500);
	if (i == 0)
		count1++;
	else
		count2++;
}
/*
*
* Function Name: forward
* Input: void
* Output: void
* Logic: Passes appropriate
* Example Call: forward();
*
*/
/*
*
* Function Name: forward_wls
* Input: node
* Output: void
* Logic: Uses white line sensors to go forward by the nodes specified
* Example Call: forward_wlsD2(2) //Goes forward by two nodes
*
*/
void forward_wls(unsigned char node)
{
	unsigned char flagw = 0;
	while (1)
	{


		Left_white_line = ADC_Conversion(3);	//Getting data of Left WL Sensor
		Center_white_line = ADC_Conversion(2);	//Getting data of Center WL Sensor
		Right_white_line = ADC_Conversion(1);	//Getting data of Right WL Sensor


		//Function to detect 

		if ((Center_white_line >0x0C && Left_white_line >0x0C) || (Right_white_line>0x0C && Left_white_line>0x0C) || (Center_white_line>0x0C && Right_white_line>0x0C))
		{
			velocity(0, 0);
			forward();
			_delay_ms(1000);
			flagw = flagw + 1;
			velocity(215, 210);
			forward_mm(30);
			stop();
			_delay_ms(500);
			if (node == flagw)
			{
				break;
			}


		}
		if ((Center_white_line >= 0x0C))
		{
			buzzer_off();
			forward();
			velocity(215, 210);
		}

		if ((Left_white_line >= 0x0C))
		{
			buzzer_off();
			forward();
			velocity(80, 190);
		}

		if ((Right_white_line >= 0x0C))
		{
			buzzer_off();
			forward();
			velocity(190, 80);
		}
		CurPos(node, 0, 0, 0);
	}
}

/*
*
* Function Name: forward_wlsD2
* Input: node
* Output: void
* Logic: Version of forward_wls used in  D2, calls SharpSide at each node in order to detect and log blocks.
* Example Call: forward_wlsD2(2) //Goes forward by two nodes calling SharpSide at 3 nodes (starting, first , second)
*
*/
void forward_wlsD2(unsigned char node)
{
	unsigned char flagw = 0;
	while (1)
	{


		Left_white_line = ADC_Conversion(3);	//Getting data of Left WL Sensor
		Center_white_line = ADC_Conversion(2);	//Getting data of Center WL Sensor
		Right_white_line = ADC_Conversion(1);	//Getting data of Right WL Sensor


		//Function to detect 

		if ((Center_white_line >0x0C && Left_white_line >0x0C) || (Right_white_line>0x0C && Left_white_line>0x0C) || (Center_white_line>0x0C && Right_white_line>0x0C))
		{
			velocity(0, 0);
			forward();
			_delay_ms(1000);
			flagw = flagw + 1;
			velocity(215, 210);
			forward_mm(30);
			stop();
			_delay_ms(500);
			if (node == flagw + 1)
			{
				SharpSide();
			}
			else if (node == flagw + 2)
			{
				SharpSide();
			}
			else if (node == flagw + 3)
			{
				SharpSide();
			}
			else if (node == flagw)
			{
				SharpSide();
				break;
			}
		}
		if ((Center_white_line >= 0x0C))
		{
			//flag1=1;
			buzzer_off();
			forward();
			velocity(215, 210);
		}

		if ((Left_white_line >= 0x0C))
		{
			//flag1=1;
			buzzer_off();
			forward();
			velocity(80, 190);
		}

		if ((Right_white_line >= 0x0C))
		{
			//flag1=1;
			buzzer_off();
			forward();
			velocity(190, 80);
		}
		CurPos(node, 0, 0, 0);
	}
}

/*
*
* Function Name: right_wls
* Input: void
* Output: void
* Logic: Goes forward by 30mm from the node so that the axle is aligned and then takes a right turn
* Example Call: right_wls();
*
*/
void right_wls()
{
	velocity(215, 210);
	forward_mm(30);
	stop();
	_delay_ms(500);
	velocity(215, 210);
	right_degrees(90);
	stop();
	_delay_ms(500);


}

/*
*
* Function Name: left_wls
* Input: void
* Output: void
* Logic: Goes forward by 30mm from the node so that the axle is aligned and then takes a left turn
* Example Call: left_wls();
*
*/
void left_wls()
{
	velocity(215, 210);
	forward_mm(30);
	stop();
	_delay_ms(500);
	velocity(215, 210);
	left_degrees(90);
	stop();
	_delay_ms(500);

}

/*
*
* Function Name: rf
* Input: node
* Output: void
* Logic: Used to take a ninety degree right turn using the function right_wls and subsequently go forward by the number of nodes specified in the input.
* Example Call: rf(2); //Takes a right and goes forward by two nodes.
*
*/	void rf(unsigned int node)
{
	right_wls(); _delay_ms(500);
	forward_mm(node); _delay_ms(500);
	CurPos(-node, 0, node, 0);
}

/*
*
* Function Name: lf
* Input: node
* Output: void
* Logic: Used to take a ninety degree left turn using the function left_wls and subsequently go forward by the number of nodes specified in the input.
* Example Call: lf(2); //Takes a left and goes forward by two nodes.
*
*/
void lf(unsigned int node)
{
	left_wls(); _delay_ms(500);
	forward_mm(node); _delay_ms(500);
	CurPos(-node, 0, 0, node);
}

/****************************************************************
* MERGED SHARP LEFT WITH SHARP RIGHT TO FORM START FUNCTION.
*****************************************************************
* Function Name: <Function Name>
* Input: <Inputs (or Parameters) list with description if any>
* Output: <Return value with description if any>
* Logic: <Description of the function performed and the logic used
* in the function>
* Example Call: <Example of how to call this function>
*
*/

void Start(void)
{
	rf(1);
	unsigned char value;
	Sensor_read = ADC_Conversion(10);
	value = Sharp_GP2D12_estimation(Sensor_read);				//Stores Distance calculated in a variable "value".
	if (150<value&&value<190)
	{
		count1++;
		buzzer_on(); _delay_ms(500); buzzer_off();
		right_degrees(180);
		forward_wls(2);
		CurPos(0, 2, 0, 2);
	}
	else if (200<value&&value<240)
	{
		forward_wls(1);
		left_wls();
		arm(0);
		Blk_Pos[0][0] = 0;
		left_wls();
		forward_wls(3);
		CurPos(0, 4, 0, 1);
	}
	else
	{
		right_degrees(180);
		forward_wls(2);
		CurPos(0, 2, 0, 2);
	}
	Sensor_read = ADC_Conversion(12);
	value = Sharp_GP2D12_estimation(Sensor_read);				//Stores Distance calculated in a variable "value".
	if (150<value&&value<190)
	{
		count1++;
		buzzer_on(); _delay_ms(500); buzzer_off();
		right_degrees(180);
		forward_wls(1);
		left_wls();
		CurPos(0, 1, 1, 0);
	}
	else if (200<value&&value<240)
	{
		forward_wls(1);
		right_wls();
		arm(0);
		Blk_Pos[0][4] = 0;
		right_wls();
		forward_wls(1);
		CurPos(0, 2, 0, 0);
		left_wls();
	}
	else
	{
		right_degrees(180);
		forward_wls(1);
		left_wls();
		CurPos(0, 1, 1, 0);

	}
}

/*
*
* Function Name: IRCorridor
* Input: void
* Output: void
* Logic: Traverses the corridor using analog IR sensors, sets Cur_Pos[1] to the appropriate row number and resets Cur_Pos[0] to zero.
* Example Call: IRCorridor();
*
*/
void IRCorridor(void)
{
	unsigned int IR_1, IR_3, IR_5;
	unsigned int flag5 = 0;
	unsigned int flag6 = 0;
	turn_on_ir_proxi_sensors();
	ShaftCountRight = 0;
	while (1)
	{
		IR_1 = ADC_Conversion(4);
		IR_5 = ADC_Conversion(8);
		IR_3 = ADC_Conversion(6);
		forward();
		velocity(225, 225);
		if ((IR_3 <= 130) && flag6 == 0)
		{
			flag6 = 1;
			flag5 = 1;
			forward();
			velocity(0, 0);
			_delay_ms(500);
			velocity(225, 225);
			left_degrees(90);
			stop();
			_delay_ms(500);
			/*forward_mm(200);*/

		}
		else if ((IR_5 >= 154) && flag5 == 1 && flag6 == 1)
		{
			_delay_ms(870);
			forward();
			velocity(0, 0);
			_delay_ms(500);
			velocity(225, 225);
			right_degrees(90);
			flag5 = 1;
			forward_mm(150);
			velocity(220, 220);
			break;
		}
		else if (IR_5<143 && flag6 == 1 && flag5 == 1)
		{
			forward();
			velocity(150, 220);
		}
		else if (IR_1< 143 && flag6 == 1 && flag5 == 1)
		{
			forward();
			velocity(220, 150);
		}
	}
	lcd_print(2, 7, ShaftCountRight, 3);

	/*
	if(Lower Bound<ShaftCountRight&&ShaftCountRight<300)
	{
	//G1
	}
	else if(Lower Bound<ShaftCountRight&&ShaftCountRight<400)
	{
	//G2
	}
	else if(Lower Bound<ShaftCountRight&&ShaftCountRight<600)
	{
	//G3
	}
	else if(Lower Bound<ShaftCountRight&&ShaftCountRight<800)
	{
	//G4
	}
	else if(Lower Bound<ShaftCountRight&&ShaftCountRight<1000)
	{
	//G5
	}
	*/
	forward_wls(1);
	CurPos(0, 1, 0, 1);
	left_wls();
	uint8_t n = 2 * Cur_Pos[1];
	forward_wls(Cur_Pos[1] - 1);
	CurPos(0, n, 0, 0);
	flag = 2;
	int i, j;
	for (i = 0; i<8; i++)
	{
		for (j = 0; i<6; j++)
		{
			Blk_Pos[i][j] = 0;
		}
	}
}


/*
*
* Function Name: exit1
* Input: void
* Output: void
* Logic: Called on row 1 for exiting D1 or when count exceeds six(Maximum number of blocks in D1). Takes appropriate exit route according to current position and block positions.* Example Call: <Example of how to call this function>
* Example Call: exit1();
*
*/
void exit1(void)
{
	if (Cur_Pos[1] == 5)
	{
		if (Blk_Pos[5][3] == 0)
		{
			lf(5 - Cur_Pos[0]);
			/*Actual Buzzer sound*/buzzer_on(); _delay_ms(1000); buzzer_off();
			flag = 2;
		}
		else if (Blk_Pos[5][3] == 1)
		{
			forward_wls(1);
			lf(2);
			right_degrees(90);
			back_mm(200);
			/*Actual Buzzer sound*/buzzer_on(); _delay_ms(1000); buzzer_off();
			flag = 2;

		}
	}
	else if (Cur_Pos[1] == 6)
	{

		lf(2);
		right_wls();
		back_mm(200);
		/*Actual Buzzer sound*/buzzer_on(); _delay_ms(1000); buzzer_off();
		lf(150);
		CurPos(0, 0, 0, 1);
		right_wls();
		flag = 2;
	}
}
/*
*
* Function Name: exit2
* Input: void
* Output: void
* Logic: Called on row 2 for exiting D2 or when count exceeds four(Maximum number of blocks in D2). Subtracts current y co-ordinate with that of row 1 and subtracts current x co-ordinate with that of column 4.
* Example Call: exit2();
*
*/
void exit2(void)
{
	forward_wls(6 - Cur_Pos[1]);
	lf(4 - Cur_Pos[0]);
	buzzer_on();
}

void D1(void)
{
	while (1)
	{
		if (flag == 0)
		{
			Left_white_line = ADC_Conversion(3);	//Getting data of Left WL Sensor
			Center_white_line = ADC_Conversion(2);	//Getting data of Center WL Sensor
			Right_white_line = ADC_Conversion(1);	//Getting data of Right WL Sensor
			if (count1<7)
			{
				if (Cur_Pos[1] == 0)
				{

					forward_wls(1);
					Cur_Pos[1] = 0;
					Sharp_Detection(); _delay_ms(500);
					Start();

				}

				else if (Cur_Pos[0] == 2 && (Cur_Pos[1]>0 && Cur_Pos[1]<6))
				{
					uint8_t tmp = Cur_Pos[1] + 1;
					if (count1<7)
					{
						if (Blk_Pos[tmp][0] == 0)
						{
							if (Blk_Pos[tmp][1] == 0)
							{
								if (Blk_Pos[tmp][3] == 0)
								{
									if (Blk_Pos[tmp][4] == 0)
									{
										if (Cur_Pos[1] == 5)
										{
											exit1();
										}
										else
										{
											forward_wls(1); _delay_ms(500);
											Sharp_Detection(); _delay_ms(500);
										}
									}
									else
									{
										forward_wls(1); _delay_ms(500);
										lf(1); _delay_ms(500);
										arm(0); _delay_ms(500);
										right_degrees(90);
										rf(1);
										left_wls(); _delay_ms(500);
										if (Cur_Pos[1] == 5)
										{
											forward_wls(1);
											exit1();
										}
										else
										{
											Sharp_Detection(); _delay_ms(500);
										}
									}
								}
								else
								{
									if (Blk_Pos[tmp][4] == 0)
									{
										if (Cur_Pos[1] == 5)
										{
											lf(1);
											right_wls(); _delay_ms(500);
											arm(0); _delay_ms(500);
											exit1();
										}
										else
										{
											forward_wls(1); _delay_ms(500);
											left_wls(); _delay_ms(500);
											arm(0); _delay_ms(500);
											right_wls(); _delay_ms(500);
											Sharp_Detection(); _delay_ms(500);
										}
									}
									else
									{
										if (Cur_Pos[1] == 5)
										{
											lf(1); _delay_ms(500);
											right_wls(); _delay_ms(500);
											arm(0); _delay_ms(500);
											lf(1); _delay_ms(500);
											right_wls(); _delay_ms(500);
											arm(0); _delay_ms(500);
											exit1();
										}
										else
										{
											lf(2); _delay_ms(500);
											right_wls(); _delay_ms(500);
											arm(0); _delay_ms(500);
											rf(2); _delay_ms(500);
											left_wls(); _delay_ms(500);
											forward_wls(1); _delay_ms(500);
											left_wls(); _delay_ms(500);
											arm(0); _delay_ms(500);
											right_wls(); _delay_ms(500);
											Sharp_Detection(); _delay_ms(500);
										}
									}
								}
							}
							else
							{
								if (Blk_Pos[tmp][3] == 0)
								{
									if (Blk_Pos[tmp][4] == 0)
									{
										forward_wls(1); _delay_ms(500);
										right_wls(); _delay_ms(500);
										arm(0); _delay_ms(500);
										left_wls(); _delay_ms(500);
										if (Cur_Pos[1] == 6)
										{
											exit1();
										}
										else
										{
											Sharp_Detection(); _delay_ms(500);
										}
									}
									else
									{
										if (Cur_Pos[1] == 5)
										{
											lf(1);
											right_wls();
											arm(0); _delay_ms(500);
											lf(3); _delay_ms(500);
											arm(0); _delay_ms(500);
											exit1();
										}
										else
										{
											forward_wls(1); _delay_ms(500);
											right_wls(); _delay_ms(500);
											arm(0); _delay_ms(500);
											left_wls(); _delay_ms(500);
											lf(1); _delay_ms(500);
											arm(0); _delay_ms(500);
											right_wls();
											rf(1);
											left_wls(); _delay_ms(500);
											Sharp_Detection(); _delay_ms(500);
										}
									}
								}
								else
								{
									if (Blk_Pos[tmp][4] == 0)
									{
										if (Cur_Pos[1] == 5)
										{
											rf(1);
											left_wls();
											arm(0); _delay_ms(500);
											lf(2);
											right_wls();
											arm(0); _delay_ms(500);
											exit1();
										}
										else
										{
											forward_wls(1); _delay_ms(500);
											left_wls(); _delay_ms(500);
											arm(0); _delay_ms(500);
											right_degrees(180);
											arm(0); _delay_ms(500);
											left_wls(); _delay_ms(500);
											forward_wls(1); _delay_ms(500);
											Sharp_Detection(); _delay_ms(500);
										}
									}
									else
									{
										if (Cur_Pos[1] == 5)
										{
											rf(1);
											left_wls();
											arm(0); _delay_ms(500);
											lf(1); _delay_ms(500);
											right_wls(); _delay_ms(500);
											arm(0); _delay_ms(500);
											lf(3); _delay_ms(500);
											right_wls(); _delay_ms(500);
											arm(0); _delay_ms(500);
											exit1();
										}
										else
										{
											lf(2); _delay_ms(500);
											right_wls(); _delay_ms(500);
											arm(0); _delay_ms(500);
											rf(2); _delay_ms(500);
											left_wls(); _delay_ms(500);
											forward_wls(1); _delay_ms(500);
											left_wls(); _delay_ms(500);
											arm(0); _delay_ms(500);
											right_degrees(180);
											arm(0); _delay_ms(500);
											left_wls(); _delay_ms(500);
											forward_wls(1); _delay_ms(500);
											Sharp_Detection(); _delay_ms(500);
										}
									}
								}
							}

						}
						else
						{
							if (Blk_Pos[tmp][1] == 0)
							{
								if (Blk_Pos[tmp][3] == 0)
								{
									if (Blk_Pos[tmp][4] == 0)
									{
										forward_wls(1); _delay_ms(500);
										rf(1);
										arm(0); _delay_ms(500);
										left_wls(); _delay_ms(500);
										lf(1); _delay_ms(500);
										if (Cur_Pos[1] == 6)
										{
											exit1();
										}
										else
										{
											right_wls(); _delay_ms(500);
											Sharp_Detection(); _delay_ms(500);
										}
									}
									else
									{
										forward_wls(1); _delay_ms(500);
										rf(1);
										arm(0); _delay_ms(500);
										left_wls(); _delay_ms(500);
										lf(2); _delay_ms(500);
										arm(0); _delay_ms(500);
										right_wls(); _delay_ms(500);
										if (Cur_Pos[1] == 5)
										{
											back_mm(200);
											exit1();
										}
										else
										{
											rf(1);
											left_wls(); _delay_ms(500);
											Sharp_Detection(); _delay_ms(500);
										}
									}
								}
								else
								{
									if (Blk_Pos[tmp][4] == 0)
									{

										forward_wls(1); _delay_ms(500);
										rf(1); _delay_ms(500);
										arm(0); _delay_ms(500);
										left_wls(); _delay_ms(500);
										lf(2); _delay_ms(500);
										arm(0); _delay_ms(500);
										right_wls(); _delay_ms(500);
										if (Cur_Pos[1] == 5)
										{
											back_mm(200);
											exit1();
										}
										else
											Sharp_Detection(); _delay_ms(500);

									}
									else
									{
										lf(2); _delay_ms(500);
										right_wls(); _delay_ms(500);
										arm(0); _delay_ms(500);
										rf(2); _delay_ms(500);
										left_wls(); _delay_ms(500);
										forward_wls(1); _delay_ms(500);
										rf(1);
										arm(0); _delay_ms(500);
										left_wls(); _delay_ms(500);
										lf(1); _delay_ms(500);
										arm(0); _delay_ms(500);
										right_wls(); _delay_ms(500);
										Sharp_Detection(); _delay_ms(500);

									}
								}
							}
							else
							{
								if (Blk_Pos[tmp][3] == 0)
								{
									if (Blk_Pos[tmp][4] == 0)
									{
										rf(2); _delay_ms(500);
										left_wls(); _delay_ms(500);
										arm(0); _delay_ms(500);
										if (Cur_Pos[1] == 5)
										{
											lf(1); _delay_ms(500);
											right_wls(); _delay_ms(500);
											arm(0); _delay_ms(500);
											exit1();
										}
										else
										{
											lf(2); _delay_ms(500);
											right_wls(); _delay_ms(500);
											forward_wls(1); _delay_ms(500);
											right_wls(); _delay_ms(500);
											arm(0); _delay_ms(500);
											left_wls(); _delay_ms(500);
											Sharp_Detection(); _delay_ms(500);
										}
									}
									else
									{
										rf(2); _delay_ms(500);
										left_wls(); _delay_ms(500);
										arm(0); _delay_ms(500);
										if (Cur_Pos[1] == 5)
										{
											lf(1); _delay_ms(500);
											right_wls(); _delay_ms(500);
											arm(0); _delay_ms(500);
											lf(3); _delay_ms(500);
											right_wls(); _delay_ms(500);
											arm(0); _delay_ms(500);
											exit1();
										}
										else
										{
											lf(4); _delay_ms(500);
											right_wls(); _delay_ms(500);
											arm(0); _delay_ms(500);
											rf(2); _delay_ms(500);
											left_wls(); _delay_ms(500);
											forward_wls(1); _delay_ms(500);
											right_wls(); _delay_ms(500);
											arm(0); _delay_ms(500);
											left_wls(); _delay_ms(500);
											Sharp_Detection(); _delay_ms(500);
										}
									}
								}
								else
								{
									if (Blk_Pos[tmp][4] == 0)
									{
										if (Cur_Pos[1] == 5)
										{
											rf(2); _delay_ms(500);
											left_wls(); _delay_ms(500);
											arm(0); _delay_ms(500);
											lf(1); _delay_ms(500);
											right_wls(); _delay_ms(500);
											arm(0); _delay_ms(500);
											lf(2); _delay_ms(500);
											right_wls(); _delay_ms(500);
											arm(0); _delay_ms(500);
											exit1();
										}
										else
										{
											rf(2); _delay_ms(500);
											left_wls(); _delay_ms(500);
											arm(0); _delay_ms(500);
											lf(2); _delay_ms(500);
											right_wls(); _delay_ms(500);
											forward_wls(1); _delay_ms(500);
											left_wls(); _delay_ms(500);
											arm(0); _delay_ms(500);
											right_degrees(180);
											arm(0); _delay_ms(500);
											left_wls(); _delay_ms(500);
											forward_wls(1); _delay_ms(500);
											Sharp_Detection(); _delay_ms(500);
										}

									}
									else
									{
										if (Cur_Pos[1] == 5)
										{
											rf(2); _delay_ms(500);
											left_wls(); _delay_ms(500);
											arm(0); _delay_ms(500);
											lf(1); _delay_ms(500);
											right_wls(); _delay_ms(500);
											arm(0); _delay_ms(500);
											lf(2); _delay_ms(500);
											right_wls(); _delay_ms(500);
											arm(0); _delay_ms(500);
											lf(1); _delay_ms(500);
											right_wls(); _delay_ms(500);
											arm(0); _delay_ms(500);
											exit1();
										}
										else
										{
											rf(2); _delay_ms(500);
											left_wls(); _delay_ms(500);
											arm(0); _delay_ms(500);
											lf(4); _delay_ms(500);
											right_wls(); _delay_ms(500);
											arm(0); _delay_ms(500);
											rf(2); _delay_ms(500);
											left_wls(); _delay_ms(500);
											forward_wls(1); _delay_ms(500);
											left_wls(); _delay_ms(500);
											arm(0); _delay_ms(500);
											right_degrees(180);
											arm(0); _delay_ms(500);
											left_wls(); _delay_ms(500);
											forward_wls(1); _delay_ms(500);
											Sharp_Detection(); _delay_ms(500);
										}
									}
								}
							}
						}
					}
					else
					{
						/*Actual Buzzer sound*/buzzer_on(); _delay_ms(1000); buzzer_off();
						// go forward by 5-Cur_Pos[0] * 200;
						exit1();
						Cur_Pos[0] = 0;
					}

				}
				else
				{
					break;
				}
			}
		}

	}
}
/*void D2(void)
{
while(1)
{
Left_white_line = ADC_Conversion(3);	//Getting data of Left WL Sensor
Center_white_line = ADC_Conversion(2);	//Getting data of Center WL Sensor
Right_white_line = ADC_Conversion(1);	//Getting data of Right WL Sensor
if(flag==1)
{
if(count2<5)
{
int n=SharpTwist();
if(n!=0)
{
//Go and check
//Stop at every node and check
forward_wlsD2(n-1);
arm(1);
if(Blk_Pos[Cur_Pos[1]+1][Cur_Pos[0]]==1)
{
right_wls();
arm(1);
Blk_Pos[Cur_Pos[1]+1][Cur_Pos[0]]==2;
back_mm();
right_wls();
forward_wlsD2(1);
CurPos(1,0,1,0);
if(Cur_Pos[1]==5)
{
exit2();
}
else
{
back_wls((Cur_Pos[0]-1));
}

}
else if(Blk_Pos[Cur_Pos[1]+1][Cur_Pos[0]-1]==1)
{
if(Cur_Pos[1]==4)
{
rf(1);
right_wls();
arm(1);
CurPos(0,0,0,1);
left_wls();
exit2();
}
else
{
back_wls(2);
rf(1);
left_wls();
arm(1);
rf(1);
CurPos(2,0,0,0); //verify

}

}
else if(Blk_Pos[Cur_Pos[1]+1][Cur_Pos[0]+1]==1)
{
rf(1);
left_wls();
arm(1);
if(Cur_Pos[1]==5)
{
right_wls();
exit2();
}
else
{
back_wls(Cur_Pos[0]-1);
}

}

}
}
else
{
exit2();
}
}
else
{
break;
}

}
}

*/

/*
* Function Name: main
* Input: None
* Output: int to inform the caller that the program exited correctly or
* incorrectly (C code standard)
* Logic: Call functions D1 followed by functions IRCorridor and D2
* Example Call: Called automatically by the Operating System
*
*/


int main(void)
{
	init_devices();
	lcd_set_4bit();
	lcd_init();
	D1();
	//IRCorridor;
	//D2();
	return 0;
}
