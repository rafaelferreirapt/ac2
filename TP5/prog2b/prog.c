/*
Aulas resolvidas por Rafael Ferreira e Rodrigo Cunha.
https://github.com/gipmon/ac2
*/
#include <detpic32.h>

int main(void)
{
	TRISBbits.TRISB14 = 1; // RB14 digital output disconnected
	AD1PCFGbits.PCFG14 = 0; // RB14 configured as analog input (AN14)

	AD1CHSbits.CH0SA = 14; /* Selects AN14 as input for the A/D converter */
	AD1CON2bits.SMPI = 0; /* 1 samples will be converted and stored
							CON => Configuration */

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

	while(1){
		/* START THE CONVERSION */
		AD1CON1bits.ASAM = 1; // Start conversion <= INICIAR necessario
		/* Wait while conversion not done (AD1IF == 0) */
		while(IFS1bits.AD1IF == 0);
		/* Read conversion result (ADC1BUF0 value) and print it BUTx => BUFFER x
		   quantas conversoes pedimos mais vao para o BUF ou seja, se pedirmos SMPI = 0 fica com
		   o BUF0
		   Se mandrmos fazer 2 conversoes devemos ler os bufferes respectivos de todas as conversoes
		 */
		printInt(ADC1BUF0, 0x00030016);
		/* Imprime o resultado da conversão */
		putChar(0xD);
		/* Reset AD1IF F => Flag */
		IFS1bits.AD1IF = 0;
	}

	return 0;
}
