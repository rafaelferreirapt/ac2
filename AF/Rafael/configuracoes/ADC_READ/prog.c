#include "detpic32.h"

#define SAMPLES 8

int main(){
	/* ADC */
	TRISBbits.TRISB14 = 1;
	AD1PCFGbits.PCFG14 = 0;

	AD1CON1bits.SSRC = 7;
	AD1CON1bits.CLRASAM = 1;
	AD1CON3bits.SAMC = 16;
	AD1CON2bits.SMPI = SAMPLES-1;

	AD1CHSbits.CH0SA = 14;
	/* FLAGS */
	IFS1bits.AD1IF = 0;
	IPC6bits.AD1IP = 4;
	IEC1bits.AD1IE = 1;
	EnableInterrupts();

	AD1CON1bits.ASAM = 1;
	AD1CON1bits.ON = 1;

	while(1);
	return 0;
}

void _int_(27) isr_adc(void){
	int *p = (int *)(&ADC1BUF0);
	int i = 0, media = 0;

	for(i=0; i<SAMPLES; i++){
		media += p[i*4];
	}
	media /= SAMPLES;

	printInt10(media);
	putChar('\n');

	AD1CON1bits.ASAM = 1;
	IFS1bits.AD1IF = 0;
}
