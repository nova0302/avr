/*
* day4_1.c
*
* Created: 2020-08-13 오후 3:21:51
* Author : TEACHER
*/

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

typedef enum _motDir {CW, CCW}EMotDir;

void stepStm(EMotDir dir)
{
	static int8_t theCounter = 0;
	if (dir == CW) {
		if (++theCounter > 3) {
			theCounter = 0;
		}
		}else{
		if (--theCounter < 0) {
			theCounter = 3;
		}
	}

	switch (theCounter) {
		case 0: {PORTD &= 0b10001111; break;}
		case 1: {PORTD &= 0b10010111; break;}
		case 2: {PORTD &= 0b10100111; break;}
		case 3: {PORTD &= 0b11000111; break;}
		default: break;
	}
}
int main(void)
{
	// port D의 6:3번째 비트를 출력으로 설정
	DDRD |= (1 << DDD6) |(1 << DDD5)| (1 << DDD4)| (1 << DDD3);
	/* Replace with your application code */
	while (1)
	{
		stepStm(CW);
		_delay_ms(100);
	}
}

