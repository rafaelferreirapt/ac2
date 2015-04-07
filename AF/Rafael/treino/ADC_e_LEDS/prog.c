/*
Aulas resolvidas por Rafael Ferreira e Rodrigo Cunha.
https://github.com/gipmon/ac2
*/
#include "detpic32.h"

#define SAMPLES 4;

int flag = 1;

int main(){
	/* configuração dos LEDS */
	TRISEbits.TRISE0 = 0;
	TRISEbits.TRISE1 = 0;
	TRISEbits.TRISE2 = 0;
	TRISEbits.TRISE3 = 0;
	LATE = (LATE & 0xF0);

	/* ADC configuration */
	TRISBbits.TRISB14 = 1;
	AD1PCFGbits.PCFG14 = 0;

	AD1CON1bits.SSRC = 7;
	AD1CON1bits.CLRASAM = 1;
	AD1CON3bits.SAMC = 16;
	AD1CON2bits.SMPI = SAMPLES -1;
	AD1CHSbits.CH0SA = 14;
	AD1CON1bits.ON = 1;

	IPC6bits.AD1IP = 6;
	IEC1bits.AD1IE = 1;
	IFS1bits.AD1IF = 0;
	EnableInterrupts();

	/* configuração do TIMER */
	T2CONbits.TCKPS = 4;
	PR2 = 49999;
	TMR2 = 0;

	IFS0bits.T2IF = 0;
	IPC2bits.T2IP = 5;
	IEC0bits.T2IE = 1;

	AD1CON1bits.ASAM = 1;

	while(1);
	return 0;
}

void _int_(27) isr_adc(void){
	int *p = (int *)(&ADC1BUF0);
	int i, media=0;
	for (i = 0; i < 8; i++){
		media += p[i*4];
	}

	media /= 8;

	/* colocar nos leds */
	/*
	if(media>=0x3FE){
		LATE = (LATE & 0xF0) | 0x0F;
	}else if(media >= 3*0xFF){
		LATE = (LATE & 0xFF) | 0x0E;
	}else if(media >= 2*0xFF){
		LATE = (LATE & 0xF0) | 0x0C;
	}else if(media >= 0xFF){
		LATE = (LATE & 0xF0) | 0x08;
	}else{
		LATE = (LATE & 0xF0);
	}
	*/

	if(media>400){
		T2CONbits.TON = 1;
	}else{
		T2CONbits.TON = 0;
		LATE = (LATE & 0xF0);
	}

	printInt10(media);
	putChar('\n');

	IFS1bits.AD1IF = 0;
	AD1CON1bits.ASAM = 1;
}

void _int_(8) isr_T2(void){
	if(flag){
		LATEbits.LATE0 = 1;
		LATEbits.LATE1 = 1;
		LATEbits.LATE2 = 1;
		flag = 0;
	}else{
		LATEbits.LATE0 = 0;
		LATEbits.LATE1 = 0;
		LATEbits.LATE2 = 0;
		flag = 1;
	}

	IFS0bits.T2IF = 0;
}
