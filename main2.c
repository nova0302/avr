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
	uint8_t bPattern[10] ={
		0b11111001 // 0
		,0b11111001 //1 
		,0b11111001 //2
		,0b11111001 //3
		,0b11111001
		,0b11111001
		,0b11111001
		,0b11111001
		,0b11111001
		,0b11111001 //9
	};
	// DDRx, PORTx, PINx
	// port b 0번째 비트를 출력으로 설정
	DDRA = (1 << DDA2) | (1 << DDA1);
	PORTA &= ~(1 << DDA1);// 00000010 
	DDRC = 0xFF;
    while (1) 
    {	
		if(PINB & (1 << PINB1)){//버튼이 눌려지지 않았을 경우
			for(int i = 0; i < 10; i++)
			{
				PORTC = bPattern[i];
				_delay_ms(1000);
			}	
		}
		else
		{
			for(int i = 9; i >= 0; i--)
			{
				PORTC = bPattern[i];
				_delay_ms(1000);
			}
		}
	}
}

