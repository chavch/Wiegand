#define F_CPU 16000000
#include <avr/io.h>
#include "c:\users\irakli\Documents\Atmel Studio\7.0\wiegand C\wiegand C\UART.h"
#include <avr/interrupt.h>
#include <util/delay.h>

`unsigned long CardID = 0; // DATA
 int bit_count= 0; //Wiegand type

 int Send;

 int DataArray[26] = {};
 int *ptr = DataArray;

 void WiegandReadD0(void) // INT0 Port PD3
 {
   bit_count++;
   CardID = CardID << 1;
   _delay_ms(1);    
 }
 void WiegandReadD1(void) // INT1 Port PD4
 {
   bit_count++;
   CardID = CardID << 1;
   CardID = CardID + 0x01;
   _delay_ms(1);
 }

 void WiegandSendD0(void) // PD5 Send Data0
 {
   PORTD = (0 << 5);
   _delay_us(20);
   PORTD = (1 << 5);
   _delay_ms(2);
 }
 void WiegandSendD1(void) // PD6 Send Data1
 {
   PORTD = (0 << 6);
   _delay_us(20);
   PORTD = (1 << 6);
   _delay_ms(2);
 }

 void Send_Data()
 {
   unsigned long data;
   int type;

   data = CardID;
   type = bit_count;

   for(int i = 0; i <= (type - 1); i++) // Decimal To Binary Array 
   {
     if(((1 << i) & data) != 0)
     {
        *(ptr + i) = 1;
     }
     else
     {
        *(ptr + i) = 0;
     }
   } // END Of FOR

   for(int i = (type - 1); i >= 0; --i) //Data Send Function
   {
     Send = *(ptr + i);

     if(Send == 1)
        WiegandSendD1();
     if(Send == 0)
        WiegandSendD0();
   }
  }

  int main(void)
  {
    Uart_init();
    sei();

    DDRD = (0 << 3) | (0 << 4); // Input Ports For Reading Wiegand
    PORTD = (0 << 3) | (0 << 4); // No Pull-up
    EIMSK = (1 << INT0) | (1 << INT1);  // Enable External Interrupts On PD3 
                                        // and PD4 

    DDRD = (1 << 5) | (1 << 6); // Output Ports For Sending Wiegand. PD5 = 
                                // D0, PD6 = D1.
    PORTD = (1 << 5) | (1 << 6); // PUll-ups Enable

    while (1) 
    {   
      _delay_ms(1000);
      printf("Card ID = %ld\t Card HEX = %.x\t Wiegand Type = %d\t\n",CardID, CardID, bit_count);
      _delay_ms(500);

      Send_Data();

      CardID = 0;
      bit_count = 0;
    }    
   }

   ISR(INT0_vect)
   {    
      WiegandReadD0();
   }
   ISR(INT1_vect) 
   {    
      WiegandReadD1();
   }`
