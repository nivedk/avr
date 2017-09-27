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


// Function to initialize slave
void TWI_init_slave(void)
{
	// Fill slave address to TWAR
	TWAR=0x11;
}


unsigned char TWI_read_slave(void)
{
	
	// Clear TWI interrupt flag,Get acknowledgment, Enable TWI
	TWCR= (1<<TWINT)|(1<<TWEA)|(1<<TWEN);
	// Wait for TWINT flag
	while (!(TWCR & (1<<TWINT)));
	// Wait for acknowledgment
	while((TWSR & 0xF8)!=0x80);
	// Get value from TWDR
	unsigned char rdata=TWDR;
	// send the receive value on PORTB
	return rdata;
}

//Function to match the slave address and slave direction bit(read)
void TWI_match_read_slave(void)
{
	for(int i = 0;i<200;i++){
		USART_Transmit('c');
	}
	TWCR=(1<<TWEA)|(1<<TWEN)|(1<<TWINT);
	// Loop till correct acknowledgment have been received
	while((TWSR & 0xF8)!= 0x60)
	{
		// Get acknowledgment, Enable TWI, Clear TWI interrupt flag
		TWCR=(1<<TWEA)|(1<<TWEN)|(1<<TWINT);
		// Wait for TWINT flag
		while (!(TWCR & (1<<TWINT)));
	}
}

int main(void)
{
	USART_Init(95);
	for(int i = 0;i<200;i++){
		USART_Transmit('b');
	}
	TWI_init_slave(); // Function to initialize slave
	while(1)
	{
		TWI_match_read_slave();
		unsigned char data = TWI_read_slave();
		USART_Transmit(data);
		_delay_ms(100);
	}
}