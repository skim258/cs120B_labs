#include <avr/io.h>
#include <seven_seg.h>
int main(void)
{
	
	DDRC = 0xFF; PORTC = 0x00;
	while(1){
	PORTC = 0x30;
	}
}
