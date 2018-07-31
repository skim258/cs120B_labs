/*
 * lab2p2.c
 *
 * Created: 7/31/2018 2:04:25 PM
 * Author : ucrcse
 */ 

#include <avr/io.h>

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

int main(void)
{
    /* Replace with your application code */
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port B's 8 pins as outputs,
    while (1) {
		unsigned char fuelA = PINA & 0x0F;
		unsigned char tmpC = 0x3F;
		//unsigned char fuelC
		if (fuelA < 13){
			SetBit(tmpC,5,0);
			if (fuelA <10){
				SetBit(tmpC,4,0);
				if (fuelA <7){
					SetBit(tmpC,3,0);
					if (fuelA < 5){
						SetBit(tmpC,2,0);
						if (fuelA < 3){
							SetBit(tmpC,1,0);
						}
					}
				}
			} 
		  }
			PORTC = tmpC;
		}
}

