/*The part about status register stc has not been included
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

void TWI_Init(void){
	USART_Transmit('f');
	//prescaler is set to 1
	TWSR = 0x00;
	//baud rate to 100khz
	TWBR = 0x20;
	//enable TWI - twi takes control of i/o ports etc
	USART_Transmit('g');
	TWCR = (1<<TWEN);
	USART_Transmit('h');
}

void TWI_Start(void){
	//clearing TWINT flag by setting it to 1, TWSTA makes this one the master and generaes start condition if the bus is free, TWEN is to enable TWI communication
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	//im not clear with the while loop bit
	USART_Transmit('i');
	while ((TWCR & (1<<TWINT)) == 0){
		USART_Transmit('r');
	}
	USART_Transmit('j');
	while((TWSR & 0xF8)!= 0x08){
		USART_Transmit('q');
	}
}

void TWI_Stop(void){
	USART_Transmit('k');
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
	USART_Transmit('l');
}

void TWI_WriteAdd(uint8_t add){
	TWDR = add;
	TWCR = (1<<TWINT)|(1<<TWEN);
	USART_Transmit('m');
	while ((TWCR & (1<<TWINT)) == 0){
		USART_Transmit('s');
	}
	USART_Transmit('n');
	while((TWSR & 0xF8)!= 0x18){
		USART_Transmit('t');
	}
}

void TWI_Write(unsigned char data){
	TWDR = data;
	TWCR = (1<<TWINT)|(1<<TWEN);
	USART_Transmit('o');
	while ((TWCR & (1<<TWINT)) == 0){
		USART_Transmit('u');
	}
	USART_Transmit('p');
	while((TWSR & 0xF8) != 0x28){
		USART_Transmit('v');
	}
}

uint8_t TWI_Read(void){
	//same as earlier TWEA enables acknowledgement
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	while ((TWCR & (1<<TWINT)) == 0){
		USART_Transmit('w');
	}
	return TWDR;
}

int main(void){
	USART_Init(95);
	//while(1){
	//	USART_Transmit('z');
	//}
	while(1){
		_delay_ms(100);
		TWI_Init();
		USART_Transmit('a');
		TWI_Start();
		USART_Transmit('b');
		TWI_WriteAdd(0x11);
		USART_Transmit('c');
		TWI_Write('x');
		USART_Transmit('d');
		TWI_Stop();
		USART_Transmit('e');

	}

}