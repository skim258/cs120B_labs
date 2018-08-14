#include <avr/io.h>
//#include "RIMS.h"
#include "timer.c"
#define B3 (PORTB & 0x08)
unsigned char tmpB = 1;
enum ThreeLED_States { ThreeLED_Start, ONE, TWO } ThreeLED_State;

void ThreeLED_Tick() {
	switch(ThreeLED_State) {
		
		case ThreeLED_Start:
		ThreeLED_State = ONE;
		PORTB = tmpB;
		break;
		
		case ONE:
			if(tmpB < 3){
			PORTB =  B3 | (tmpB << 1);
			tmpB = tmpB << 1;
		}
		if((tmpB << 1) > 5){
		ThreeLED_State = TWO;
		}
		break;
		
		case TWO:
		tmpB = 1;
		PORTB = (B3 | tmpB);
		ThreeLED_State = ONE;
		break;
		
	}
}

enum BlinkingLED_States { BlinkingLED_Start, OFF, ON } BlinkingLED_State;
void BlinkingLED_Tick() {
	switch(BlinkingLED_State) {
		case BlinkingLED_Start:
		//PORTB = 0;
		BlinkingLED_State = OFF;
		break;
		
		case OFF:
		PORTB = PORTB & 0xF7;
		BlinkingLED_State = ON;
		break;
		
		case ON:
		PORTB = PORTB | 0x08;
		BlinkingLED_State = OFF;
		break;
	}
}




int main(void) {
	DDRB = 0xFF; PORTB = 0x00;
	
	unsigned long ThreeLED_elapsedTime = 0;
	unsigned long BlinkingLED_elapsedTime = 0;
	const unsigned long timerPeriod = 12;
	//B = 0;
	TimerSet(timerPeriod);
	TimerOn();

	while (1) {
		if (ThreeLED_elapsedTime >= 38) {
			ThreeLED_Tick();
			ThreeLED_elapsedTime = 0;
		}
		if (BlinkingLED_elapsedTime >= 125) {
			
			BlinkingLED_Tick();
			BlinkingLED_elapsedTime = 0;
		}
		while (!TimerFlag){} // Wait for timer period
		TimerFlag = 0; // Lower flag raised by timer


		ThreeLED_elapsedTime += timerPeriod;
		BlinkingLED_elapsedTime += timerPeriod;
	}
}