#include <detpic32.h>

int cycle = 0;

int main(){
	T3CONbits.TCKPS = 7;
	PR3 = 39062;
	TMR3 = 0;
	T3CONbits.TON = 1;

	IFS0bits.T3IF = 0;
	IPC3bits.T3IP = 2;
	IEC0bits.T3IE = 1;
	EnableInterrupts();

	while(1);
}

void _int_(12) isr_T3(void){
	if(cycle==0){
		putChar('2');
		cycle++;
	}else if(cycle==20){
		cycle = 0;
	}else{
		cycle++;
	}
	IFS0bits.T3IF = 0;
}
