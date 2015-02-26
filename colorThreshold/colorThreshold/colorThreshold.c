/*
 * colorThreshold.c
 *
 * Created: 1/24/2015 2:08:19 PM
 *  Author: Ahmed
 */ 


#include <avr/io.h>
#include "Color_Sensor.c"

int main(void)
{
    int c[4],indicator[4],color[4],i,min;
	init();
	for(i=0;i<4;i++)
		c[i]=0;
	for(i=0;i<4;i++)
	{
		read();
	
		if(red>green && red>blue)
			if(c[0]>red)
			{
				c[3]=red;
				indicator[3]=3-i;
			}
			else
			{
				if(c[0]!=0)
				{
					c[3]=c[0];
					indicator[3]=indicator[0];
				}
				c[0]=red;
				indicator[0]=3-i;
				
			}
		else if(blue>green && red<blue)
			if(c[2]>blue)
			{
				c[3]=blue;
				indicator[3]=3-i;
			}
			else
			{
				if(c[2]!=0)
				{
					c[3]=c[2];
					indicator[3]=indicator[2];
				}
				c[2]=blue;
				indicator[2]=3-i;
			}
		else if(c[1]>green)
			{
				c[3]=green;
				indicator[3]=3-i;
			}

			else
			{
				if(c[1]!=0)
				{
					c[3]=c[1];
					indicator[3]=indicator[1];
				}
				c[1]=green;
				indicator[1]=3-i;
			}
		
	}
	if(c[0]<c[1])
		if(c[0]<c[2])
			min=c[0];
		else min=c[2];
	else if(c[1]<c[2])
			min=c[1];
		else min=c[]
	for(i=0;i<4;i++)
		color[indicator[i]]=i;
	int t=c[3];
	lcd_cursor(1,1);  //set the cursor on row 1, column 1
	
	lcd_string("Threshold"); // Display "Blue Pulses" on LCD
	t+=200;
	lcd_print(2,1,t,5);
	_delay_ms(2000);
	lcd_wr_command(0x01); //Clear the LCD
	
	lcd_string("At Node 1"); // Display "Blue Pulses" on LCD
	lcd_print(2,1,color[0],5);
	_delay_ms(2000);
	lcd_wr_command(0x01);
	
	lcd_string("At Node 2"); // Display "Blue Pulses" on LCD
	lcd_print(2,1,color[1],5);
	_delay_ms(2000);
	lcd_wr_command(0x01);
	
	lcd_string("At Node 3"); // Display "Blue Pulses" on LCD
	lcd_print(2,1,color[2],5);
	_delay_ms(2000);
	lcd_wr_command(0x01);
	
	lcd_string("At Node 4"); // Display "Blue Pulses" on LCD
	lcd_print(2,1,color[3],5);
	_delay_ms(2000);
	lcd_wr_command(0x01);
	return 0;
}