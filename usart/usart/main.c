/*
 * nived_USart.c
 *
 * Created: 7/3/2017 10:32:18 PM
 * Author : Nived
 */

#include <avr/io.h>
#define  F_CPU 14745600UL
#include <util/delay.h>
#define  BAUDRATE 9600
#define BAUD_PRESCALER ((F_CPU/16*(BAUDRATE))-1)


void USART_Init( unsigned int ubrr)
{
	//Set baud rate
	UBRRH = (unsigned char)(ubrr>>8);
	UBRRL = (unsigned char)ubrr;
	UCSRB = (1<<RXEN)|(1<<TXEN);
	// Set frame format: 8data, 2stop bit
	UCSRC = (1<<URSEL)|(1<<USBS)|(3<<UCSZ0);
}

void USART_Transmit( unsigned char data )
{
	// Wait for empty transmit buffer
	while ( !( UCSRA & (1<<UDRE)) );
	// Put data into buffer, sends the data
	UDR = data;
}

unsigned char USART_Receive( void )
{
	/* Wait for data to be received*/
	while ( !(UCSRA & (1<<RXC)) );
	/* Get and return received data from buffer*/
	return UDR;
}

void main( void )
{
	USART_Init(95);
	while(1){

		USART_Transmit('a');
		_delay_ms(100);
		//char r = USART_Receive()
	}
}