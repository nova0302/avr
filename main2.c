/*
* blinky.c
*
* Created: 2020-08-10 오전 10:54:57
* Author : TEACHER
*/
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	uint8_t bP[10] ={
		0b11111001 		,0b11111001
		,0b11111001 	,0b11111001
		,0b11111001		,0b11111001
		,0b11111001		,0b11111001
		,0b11111001		,0b11111001
	};

	// DDRx, PORTx, PINx
	// port b 0번째 비트를 출력으로 설정
	DDRA = (1 << DDA2) | (1 << DDA1);
	PORTA &= ~(1 << DDA1);// 11111101
	DDRC = 0xFF;
	while (1)
	{
		for(int i = 0; i < 10; i++)
		{
			PORTC = bP[i];
			PORTA &= ~(1 << DDA1);// 11111101
			_delay_ms(10);
			PORTC = bP[i];
			PORTA &= ~(1 << DDA2);// 11111101
			_delay_ms(10);
		}
	}
}

