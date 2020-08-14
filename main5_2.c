/*
* day5_2.c
*
* Created: 2020-08-14 오후 11:48:33
* Author : Sanglae Kim
*/

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

#define sbit(x,PORT) (PORT) |= (1<<x)
#define cbit(x,PORT) (PORT) &= ~(1<<x)

uint16_t num;

void drv4015(){
	for(int i=16; i>=0; i--){	
		if(num & (1 << i))
			sbit(1, PORTD);
		else
			cbit(1, PORTD);
		PORTD ^= 1;
		PORTD ^= 1;
	}
}

int main(void)
{
	DDRD = (1 << DDD1) | (1 << DDD0); // PD1 : data, PD0 : clk
	drv4015();
	_delay_ms(500);
	while (1)
	{		
		num++;
		drv4015();
		_delay_ms(500);
	}
}

