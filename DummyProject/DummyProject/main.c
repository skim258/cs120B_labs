/*
 * lab10_part2.c
 *
 * Created: 8/21/2018 5:25:17 PM
 * Author : Daniel
 */  

#include <avr/io.h>
#include <avr/interrupt.h>
#include <bit.h>
#include <timer.h>
#include <stdio.h>
#include "io.h"
#include "io.c"

//--------Find GCD function --------------------------------------------------
unsigned long int findGCD(unsigned long int a, unsigned long int b)
{
	unsigned long int c;
	while(1){
		c = a%b;
		if(c==0){return b;}
		a = b;
b = c;
	}
	return 0;
}
//--------End find GCD function ----------------------------------------------

//--------Task scheduler data structure---------------------------------------
// Struct for Tasks represent a running process in our simple real-time operating system.
typedef struct _task {
	/*Tasks should have members that include: state, period,
		a measurement of elapsed time, and a function pointer.*/
	signed char state; //Task's current state
	unsigned long int period; //Task period
	unsigned long int elapsedTime; //Time elapsed since last task tick
	int (*TickFct)(int); //Task tick function
} task;

//--------End Task scheduler data structure-----------------------------------

//--------Shared Variables----------------------------------------------------
//unsigned char SM2_output = 0x00;
//unsigned char SM3_output = 0x00;
unsigned char pause = 0;

//--------End Shared Variables------------------------------------------------

//--------User defined FSMs---------------------------------------------------

enum SM4_States { SM4_display };
const unsigned char array[] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'C', 'S', '1', '2', '0', 'B', ' ', 'i', 's', ' ', 'L', 'e', 'g', 'e', 'n',
'd', '.', '.', '.', ' ', 'w', 'a', 'i', 't', ' ', 'f', 'o', 'r', ' ', 'i', 't', ' ', 'D', 'A', 'R', 'Y', '!'};
unsigned char array_index = 0;
// Combine blinking LED outputs from SM2 and SM3, and output on PORTB
int ScrollSM(int state) {
	// Local Variables

	//unsigned char output;

	//State machine transitions

	switch(state) {
		case SM4_display:	
			//LCD_WriteData(string[array_index] + '0');
			//array_index++;
			
			if(array_index >= 52){
				array_index = 0;
			}else{
				array_index++;
			}
			break;

		default:		
			break;
	}
	return state;
}

enum Cursor_SM{start, init, something};

int CursorSM(int state){
	static unsigned char x, y;

	switch(state){	
		case start:
		state = init;
			break;
		
		case init:
		state = something;
		x = 1;
		y = array_index;
		LCD_Cursor(1);
			break;
		
		case something:
			//LCD_Cursor(16);
			if(y >= sizeof(array)){ //char itself is 1byte
				LCD_WriteData(' ');
			}else{
				LCD_WriteData(array[y]);
			}
			
			if(x >= 16){
				state = SM4_display;
			}else{
				x++;
				y++;
			}
			
			break;
			
	}
	
	
	return state;
}

// --------END User defined FSMs-----------------------------------------------

// Implement scheduler code from PES.
int main()
{
	// Set Data Direction Registers
	// Buttons PORTA[0-7], set AVR PORTA to pull down logic
	DDRA = 0xFF; PORTA = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	// . . . etc
	//LCD_init();

	// Period for the tasks
	unsigned long int Scroll_calc = 200;
	unsigned long int Cursor_calc = 5;

	//Calculating GCD
	unsigned long tempGCD = findGCD(Scroll_calc, Cursor_calc);

	//Greatest common divisor for all tasks or smallest time unit for tasks.
	unsigned long int GCD = tempGCD;

	//Recalculate GCD periods for scheduler
	unsigned long int Scroll_period = Scroll_calc/GCD;
	unsigned long int Cursor_period = Cursor_calc/GCD;

	//Declare an array of tasks 
	static task ScrollTask, CursorTask;
	task *tasks[] = { &ScrollTask, &CursorTask };
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	// Task 1
	ScrollTask.state = SM4_display;//Task initial state.
	ScrollTask.period = Scroll_period;//Task Period.
	ScrollTask.elapsedTime = Scroll_period;//Task current elapsed time.
	ScrollTask.TickFct = &ScrollSM;//Function pointer for the tick.
	//t2
	CursorTask.state = start;
	CursorTask.period = Cursor_period;
	CursorTask.elapsedTime = Cursor_period;
	CursorTask.TickFct = &CursorSM;

	// Set the timer and turn it on
	LCD_init();
	LCD_ClearScreen();
	LCD_Cursor(1);
	LCD_WriteData(0 + '0');
	
	TimerSet(GCD);
	TimerOn();

	unsigned short i; // Scheduler for-loop iterator
	while(1) {
		// Scheduler code
		for ( i = 0; i < numTasks; i++ ) {
			// Task is ready to tick
			if ( tasks[i]->elapsedTime == tasks[i]->period ) {
				// Setting next state for task
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				// Reset the elapsed time for next tick.
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}
		while(!TimerFlag);
		TimerFlag = 0;
	}
	return 0;
}
