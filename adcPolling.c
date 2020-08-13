/*
 * adcPolling.c
 *
 * Created: 2020-08-13 오전 10:59:53
 * Author : TEACHER
 */ 

#include <stdio.h> //for printf
#include <stdint.h> // for uint~~
#include <assert.h>
#include <stdbool.h>

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

void USART_init(unsigned int ubrr) {
	UBRRH = (unsigned char)(ubrr >> 8);
	UBRRL = (unsigned char)ubrr;
	UCSRB = 1 << RXEN  | 1 << TXEN;
	UCSRC = 1 << URSEL | 1 << USBS | 3 << UCSZ0;
}

int USART_transmit( unsigned char data, FILE *stream ) {
	while(!(UCSRA & (1 << UDRE)));
	UDR = data;
	return 0;
}

int main(void)
{
	
	fdevopen(USART_transmit, NULL);
	
	USART_init(MYUBRR);
	// AREF = AVcc
	ADMUX |= (1<<REFS0);
	// ADC channel 0
	ADMUX &= 0xF0; 
	// ADC Enable and prescaler of 128
	// 8000000/128 = 62500
	ADCSRA |= (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
	uint16_t i = 0;
	
	/* Replace with your application code */
	while (1)
	{
		// start single conversion
		// write '1' to ADSC
		ADCSRA |= (1<<ADSC);
		
		// wait for conversion to complete
		// ADSC becomes '0' again
		// till then, run loop continuously
		while(ADCSRA & (1<<ADSC));
		
		
		printf("%d: ADC Values : %d\r\n", i++, ADCW);
		_delay_ms(500);
	}
}

