/*
 * spi_slave.c
 *
 * Created: 09-08-2017 17:55:13
 * Author : nived
 */ 

#include <avr/io.h>
#define F_CPU 14745600UL
#include <util/delay.h>
#define BAUDRATE 9600
#define BAUD_PRESCALER 95
void SPI_SlaveInit(void)
{
	/* Set MISO output, all others input */
	DDRB = 0b01000000;
	/* Enable SPI */
	SPCR = (1<<SPE);
}
void USART_Init( unsigned int ubrr)
{
	/*Set baud rate */
	UBRRH = (unsigned char)(95>>8);
	UBRRL = (unsigned char)95;
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
unsigned char SPI_SlaveReceive(void)
{
	/* Wait for reception complete */
	while(!(SPSR & (1<<SPIF)))
	{
		;
	}
	/* Return Data Register */
	return SPDR;
}
//void SPI_SlaveTransmit(char cData)
//{
///* Start transmission */
//SPDR = cData;
///* Wait for transmission complete */
//while(!(SPSR & (1<<SPIF)));
//}
int main(void)
{
	//SPI_SlaveInit();
	USART_Init(BAUD_PRESCALER);
	// Enable the USART Recieve Complete interrupt (USART_RXC)
	/* Replace with your application code */
	for (int a = 0; a<1000; a++)
	USART_Transmit('w');
	}
	
	while (1)
	{
		unsigned char x;
		for (int a = 0; a<1000; a++)
			USART_Transmit('w');
		}
		
		x=SPI_SlaveReceive();
		USART_Transmit(x);
		_delay_ms(100);
	}
}

