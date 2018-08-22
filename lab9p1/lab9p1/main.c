/*
 * lab9p1.c
 *
 * Created: 8/16/2018 6:43:27 PM
 * Author : Siwon Kim
 */ 

#include <avr/io.h>

void set_PWM(double frequency) {
	static double current_frequency; // Keeps track of the currently set frequency
	
	// Will only update the registers when the frequency changes, otherwise allows
	// music to play uninterrupted.
	if (frequency != current_frequency) {
		if (!frequency) { TCCR3B &= 0x08; } //stops timer/counter

		else { TCCR3B |= 0x03; } // resumes/continues timer/counter
		// prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) { OCR3A = 0xFFFF; }
		// prevents OCR0A from underflowing, using prescaler 64     // 31250 is largest frequency that will not result in underflow
		
		else if (frequency > 31250) { OCR3A = 0x0000; }

		// set OCR3A based on desired frequency
		else { OCR3A = (short)(8000000 / (128 * frequency)) - 1; }
		
		TCNT3 = 0; // resets counter
		current_frequency = frequency; // Updates the current frequency
	}
}

void PWM_on() {
	TCCR3A = (1 << COM3A0);
	// COM3A0: Toggle PB3 on compare match between counter and OCR0A
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	// WGM02: When counter (TCNT0) matches OCR0A, reset counter
	// CS01 & CS30: Set a prescaler of 64
	set_PWM(0);
}

void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}

enum States{init, wait, on, off}state;
unsigned char choose;
unsigned char flag;	// 1 = C, 2 = D, 3 = E

void tick(){
	choose = ~PINA & 0x07;
	switch(state){
		case init:
		//PORTB = 0x00;
		state = wait;
		break;
		
		case wait:
		if(choose == 0x01){
			//flag = 1;
			//set_PWM(261.63);
			state = on;
			}else if(choose == 0x02){
			//flag = 2;
			//set_PWM(293.66);
			state = on;
			}else if(choose == 0x04){
			//flag = 3;
			//set_PWM(329.63);
			state = on;
			}else{
			//flag = 0;
			//set_PWM(0);
			state = off;
		}
		break;
		
		case on:
		//PWM_on();
		//PORTB = 0x40;
		if(choose == 0x01){
			//set_PWM(261.63);
			state = on;
			}else if(choose == 0x02){
			//set_PWM(293.66);
			state = on;
			}else if(choose == 0x04){
			//set_PWM(329.63);
			state = on;
			}else{
			state = off;
		}
		break;
		
		case off:
		//PWM_off();
		//PORTB = 0x00;
		//set_PWM(0);
		state = wait;
		break;
		
		default:
		break;
	}
	
	switch(state){
		case init:
		break;
		
		case wait:
		set_PWM(0);
		break;
		
		case on:
		if(choose == 0x01){
			set_PWM(261.63);
			}else if(choose == 0x02){
			set_PWM(293.66);
			}else if(choose == 0x04){
			set_PWM(329.63);
			}else{
			set_PWM(0);
		}
		break;
		
		case off:
		set_PWM(0);
		break;
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	
	state = init;
	PWM_on();
	
	while (1)
	{
		//PWM_on();
		tick();
	}
}

