/*
 * DeathDoor.c
 *
 * Created: 3/3/2015 12:25:20 PM
 *  Author: Ahmed
 */ 


#include <avr/io.h>



void indicator_set()
{
	forwardJaa_Indi();
	buzzer();
	//turn efficiency
	forward_mm(160);
	buzzer();
	//scan 4
	soft_right_2();
	_delay_ms(700);
	while(ADC_Conversion(3)<40);
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
	forwardJaa();
	//buzzer_on();
}

void newSenseSequence()
{
		//node 1
	velocity(turn_v,turn_v);
	forward_mm(60);
	while (ADC_Conversion(3)<50)	//earlier 3
		right();
	//_delay_ms(140);
	stop();
	
	//node 2
	left_degrees(150);
	velocity(turn_v, turn_v);
	while (ADC_Conversion(2)<50)
		left();
	//_delay_ms(50);
	stop();
	
	//on line
	left();
	while(ADC_Conversion(2)<=40);
	stop();
}

int main(void)
{
    __init__();
	forward();
	_delay_ms(1000);
	indicator_set();
	
	
	//reached Terminal

	newSenseSequence();

	
	
	back_mm(60);
	pick(0);
	pick(1);
	
	forwardJaa();
	turnLeft();
	forwardJaa();
	
	//at sort
	turnLeft();
	back_mm(60);
	drop(0);
	Uturn();
	
	
	forwardJaa();
	newSenseSequence();
	drop(1);
	
	forwardJaa();
	pick(0);
	
	forwardJaa();
	
	//At Terminal 3
	newSenseSequence();
	back_mm(60);
	pick(1);
	blankUturn();
	//back maybe
	drop(0);
	Uturn();
	//may be back_mm()
	pick(0)
	
	forwardJaa();
	turnRight();
	forwardJaa();
	forwardJaa();
	
	//at Sort
	turnRight();
	forwardJaa();
	
	//At term 2
	Uturn();
	back_mm(60);
	
	
}