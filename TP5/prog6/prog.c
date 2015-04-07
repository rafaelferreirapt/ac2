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
	AD1CON2bits.SMPI = 3; /* 4 samples will be converted and stored */

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

	int i;

	//while(1){
		/* START THE CONVERSION */
		AD1CON1bits.ASAM = 1; /* Start conversion */
		/* Wait while conversion not done (AD1IF == 0) */
		while( IFS1bits.AD1IF == 0 );

		/* Read conversion result (ADC1BUF0 value) and print it */
		int *p = (int *)(&ADC1BUF0);
		int media = 0;
		int v = 0;
		for(i=0; i<4; i++){
			media += p[i*4];
			v += (p[i*4]*33+511)/1023;
		}
		media /= 4;
		printInt(media, 0x0004000A);
		putChar('\n');
		v /= 4;
		printInt(v, 0x0004000A);
		/* Reset AD1IF */
		IFS1bits.AD1IF = 0;
	//}
	return 0;
}
