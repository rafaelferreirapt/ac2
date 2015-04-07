/*
Aulas resolvidas por Rafael Ferreira e Rodrigo Cunha.
https://github.com/gipmon/ac2
*/
#include <detpic32.h>

int main(void)
{
	volatile int aux;

	TRISBbits.TRISB14 = 1; /* RB14 digital output disconnected */
	AD1PCFGbits.PCFG14 = 0; /* RB14 configured as analog input (AN14) */

	AD1CHSbits.CH0SA = 14; /* Selects AN14 as input for the A/D converter */
	AD1CON2bits.SMPI = 1; /* 4 samples will be converted and stored */

	/* Conversion trigger selection bits: in this
	// mode an internal counter ends sampling and
	// starts conversion*/
	AD1CON1bits.SSRC = 7;
	/* // Stop conversions when the 1st A/D converter
	// interrupt is generated. At the same time,
	// hardware clears the ASAM bit*/
	AD1CON1bits.CLRASAM =1;
	/* // Sample time is 16 TAD (TAD = 100 ns) */
	AD1CON3bits.SAMC = 16;

	/* Enable A/D converter */
	AD1CON1bits.ON=1;

	TRISEbits.TRISE0 = 0; /* configura RE0 como saida digital */

	while(1){
		LATEbits.LATE0 = 1; /* set RE0 */

		/* START THE CONVERSION */
		AD1CON1bits.ASAM = 1; /* Start conversion */
		/* Wait while conversion not done (AD1IF == 0) */
		while( IFS1bits.AD1IF == 0 );
		/* Reset RE0 */
		LATEbits.LATE0 = 0; /* reset RE0 /*
		/* Read conversion result (ADC1BUF0 value) and print it */
		aux = ADC1BUF0; /* guarda o valor da conversão numa variável auxiliar */
		aux = ADC1BUF1; /* guarda o valor da conversão numa variável auxiliar */
		/* Imprime o resultado da conversão */
		//putChar('\n'); 7.2 quadrados * 500 ns
		/* Reset AD1IF */
		IFS1bits.AD1IF = 0;
	}
	return 0;
}
