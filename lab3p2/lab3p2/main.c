/*
 * lab3p2.c
 *
 * Created: 8/1/2018 1:28:50 PM
 * Author : ucrcse
 */ 

#include <avr/io.h> 

enum led_states{init,wait01, pb1,wait10, wait11, pb0, wait00}led_state;
	
	void led_list(){
	  unsigned char button = PINA & 0x01;
	  unsigned char tmpB = 0x00;
 		switch(led_state){
			case init:
				PORTB = tmpB | 0x01;		//first led on
				led_state = wait01;
			break;
			
			case wait01:
				if (button)
				{
					led_state = pb1;
				}
				else{
					led_state = wait01;
				}
			break;
			
			case pb1:
				led_state = wait10;
				PORTB = tmpB | 0x02;   //2nd led on
			break;
			
			case wait10:
				if(!button){
					led_state = wait11;
				}
			break;

			case wait11:
				if(button){
					led_state = pb0;
				}
			break;
			
			case pb0:
				led_state = wait00;
				PORTB = tmpB | 0x01; //1st led on
			break;

			case wait00:
				if (!button)
				{
					led_state = wait01;
				}
			break;

			default:
			led_state = init;
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
		led_list();
    }
}

