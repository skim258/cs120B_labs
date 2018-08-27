/*
 * lab10_part3.c
 *
 * Created: 8/21/2018 5:31:02 PM
 * Author : Daniel, Siwon 
 */ 

#include <avr/io.h>
#include "scheduler.h"
#include "keypad.h"
#include "timer.h"
#include "io.h"
#include "io.c"

enum Keypad_States { init, press };

unsigned char key;

int KeypadSM(int state){
	
	switch(state){
		case init:
		state = press;
		break;
		
		case press:
		key = GetKeypadKey();
		if(key != '\0'){
			LCD_Cursor(1);
			switch (key) {
				case '\0': LCD_WriteData(' '); break;
				case '0': LCD_WriteData('0'); break;
				case '1': LCD_WriteData('1'); break;
				case '2': LCD_WriteData('2'); break;
				case '3': LCD_WriteData('3'); break;
				case '4': LCD_WriteData('4'); break;
				case '5': LCD_WriteData('5'); break;
				case '6': LCD_WriteData('6'); break;
				case '7': LCD_WriteData('7'); break;
				case '8': LCD_WriteData('8'); break;
				case '9': LCD_WriteData('9'); break;
				case 'A': LCD_WriteData('A'); break;
				case 'B': LCD_WriteData('B'); break;
				case 'C': LCD_WriteData('C'); break;
				case 'D': LCD_WriteData('D'); break;
				case '*': LCD_WriteData('*'); break;
				case '#': LCD_WriteData('#'); break;
				default: PORTB = 0x1B; break;
			}
		}
		state = press;
		break;
	}
	return state;
}

int main(void)
{
	DDRA = 0xFF; PORTA = 0x00;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xF0; PORTC = 0x0F;
	DDRD = 0xFF; PORTD = 0x00;
	
	unsigned long int Keypad_TaskCalc = 10;
	
	unsigned long int tmpGCD = 1;
	tmpGCD = findGCD(Keypad_TaskCalc, Keypad_TaskCalc);
	
	unsigned long int GCD = tmpGCD;
	
	unsigned long int Keypad_Period = Keypad_TaskCalc/GCD;
	
	static task Keypad_task;
	task *tasks[] = {&Keypad_task};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	
	Keypad_task.state = init;
	Keypad_task.period = Keypad_Period;
	Keypad_task.elapsedTime = Keypad_Period;
	Keypad_task.TickFct = &KeypadSM;
	
	LCD_init();
	LCD_ClearScreen();
	
	TimerSet(GCD);
	TimerOn();
	
	unsigned short i;
	
	while (1)
	{
		for(i = 0; i < numTasks; i++) {
			// task is ready to tick
			if(tasks[i]->elapsedTime == tasks[i]->period) {
				// setting next state for task
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				// reset the elapsed time for the next tick
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}
		while(!TimerFlag);      // wait for a period
		TimerFlag = 0;          // reset TimerFlag
	}
}
