//ATMEGA 32A
/*
 * i2c master.c
 *
 * Created: 7/25/2017 10:57:47 PM
 * Author : Chitrangna
 */ 

#include <avr/io.h>
#define  F_CPU 14745600UL
#include <util/delay.h>
#define  BAUDRATE 9600
#define BAUD_PRESCALER ((F_CPU/16*(BAUDRATE))-1)


void uartinit( unsigned int ubrr)
{
	/*Set baud rate */
	UBRRH = (unsigned char)(ubrr>>8);
	UBRRL = (unsigned char)ubrr;
	UCSRB = (1<<RXEN)|(1<<TXEN);
	/* Set frame format: 8data, 2stop bit */
	UCSRC = (1<<URSEL)|(1<<USBS)|(3<<UCSZ0);
}

void transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSRA & (1<<UDRE)) );
	/* Put data into buffer, sends the data */
	UDR = data;
}


void init(void){
	#if defined(TWPS0)
	TWSR=0x00; //has prescaler=1
	#endif
	TWBR=0b00100000;//set Baud rate ton 100kHZ 
}



int main(void)
{	
	init();
	uartinit( 95/*BAUD_PRESCALER*/);
    static uint8_t eeadr=0x20;
	static uint8_t write=0b00000000;
	while (1) 
    {	
		_delay_ms(100);
		begin:
		TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
		//now twsr will be 0x08
		while (!(TWCR &(1<<TWINT)));		//ind start condn transmitted
		//TWSR 0X18,0X20, or 0x38
		switch(TWSR & 0xF8){
			case 0x08 :
			break;
			
			default:
			goto begin;
		}
		
		//mt mode>>sla+w in twdr
		TWDR=eeadr;		
		TWCR = (1<<TWINT) |(1<<TWEN);
		while (!(TWCR &(1<<TWINT)));		//wait till twint becomes 1
		switch(TWSR & 0xF8){
			case 0x18 :
			break;
			
			default:
			goto begin;
		}
		TWDR = '5';				//set data
		transmit(TWDR);
		TWCR = (1<<TWINT) |(1<<TWEN);
		while (!(TWCR &(1<<TWINT)));
		switch(TWSR & 0xF8){
			case 0x28 :
			break;
			
			default:
			goto begin;
		}
		TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
    }
	return 0;
}