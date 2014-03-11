#include <detpic32.h>

void delay(unsigned int);

int main(void){
	int i;
	TRISB = 0x0000;
	PORTB = 0x0000;
	LATBbits.LATB8 = 0;
	LATBbits.LATB9 = 1;
	while(1){
		
	}
}
