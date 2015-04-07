/*
Aulas resolvidas por Rafael Ferreira e Rodrigo Cunha.
https://github.com/gipmon/ac2
*/
#include <detpic32.h>

int main(void)
{
	T3CONbits.TCKPS = 7; // 1:256 prescaler (i.e fin = 78,125 KHz)

	PR3 = 39062; // Fout = 20MHz / (32 * (39061,5 + 1)) = 2 Hz
	TMR3 = 0; // Reset timer T3 count register
	T3CONbits.TON = 1; // Enable timer T3 (must be the last command of the // timer configuration sequence)

	while(1){
		while(IFS0bits.T3IF == 0);
		IFS0bits.T3IF = 0;
		putChar('.');
	}
}
