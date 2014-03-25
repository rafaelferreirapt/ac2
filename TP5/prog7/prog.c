#include <detpic32.h>

void delay(unsigned int);
void send2displays(unsigned char);
unsigned char toBcd(unsigned char);

int main(void)
{

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

	int i = 0, media = 0, v = 0;

	while(1){

		delay(10); //Wait 10 ms using the core timer

		if(i++ == 25){//250ms

			v = media = 0;

			/* START THE CONVERSION */
			AD1CON1bits.ASAM = 1; /* Start conversion */
			/* Wait while conversion not done (AD1IF == 0) */
			while( IFS1bits.AD1IF == 0 );

			/* Read conversion result (ADC1BUF0 value) and print it */
			int *p = (int *)(&ADC1BUF0);

			for(i=0; i<4; i++){
				media += p[i*4];
				v += (p[i*4]*33+511)/1023;
			}
			media /= 4;
			//printInt(media, 0x0004000A);
			//putChar('\n');
			v /= 4;
			//printInt(v, 0x0004000A);
			/* Reset AD1IF */
			IFS1bits.AD1IF = 0;

			i = 0;
			v = toBcd(v);
		}

		send2displays(v);

	}
	return 0;
}
