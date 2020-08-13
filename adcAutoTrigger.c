/*
 * adcAtuoTrigger.c
 *
 * Created: 2020-08-13 오전 11:04:56
 * Author : TEACHER
 */ 

#include <stdio.h> //for printf
#include <stdint.h> // for uint~~
#include <assert.h>
#include <stdbool.h>

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

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

	DDRB =  (1 << DDB3) |(1 << DDB2);                     // Setup PB2 and PB3 as output
	
	TCCR0 |= (1<<CS02)|(1<<CS00)|    // Start timer0 with prescaler 1024
	(1<<WGM01)|(1<<COM00);  // CTC mode, toggle OC0 on compare match
	
	OCR0 = 255;                      // Set the capturevalue 8000000/1024/255 = 30Hz, PB3
	
	SFIOR |= (1<<ADTS0)|(1<<ADTS1);  // Set Timer 0 Compare as ADC trigger
	
	ADMUX |= (1<<REFS0);             // Set reference to AVcc

	ADCSRA |= (1<<ADEN)|(1<<ADATE)|  // Enable ADC, Enable auto Triggering
	(1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0)|(1<<ADIE);  // Set prescaler to 16
	// Fadc=Fcpu/prescaler= 1000000/16=62.5kHz
	// Fadc should be between 50kHz and 200kHz

	sei();                           // Set the I-bit in SREG

	for(;;);                         // Endless loop;
	// main() will never be left

	return 0;                        // This line will never be executed
}

// Interrupt subroutine for ADC conversion complete interrupt
ISR(ADC_vect)
{
	static uint8_t n=0;

	if(ADC >= 512)                // Compare the conversionresult with 512
		PORTB |= (1 << PORTB2);            // If larger, set PB3
	else
		PORTB &= ~(1 << PORTB2);           // If smaller, reset PB3
	if(++n % 20 == 0)
		printf("ADC Value: %d\r\n", ADCW);
	TIFR |= (1<<OCF0);            // Clear the Timer 0 interruptflag
}


