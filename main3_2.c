/*
* day3_2.c
*
* Created: 2020-08-12 오후 4:32:16
* Author : TEACHER
*/

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <util/atomic.h>
#include <avr/interrupt.h>

unsigned long millis ();
void init_millis(unsigned long f_cpu);
void boardInit(void);

int16_t num = 0;
uint8_t bP[10] ={
	0b0111111       ,0b0000110
	,0b1011011      ,0b1001111
	,0b1100110      ,0b1101101
	,0b1111101      ,0b0100111
	,0b1111111      ,0b1101111
};
int main(void)
{
	unsigned long tNow, tLast=0;
	uint8_t enswLast=0, enswNow;
	uint8_t en1Last=0, en1Now;
	uint8_t en2Now;
	uint8_t cntUp = 1;
	boardInit();
	while (1)// 조건이 참(0이아니면)이면 반복해서 계속 실행
	{
		tNow = millis();
		if(tNow - tLast > 50)
		{
			// EN_SW Event 처리
			enswNow = (PINB & (1 << PINB2)) ? 1 : 0;
			if(!enswNow && enswLast)
			{
				PORTD  ^= 1;
				if(cntUp == 0)
					cntUp = 1;
				else
					cntUp = 0;
			}
			enswLast = enswNow;
			// Rotary Encoder 회전 처리
			en1Now = (PINB & (1 << PINB0)) ? 1 : 0;
			if(en1Now != en1Last)
			{
				PORTD  ^= 1;
				en2Now = (PINB & (1 << PINB1)) ? 1 : 0;
				if(en1Now == en2Now)
				{
					if(++num > 999) 
						num = 0;
				}
				else
				{
					if(--num < 0) 
						num = 999;
				}
			}
			en1Last = en1Now;
			tLast = tNow;
		}
	}
}


// Interrupt Service Routine
ISR(TIMER0_COMP_vect)
{
	static uint8_t selDigit = 0;
	if(selDigit == 0){
		selDigit = 1;
		uint8_t digit = num / 100; // 100의 자리값 계산
		PORTC = bP[digit]; // data 실어주고
		PORTA |= (1 << DDA3) | (1 << DDA2);
		PORTA &= ~(1 << DDA1);// 11111101 // 켜주고
	}
	else if (selDigit == 1)
	{
		selDigit = 2;
		uint8_t digit = (num / 10) % 10; // 10의 자리값 계산
		PORTC = bP[digit]; // data 실어주고
		PORTA |= (1 << DDA3) | (1 << DDA1);
		PORTA &= ~(1 << DDA2);// 11111011 // 켜주고
	}
	else
	{
		selDigit = 0;
		uint8_t digit = num % 10; // 1의 자리값 계산
		PORTC = bP[digit];// data 실어주고
		PORTA |= (1 << DDA2) |(1 << DDA1);
		PORTA &= ~(1 << DDA3);// 11110111 // 켜주고
	}
}

volatile unsigned long timer1_millis;
//NOTE: A unsigned long holds values from 0 to 4,294,967,295 (2^32 - 1). It will roll over to 0 after reaching its maximum value.

//timer1 인터럽트 서비스 루틴
ISR(TIMER1_COMPA_vect)
{
	timer1_millis++;
}
// timer1을 설정
void init_millis(unsigned long f_cpu)
{
	unsigned long ctc_match_overflow;
	
	ctc_match_overflow = ((f_cpu / 1000) / 8); //when timer1 is this value, 1ms has passed
	
	// (Set timer to clear when matching ctc_match_overflow) | (Set clock divisor to 8)
	TCCR1B |= (1 << WGM12) | (1 << CS11);
	
	// high byte first, then low byte
	OCR1AH = (ctc_match_overflow >> 8);
	OCR1AL = ctc_match_overflow;
	
	// Enable the compare match interrupt
	TIMSK |= (1 << OCIE1A);
	
	//REMEMBER TO ENABLE GLOBAL INTERRUPTS AFTER THIS WITH sei(); !!!
}
// 현재시간을 밀리세크 단위로 출력해준다.
unsigned long millis ()
{
	unsigned long millis_return;
	
	// Ensure this cannot be disrupted
	ATOMIC_BLOCK(ATOMIC_FORCEON) {
		millis_return = timer1_millis;
	}
	return millis_return;
}

void boardInit(void){
	// timer0 설정 
	TCCR0 |= (1 << WGM01) //  Clear Timer on Compare Match (CTC) Mode 로 설정
	| (1 << CS02) | (1 << CS00); // f_cpu/1024 -> 1Khz
	OCR0 = 64; 
	TIMSK |= (1 << OCIE0); // timer0 인터럽트 인에이블
	// DDRx, PORTx, PINx
	// port A 3~1번째 비트를 출력으로 설정
	DDRA = (1 << DDA3) | (1 << DDA2) | (1 << DDA1);
	// port C 7:0번째 비트를 출력으로 설정
	DDRC = 0xFF;
	// 위에서 출력으로 설정한 포트 C를 0으로 초기화
	PORTC = 0;
	// port D의 0번째 비트를 출력으로 설정
	DDRD = (1 << DDD0);
	
	init_millis(8000000UL); //the frequency Atmega16a is running at
	sei(); // 글로벌 인터럽트 인에이블
}

