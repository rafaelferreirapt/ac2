/*
Aulas resolvidas por Rafael Ferreira e Rodrigo Cunha.
https://github.com/gipmon/ac2
*/
#include <detpic32.h>

int fcp = 0;

int main(void)
{
	T3CONbits.TCKPS = 7; // 1:256 prescaler (i.e fin = 78,125 KHz)

	PR3 = 39062; // Fout = 20MHz / (32 * (39061,5 + 1)) = 2 Hz
	TMR3 = 0; // Reset timer T3 count register
	T3CONbits.TON = 1; // Enable timer T3 (must be the last command of the // timer configuration sequence)

	// configuraçao das interrupts tabela 7.1
	IPC3bits.T3IP = 4; // Configurar a prioridade relativa
	IFS0bits.T3IF = 0; // Reset do Instruction Flag
	IEC0bits.T3IE = 1; // Autorizar a interrupção
	EnableInterrupts();// Ativar globalmente as interrupções
	// fim da configuraçao das interrupts

	while(1){}
}
void _int_(12) isr_T3(void){
	if(fcp){
		putChar('.');
		fcp = 0;
	}else{
		fcp = 1;
	}
	IFS0bits.T3IF = 0;
}
