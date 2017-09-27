/*
 * I2C.c
 *
 * Created: 7/6/2017 3:47:58 PM
 * Author : zeus
 */ 

#include <avr/io.h> 
#define  F_CPU 14745600UL
#include <util/delay.h>
#define  BAUDRATE 9600
#define BAUD_PRESCALER ((F_CPU/16*(BAUDRATE))-1)
//Baud rate is 2400


void USART_Init( unsigned int ubrr)
{
	/*Set baud rate */
	UBRRH = (unsigned char)(ubrr>>8);
	UBRRL = (unsigned char)ubrr;
	UCSRB = (1<<RXEN)|(1<<TXEN);
	/* Set frame format: 8data, 2stop bit */
	UCSRC = (1<<URSEL)|(1<<USBS)|(3<<UCSZ0);
}

void USART_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSRA & (1<<UDRE)) );
	/* Put data into buffer, sends the data */
	UDR = data;
}

unsigned char USART_Recieve( void )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSRA & (1<<RXC)) );
	/* Put data into buffer, sends the data */
	return UDR;
}

void ERROR()
{
	USART_Transmit('E');
}
void SLA_Transmit(unsigned char data)
{
	TWBR=0b00100000;//set Baud rate ton 100kHZ
	TWSR=(0<<TWPS1)|(0<<TWPS0);//set prescalar to 1
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	/* setting TWINT, Declaring Master and transmitting Start condition if Bus is available, activating TWI mode*/
	USART_Transmit('1');
	 while (!(TWCR &(1<<TWINT)));/* Wait for START BIT to be transmitted. TWINT is set - START condition has been transmitted.*/
	 USART_Transmit('2');
	 while((TWSR & 0xF8) != 0x08);/*checking for errors*/
    USART_Transmit('3');
	unsigned int a = 0x20;
	TWDR = 0x20; /*Loading address*/
	USART_Transmit('4');
	TWCR = (1<<TWINT) |(1<<TWEN);/*clearing TWINT, setting TWI transmission active*/
	USART_Transmit('5');
	while (!(TWCR &(1<<TWINT)));/*Address transmitted, ACK received*/
	USART_Transmit('6');
	while((TWSR & 0xF8) != 0x18/*MT_SLA_ACK*/);
	TWDR = data; /* Load Data to be sent*/
	USART_Transmit('B');
	TWCR = (1<<TWINT) | (1<<TWEN); /*Transmitiing Data*/
	USART_Transmit('c');
	while (!(TWCR & (1<<TWINT))); /* Data transmitted ACK received*/
	USART_Transmit('d');
	while((TWSR & 0xF8) != 0x28/*MT_SLA_ACK*/);
	USART_Transmit(TWSR & 0xF8);
	USART_Transmit('e');
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO); /* Transmit stop condition*/
	USART_Transmit('f');
	while(!(TWCR & (1<<TWSTO)));
	USART_Transmit('g');
		
}
int main(void)
{
	USART_Init(95);
	while (1) 
    {
		SLA_Transmit('a');
		_delay_ms(100);
    }
}