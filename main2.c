/*
* blinky.c
*
* Created: 2020-08-10 오전 10:54:57
* Author : TEACHER
*/
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
uint8_t num = 0;
uint8_t bP[10] ={
	0b11111001 		,0b11111001
	,0b11111001 	,0b11111001
	,0b11111001		,0b11111001
	,0b11111001		,0b11111001
	,0b11111001		,0b11111001
};

int main(void)
{

	// timer0 setting
	TCCR0 |= (1 << WGM01) | (1 << CS02) | (1 << CS00);
	OCR0 = 64;
	TIMSK |= (1 << OCIE0);
	sei();

	// DDRx, PORTx, PINx
	// port b 0번째 비트를 출력으로 설정
	DDRA = (1 << DDA2) | (1 << DDA1);
	PORTA &= ~(1 << DDA1);// 11111101
	DDRC = 0xFF;
	while (1)
	{
		if (num++ > 99)
		{
			num = 0;
		}
		_delay_ms(1000);
	}
}

ISR(TIMER0_COMP_vect)
{
	static uint8_t selDigit = 0;

	if(selDigit == 0){
		selDigit = 1;
		uint8_t digit=num/10;
		PORTC = bP[digit];
		PORTA |= (1 << DDA2);
		PORTA &= ~(1 << DDA1);// 11111101
	}
	else
	{
		selDigit = 0;
		uint8_t digit = num % 10;
		PORTC = bP[digit];
		PORTA |= (1 << DDA1);
		PORTA &= ~(1 << DDA2);// 11111101
	}
}

