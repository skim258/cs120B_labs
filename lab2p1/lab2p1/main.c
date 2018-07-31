/*
 * lab2p1.c
 *
 * Created: 7/31/2018 1:23:05 PM
 * Author : ucrcse
 */ 

#include <avr/io.h>


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0x00; PORTB = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	
    /* Replace with your application code */
    while (1) 
    {
		unsigned char tmpA = PINA;
		unsigned char tmpB = PINB;
		unsigned char cntr = 0;
		while (tmpA > 0){
			if(tmpA%2 == 1){
				cntr++;
			}
			tmpA >> 1;
		}
		while (tmpB > 0){
			if(tmpB%2 == 1){
				cntr++;
			}
			tmpB >> 1;
		}
		PORTC = cntr;
    }
}

