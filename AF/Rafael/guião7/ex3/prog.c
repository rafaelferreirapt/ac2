/*
Aulas resolvidas por Rafael Ferreira e Rodrigo Cunha.
https://github.com/gipmon/ac2
*/
#include <detpic32.h>

void setPWM(int pwm){
	OC1RS = ((PR3+1)*pwm)/100;
}

int main(void){

	/* Configurar o timer T3 */
	T3CONbits.TCKPS = 2;
	PR3 = 50000;
	TMR3 = 0;
	T3CONbits.TON = 1;
	/* Configurar o timer T3 */

	/* Configurar o OCR */
	OC1CONbits.OCM = 6;
	OC1CONbits.OCTSEL = 1;
	setPWM(5);
	OC1CONbits.ON = 1;
	/* Configurar o OCR */;

	while(1);
	return 0;
}
