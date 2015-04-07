/*
Aulas resolvidas por Rafael Ferreira e Rodrigo Cunha.
https://github.com/gipmon/ac2
*/
#include <detpic32.h>

void delay(unsigned int);
void send2displays(unsigned char);
unsigned char toBcd(unsigned char);

volatile unsigned char value2display = 0; //Global variable

int main(void)
{
	int i;

	TRISBbits.TRISB14 = 1;  // RB14 digital output disconnected
	AD1PCFGbits.PCFG14 = 0; // RB14 configured as analog input (AN14)

	AD1CHSbits.CH0SA = 14; /* Selects AN14 as input for the A/D converter */
	AD1CON2bits.SMPI = 7;  /* 1 samples will be converted and stored - CON => Configuration */

	/* 3 instruções q são sempre necessárias por */
	AD1CON1bits.SSRC = 7;
	AD1CON1bits.CLRASAM = 1; /* clear ASAM */
	AD1CON3bits.SAMC = 16;

	/* Enable A/D converter */
	AD1CON1bits.ON=1;

	IFS1bits.AD1IF = 0; /* A flag status e usada para dizer quando  conclui a conversao */
	IPC6bits.AD1IP = 1; /* Interrupt Priority Control6 e nesses bits e devemos fazer set do valor da prioridade
	pode ir de 1 a 7 mas o 7 e reservado para interrupcoes de sistema */
	IEC1bits.AD1IE = 1; /* Fazer eneble da AD1 interrupt */

	EnableInterrupts(); /* interrupts global */

	i = 0;

	while(1){
		delay(10);
		if(i++ == 25)		//250ms
		{
			AD1CON1bits.ASAM = 1; /* start conversao */
			i = 0;
		}
		send2displays(value2display);
	}
	return 0;
}

void _int_(27) isr_adc(void){

	int *p = (int *)(&ADC1BUF0);
	int i, media = 0, v = 0;

	for(i=0; i<8; i++){
		media += p[i*4];
		v += (p[i*4]*33+511)/1023;
	}
	media /= 8;
	v /= 8;

	value2display = toBcd(v);

	IFS1bits.AD1IF = 0;
}
