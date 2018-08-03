/*
 * lab3p2.c
 *
 * Created: 8/1/2018 1:28:50 PM
 * Author : ucrcse
 */ 

#include <avr/io.h> 

enum c_val{init, press1, wait1, press2, wait2, open, wrong,wait_lock}door_state;
enum bool{false, true}wrongpw;	 
	void changeC(){
	 unsigned char door = PINB & 0x01;
	 unsigned char button_x = 0x01;
	 unsigned char button_y = 0x02;
	 unsigned char button_sharp = 0x04;
	
 		 switch(door_state){
			case init:
			PORTB = 0;
			wrongpw = false;
				door = 0;
				if(PINA) door_state = press1;
			break;
			
			case press1:
				if (PINA != button_sharp)
				{
					wrongpw = true;
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
				
				if (PINA != 0)
				{
					if (PINA != button_y)
					{
						wrongpw = true;
						
					}
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
					door_state = wait_lock;
				}
			break;
			
			case wrong:
				door_state = init;
			break;
			
			case wait_lock:
				if (!PINA)
				{
					
					door_state = init;
				}
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
    while (1) 
    {
		
		changeC();
    }
}

