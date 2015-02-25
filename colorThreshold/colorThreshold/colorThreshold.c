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
    int c[4],i;
	init();
	for(i=0;i<4;i++)
		c[i]=0;
	for(i=0;i<4;i++)
	{
		read();
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
	return 0;
}