//ATMEGA 32A
/*
 * i2c slave.c
 *
 * Created: 29-Jul-17 11:08:45 PM
 * Author : Chitrangna
 */ 

#include <avr/io.h>
#include <util/twi.h>		
//#include <util/delay.h>
//#ifndef F_CPU			//its already defined so wont write this way- we need to change fcpu
#define F_CPU 14745600UL                     //clock frequency
#include <util/delay.h>
#define BAUD 9600                            //9600 bps
#define BAUDRATE (F_CPU/(16UL*BAUD)-1)    

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
	TWAR = 0x20;
	//TWCR = (1<<TWEA)|(1<<TWEN);//this works as well instead of the line following 
	TWCR = (1<<TWEA)|(1<<TWEN)|(0<<TWSTO)|(0<<TWSTA);//extra definition- without it also works
}


int main(void)
{
    init();
	uartinit( 95/*BAUD_PRESCALER*/);
    //unsigned char send='t';
    while (1) 
    {
    	begin:
		TWCR = (1<<TWINT)|(1<<TWEA)|(1<<TWEN);
		while (!(TWCR &(1<<TWINT)));//&(1<<TWEA)));	//twea isnt high after sending ack 	
		switch(TWSR & 0xF8)
		{
			case 0x60 :
			break;

			default:
			goto begin;
		}
		
		TWCR = (1<<TWINT)|(1<<TWEA)|(1<<TWEN); 
		//_delay_ms(100);
		while (!(TWCR &(1<<TWINT)));    //wait for the transmit to end
		

		switch(TWSR & 0xF8)				//checking whether ack for data is sent or not
		{
		case 0x80 :
		break;
		
		default:
		goto begin;
		}
		unsigned char send;			//this should be after sending ack since before it TWDR has slave address stored in it. data comes into it only after sending ack 
		send=TWDR;
		
		transmit(send);
		_delay_ms(100);

    }
    return 0;
}