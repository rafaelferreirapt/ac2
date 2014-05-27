#include "detpic32.h"

void dutycycle(int dutycycle){
	OC1RS = ((PR2 + 1) * dutycycle)/100;
}

int main(){
	/* configuração da OCR */
	T2CONbits.TCKPS = 5;
	PR2 = 62499;
	TMR2 = 0;
	T2CONbits.TON = 1;

	OC1CONbits.OCM = 6;
	OC1CONbits.OCTSEL = 0;
	dutycycle(50);
	OC1CONbits.ON = 1;
	while(1);
	return 0;
}