/*
 * I2C_MASTER_RECEIVE.c
 *
 * Created: 7/15/2017 10:53:43 PM
 * Author : zeus
 */ 
#include <avr/io.h>
#define F_CPU 14745600UL
#include <util/delay.h>
#define BAUDRATE 9600
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
void Slave_init()
{
  TWAR = 0x20; //Defining address
}


unsigned char Slave_receive() 
{
	//TWCR  = (1<<TWEA)|(1<<TWEN);
	/*#if defined(TWCR)
	TWSTO=0;
	TWSTA=0;
	TWEA=1;
	TWEN=1;
	#endif*/
	 
	while((TWSR & 0xF8)!= 0x60)  // Loop till correct acknowledgment have been received
	{
		// Get acknowledgment, Enable TWI, Clear TWI interrupt flag
		TWCR=(1<<TWEA)|(1<<TWEN)|(1<<TWINT); //Sending Acknowgledegment to Master requires TWINT,TWEN also to be set to 1.
		while (!(TWCR & (1<<TWINT)));  // Wait for TWINT flag, Wait until previous action has been completed.
	}
	//Address has been received in TWDR.
	// Clear TWI interrupt flag,Get acknowledgment, Enable TWI
	while((TWSR & 0xF8)!=0x80); // Wait for acknowledgment, Wait until previous action has been completed.
	{
		TWCR= (1<<TWINT)|(1<<TWEA)|(1<<TWEN);  //Sending Acknowgledegment to Master requires TWINT,TWEN also to be set to 1.
		while (!(TWCR & (1<<TWINT))); // Wait for TWINT flag
	}
	//DATA has been received in TWDR.
	
	/*An Alternative
	TWCR= (1<<TWINT)|(1<<TWEA)|(1<<TWEN);  //Sending Acknowgledegment to Master requires TWINT,TWEN also to be set to 1.
	while (!(TWCR & (1<<TWINT))); // Wait for TWINT flag
	//data=TWDR; data will store the value of Address.
	//TWDR has the value of address.
	while((TWSR & 0xF8)!=0x80); // Wait for acknowledgment, Wait until previous action has been completed.
	//DATA has been received in TWDR.*/ //Don't know why this alternative works
	
	unsigned char recv_data;
	return recv_data=TWDR; // Get value from TWDR,data is directly stored in TWDR

}

int main(void)
{
	USART_Init(95);
	Slave_init();
    while (1) 
    {
		unsigned char data_1 = Slave_receive();
		USART_Transmit(data_1);
		_delay_ms(100);
    }
}