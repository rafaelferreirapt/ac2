#include <detpic32.h>

void delay(unsigned int);
void send2displays(unsigned char);
unsigned char toBcd(unsigned char);
void configureAll();

volatile unsigned char value2display = 0; //Global variable

int main(void)
{
	configureAll();
	EnableInterrupts();

	TRISEbits.TRISE4 = 1;
	TRISEbits.TRISE5 = 1;

	while(1){
		if(PORTEbits.RE4==1 && PORTEbits.RE5==0){
			IEC0bits.T1IE = 0; // Autorizar a interrupção
		}else{
			IEC0bits.T1IE = 1; // Autorizar a interrupção
		}
	}
	return 0;
}

void configureAll(){
	/*	Configuration of AD */
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

	/*	Configuração of AD */

	/*	Configuração Timer 3 */

	T3CONbits.TCKPS = 2;

	PR3 = 49999; // Fout = 20MHz / (32 * (39061,5 + 1)) = 2 Hz
	TMR3 = 0; // Reset timer T3 count register
	T3CONbits.TON = 1; // Enable timer T3 (must be the last command of the // timer configuration sequence)

	// configuraçao das interrupts tabela 7.1
	IPC3bits.T3IP = 4; // Configurar a prioridade relativa
	IFS0bits.T3IF = 0; // Reset do Instruction Flag
	IEC0bits.T3IE = 1; // Autorizar a interrupção

	/*	Configuração Timer 3 */

	/*	Configuração Timer 1 */

	T1CONbits.TCKPS = 3;

	PR1 = 19530; // Fout = 20MHz / (256 * (39061,5 + 1)) = 2 Hz
	TMR1 = 0; // Reset timer T3 count register
	T1CONbits.TON = 1; // Enable timer T3 (must be the last command of the // timer configuration sequence)

	// configuraçao das interrupts tabela 7.1
	IPC1bits.T1IP = 4; // Configurar a prioridade relativa
	IFS0bits.T1IF = 0; // Reset do Instruction Flag
	IEC0bits.T1IE = 1; // Autorizar a interrupção

	/*	Configuração Timer 1 */
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

void _int_(4) isr_T1(void){
	AD1CON1bits.ASAM = 1; /* start conversao */
	IFS0bits.T1IF = 0;
}

void _int_(12) isr_T3(void){
	send2displays(value2display);
	IFS0bits.T3IF = 0;

}
