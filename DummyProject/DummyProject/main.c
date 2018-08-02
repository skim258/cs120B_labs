/*
 * lab3p2.c
 *
 * Created: 8/1/2018 1:28:50 PM
 * Author : ucrcse
 */ 

#include <avr/io.h> 

enum c_val{init, press1, wait1, press2, wait2, open, wrong}door_state;
	 
	void changeC(unsigned char wrongpw){
	 unsigned char door = PINB & 0x01;
	 unsigned char button_x = PINA &0x01;
	 unsigned char button_y = PINA &0x02;
	 unsigned char button_sharp = PINA &0x04;
	
 		 switch(door_state){
			case init:
			 wrongpw = 0;
				door = 0;
				if(PINA) door_state = press1;
			break;
			
			case press1:
				if (PINA != button_y)
				{
					wrongpw = wrongpw | 0x01;
				}
				door_state = wait1;
			break;
			
			case wait1:
				if (!PINA)
				{
					door_state = press2;
				}
			break;
			
			case press2:
				if (PINA != button_sharp)
				{
					wrongpw = wrongpw | 0x01;
					
				}
				if (PINA != 0)
				{
					door_state = wait2;
				}
			break;
				
			case wait2:
				if (!PINA)
				{
					door_state = wrongpw ? wrong : open;
				}
			break;
			
			case open:
			door = 1;
			PORTB = door;
				if (PINA == (PINA | 0x80)) // if pin7 is on
				{
					door_state = init;
				}
			break;
			
			case wrong:
				door_state = init;
			break;
			
			default:
			
			break;
			}
		return;
		};
int main(void)
{
    /* Replace with your application code */
	DDRA = 0x00; PORTA = 0xFF; 
	DDRB = 0xFF; PORTB = 0x00; 
	unsigned char wrongpw;
    while (1) 
    {
		
		changeC(wrongpw);
    }
}

