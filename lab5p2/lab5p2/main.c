/*
 * lab05_part1.c
 *
 * Created: 8/7/2018 1:22:22 PM
 * Author : Daniel
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#define A0 (~PINA & 0x01)
volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks

void TimerOn() {
	// AVR timer/counter controller register TCCR1
	TCCR1B = 0x0B;// bit3 = 0: CTC mode (clear timer on compare)
	// bit2bit1bit0=011: pre-scaler /64
	// 00001011: 0x0B
	// SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks/s
	// Thus, TCNT1 register will count at 125,000 ticks/s

	// AVR output compare register OCR1A.
	OCR1A = 125;	// Timer interrupt will be generated when TCNT1==OCR1A
	// We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
	// So when TCNT1 register equals 125,
	// 1 ms has passed. Thus, we compare to 125.
	// AVR timer interrupt mask register
	TIMSK1 = 0x02; // bit1: OCIE1A -- enables compare match interrupt

	//Initialize avr counter
	TCNT1=0;

	_avr_timer_cntcurr = _avr_timer_M;
	// TimerISR will be called every _avr_timer_cntcurr milliseconds

	//Enable global interrupts
	SREG |= 0x80; // 0x80: 1000000
}

void TimerOff() {
	TCCR1B = 0x00; // bit3bit1bit0=000: timer off
}

void TimerISR() {
	TimerFlag = 1;
}

// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect) {
	// CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--; // Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) { // results in a more efficient compare
		TimerISR(); // Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

enum PORTB_states { INIT, ONE, TWO, THREE, right_press, wait_reset } PORTB_State;

void PORTB_Tick() {
	//B0 = 0x01; B1 = 0x02; B2 = 0x04;
	
	switch(PORTB_State) {
		case INIT:
		PORTC = 0x00;
		PORTB_State = ONE;
		break;
		
		case ONE:
		PORTC = 0x01;
		PORTB_State = TWO;
		if (A0)
		{
			PORTB_State = right_press;
			
		}
		break;
		
		case TWO:
		PORTC = 0x02;
		PORTB_State = THREE;
		if (A0)
		{
			PORTB_State = right_press;
			
		}
		break;
		
		case THREE:
		PORTC = 0x04;
		PORTB_State = ONE;
		if (A0)
		{
			PORTB_State = right_press;
			
		}
		break;
		
		case right_press :
			PORTB_State  = A0 ? right_press : wait_reset;
		break;
		
		case wait_reset : 
			PORTB_State = !A0 ? wait_reset : ONE;
		break;
	}
	
	
}

int main(void)
{
	DDRC = 0xFF; // Set port B to output
	PORTC = 0x00; // Init port B to 0s
	DDRA = 0x00; PORTA = 0xFF;
	TimerSet(38);
	TimerOn();
//	unsigned char tmpB = 0x00;
//	PORTB_State = ONE;
	while(1) {
		// User code (i.e. synchSM calls)
//		tmpB = ~tmpB;	// Toggle PORTB; Temporary, bad programming style
//		PORTB = tmpB;
		PORTB_Tick();
	    while (!TimerFlag);	// Wait 1 sec
		TimerFlag = 0;
		// Note: For the above a better style would use a synchSM with TickSM()
		// This example just illustrates the use of the ISR and flag
	}
}

