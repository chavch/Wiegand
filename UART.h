#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>



#define BUAD 115200
#define TX_BUFFER_SIZE 128

#ifndef UART_H_
#define UART_H_

static int Uart_PutChar(char c, FILE *stream);
uint8_t Uart_GetChar(void);

 FILE uart_str = FDEV_SETUP_STREAM(Uart_PutChar, NULL, _FDEV_SETUP_RW);


void Uart_init()
{
	UBRR0H =  0x00;
	UBRR0L =  8;  // UBRRn = (F_CPU/16*BAUD) - 1
	
	UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0);
	
	UCSR0C = (1 << UCSZ01 ) | (1 << UCSZ00);
	
	stdout = &uart_str;	
}

static int Uart_PutChar(char c, FILE *stream)
{
	while (!(UCSR0A & (1 << UDRE0)));

	UDR0 = c;

	return 0;
}

uint8_t Uart_GetChar(void)
{	
	while( !(UCSR0A & (1<<RXC0)) );
	return(UDR0);	
}

void Uart_SendByte(char data)
{
	while (!(UCSR0A & (1 << UDRE0)));

	UDR0 = data;	
}

#endif /* UART_H_ */
