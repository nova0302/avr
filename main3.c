/*
 * day3.c
 *
 * Created: 2020-08-12 오전 9:32:29
 * Author : TEACHER
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
uint16_t num = 0;
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
	sei(); // 글로벌 인터럽트 인에이블
	// DDRx, PORTx, PINx
	// port b 0번째 비트를 출력으로 설정
	DDRA = (1 << DDA3) | (1 << DDA2) | (1 << DDA1);
	//PORTA &= ~(1 << DDA1);// 11111101
	DDRC = 0xFF;
	DDRB = 1;
	while (1)// 조건이 참(0이아니면)이면 반복해서 계속 실행
	{
		PORTB ^= 1;
		if (num++ > 999)
		{
			num = 0;
		}
		_delay_ms(100);
	}
}
// Interrupt Service Routine
ISR(TIMER0_COMP_vect)
{
	static uint8_t selDigit = 0;
	if(selDigit == 0){
		selDigit = 1;
		uint8_t digit = num / 100; // 100의 자리값 계산
		PORTC = bP[digit];
		PORTA |= (1 << DDA3) | (1 << DDA2);
		PORTA &= ~(1 << DDA1);// 11111101
	}
	else if (selDigit = 1)
	{
		selDigit = 2;
		uint8_t digit = (num / 10) % 10; // 10의 자리값 계산
		PORTC = bP[digit];
		PORTA |= (1 << DDA3) | (1 << DDA1);
		PORTA &= ~(1 << DDA2);// 11111011
	}	
	else
	{
		selDigit = 0;
		uint8_t digit = num % 10; // 1의 자리값 계산
		PORTC = bP[digit];
		PORTA |= (1 << DDA2) |(1 << DDA1);
		PORTA &= ~(1 << DDA3);// 11110111
	}
}

