/*
Aulas resolvidas por Rafael Ferreira e Rodrigo Cunha.
https://github.com/gipmon/ac2
*/
/* PARTE I do guião 7 */
#include <detpic32.h>

int media = 0, v = 0;

void configureAll(void){
	/* ÍNICIO :: Configuração da ADC */
	TRISBbits.TRISB14 = 1;
	AD1PCFGbits.PCFG14 = 0;

	AD1CON1bits.SSRC = 7;

	AD1CON1bits.CLRASAM = 1;

	AD1CON3bits.SAMC = 16;
	AD1CON2bits.SMPI = 7;

	AD1CHSbits.CH0SA = 14;
	AD1CON2bits.SMPI = 7;

	IPC6bits.AD1IP = 4;
	IEC1bits.AD1IE = 1;
	IFS1bits.AD1IF = 0;

	AD1CON1bits.ON = 1;
	/* FIM :: Configuração da ADC */

	/* ÍNICIO :: Configuração do Timer 1 (tipo A) */
	T1CONbits.TCKPS = 3;
	PR1 = 19530;
	TMR1 = 0;
	T1CONbits.TON = 1;

	IFS0bits.T1IF = 0;
	IPC1bits.T1IP = 2;
	IEC0bits.T1IE = 1;

	/* ÍNICIO :: Configuração do  Timer 3 (tipo B) */
	T3CONbits.TCKPS = 2;
	PR3 = 50000;
	TMR3 = 0;
	T3CONbits.TON = 1;

	IFS0bits.T3IF = 0;
	IPC3bits.T3IP = 2;
	IEC0bits.T3IE = 1;
	/* FIM :: Configuração do  Timer 1 (tipo B) */
	EnableInterrupts();
}

int main(){
	configureAll();
	while(1);
	return 0;
}

void _int_(27) isr_adc(void){
	int *p = (int *)(&ADC1BUF0);
	int i;
	for (i = 0; i < 8; i++){
		media += p[i*4];
		v += (p[i*4]*33+511)/1023;
	}

	media /= 8;
	v /= 8;
	IFS1bits.AD1IF = 0;
}

void _int_(4) isr_T1(void){
	AD1CON1bits.ASAM = 1;
	IFS0bits.T1IF = 0;
}

void _int_(12) isr_T3(void){
	printInt(v, 0x00030010);
	putChar('\n');
	IFS0bits.T3IF = 0;
}
