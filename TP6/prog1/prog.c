#include <detpic32.h>

int main(void)
{
	TRISBbits.TRISB14 = 1;  // RB14 digital output disconnected
	AD1PCFGbits.PCFG14 = 0; // RB14 configured as analog input (AN14)

	AD1CHSbits.CH0SA = 14; /* Selects AN14 as input for the A/D converter */
	AD1CON2bits.SMPI = 0;  /* 1 samples will be converted and stored - CON => Configuration */

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
	AD1CON1bits.ASAM = 1; /* start conversao */
	while(1){
		/* fim da interrupção PC = IPC, guardado quando ocorre a interrupcao */
	}
	return 0;
}

void _int_(27) isr_adc(void){
	printInt(ADC1BUF0, 0x00030010);
	putChar('\n');

	AD1CON1bits.ASAM = 1;
	IFS1bits.AD1IF = 0;
}
