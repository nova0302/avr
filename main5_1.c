/*
 * day5_1.c
 *
 * Created: 2020-08-14 오전 10:22:52
 * Author : TEACHER
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

#define sbit(x,PORT) (PORT) |= (1<<x)
#define cbit(x,PORT) (PORT) &= ~(1<<x)

int main(void)
{
	uint16_t rValues[5]={330,458,595,724, 895};
	// LED 17~20 
	DDRA |=0xF0; 		
		
   // AREF = AVcc
   ADMUX = (1<<REFS0);

   // ADC channel 0
   ADMUX &= 0xF0;

   // ADC Enable and prescaler of 128
   // 800000/128 = 62500
   ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);

   /* Replace with your application code */
   while (1)
   {
	   uint16_t adcValue;
	   // start single conversion
	   // write '1' to ADSC
	   ADCSRA |= (1<<ADSC);

	   // wait for conversion to complete
	   // ADSC becomes '0' again
	   // till then, run loop continuously
	   while(ADCSRA & (1<<ADSC));
		adcValue = ADCW;
			 if(adcValue < rValues[0]) {PORTA &= 0x0f; sbit(4, PORTA);}
		else if(adcValue < rValues[1]) {PORTA &= 0x0f; sbit(5, PORTA);}
		else if(adcValue < rValues[2]) {PORTA &= 0x0f; sbit(6, PORTA);}
		else if(adcValue < rValues[3]) {PORTA &= 0x0f; sbit(7, PORTA);}
		else if(adcValue < rValues[4]) {PORTA |= 0xf0;}
	   
	   _delay_ms(10);
   }
}

