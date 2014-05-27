#include "detpic32.h"

int sequence[] = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xF};
int i = 0;

#define SIZE 9

int main(){
	TRISE = (TRISE & 0xF0);
	/* configurar os timers */
	T2CONbits.TCKPS = 4;
	PR2 = 62499;
	TMR2 = 0;
	T2CONbits.TON = 1;

	IFS0bits.T2IF = 0;
	IPC2bits.T2IP = 2;
	IEC0bits.T2IE = 1;
	EnableInterrupts();

	while(1);
	return 0;
}

void _int_(8) isr_T2(void){
	LATE = (LATE & 0xF0) | sequence[i];
	if(i>=SIZE){
		i = 0;
	}else{
		i++;
	}
	IFS0bits.T2IF = 0;
}
