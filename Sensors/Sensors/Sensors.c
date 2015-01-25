#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <math.h> //included to support power function
#include "white.c"
#include "sensor.c"
#include "velocity.c"
#include "servo.c"
#include "color.c"

void port_init();
void timer5_init();
void velocity(unsigned char, unsigned char);
void motors_delay();
void onNode();
//Servo Test
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
void buzzer_on (void)
{
	unsigned char port_restore = 0;
	port_restore = PINC;
	port_restore = port_restore | 0x08;
	PORTC = port_restore;
}

void buzzer_off (void)
{
	unsigned char port_restore = 0;
	port_restore = PINC;
	port_restore = port_restore & 0xF7;
	PORTC = port_restore;
}


void init_devices (void)
{
	velocity_main();
	servo_main();
	color_main();
	white_main();
	sensor_main(); 
}


// forward jaa plz

void set_color()
{
	Left_white_line = ADC_Conversion(3);	//Getting data of Left WL Sensor
	Center_white_line = ADC_Conversion(2);	//Getting data of Center WL Sensor
	Right_white_line = ADC_Conversion(1);	//Getting data of Right WL Sensor
	//read_color();
}

//Main Function
int main()
{
	init_devices();
	//travel(1,3);
	//buzzer();
	while(1)
	{
		_delay_ms(1000);
		servo_1(0);
		lcd_print(1,1,red,5);
		velocity(100,100);
		forward();	
		_delay_ms(1000);
		print_sensor(1,1,11);
		print_sensor(1,5,1);
		print_sensor(1,8,2);
		print_sensor(1,11,3);
		
		lcd_print(2,1,red,5);
		lcd_print(2,1,blue,5);
		lcd_print(2,1,green,5);
		servo_1(100);
		
		_delay_ms(5000);	// Display for 1000ms or 1 second	
	}
}