/*
 * day5_2.c
 *
 * Created: 2020-08-14 오전 11:09:24
 * Author : TEACHER
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define sbit(x,PORT) (PORT) |= (1<<x)
#define cbit(x,PORT) (PORT) &= ~(1<<x)

uint16_t num;

int main(void)
{
	// timer0 설정
	TCCR0 |= (1 << WGM01) //  Clear Timer on Compare Match (CTC) Mode 로 설정
	| (1 << CS02) | (1 << CS00); // f_cpu/1024 -> 1Khz
	OCR0 = 64;
	TIMSK |= (1 << OCIE0); // timer0 인터럽트 인에이블
	
	DDRD = (1 << DDD1) | (1 << DDD0); // PD1 : data, PD0 : clk
	sei(); // 글로벌 인터럽트 인에이블
    
    while (1) 
    {
		num++;
		_delay_ms(100);
    }
}

void drv4015(){
	for(int i=0; i<16; i++){
		PORTD ^= 1;
		if(num & (1 << i))
			sbit(1, PORTD);
		else
			cbit(1, PORTD);
		PORTD ^= 1;	
	}
}
ISR(TIMER0_COMP_vect)
{
	drv4015();
}

