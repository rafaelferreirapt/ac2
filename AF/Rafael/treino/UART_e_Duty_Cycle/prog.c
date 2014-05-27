#include "detpic32.h"

int dutycycle_percentage = 50;

int dutycycle(){
	OC1RS = ((PR3 + 1) * dutycycle_percentage)/100;
}

int main(){
	/* configuração do timer 3 */
	T3CONbits.TCKPS = 2;
	PR3 = 49999;
	TMR3 = 0;
	T3CONbits.TON = 1;

	OC1CONbits.OCM = 6;
	OC1CONbits.OCTSEL = 1;
	dutycycle();
	OC1CONbits.ON = 1;

	/* configuração da UART */
	U1BRG = ((20000000+8*115200)/(16*115200))-1;
	U1MODEbits.BRGH = 0;
	U1MODEbits.PDSEL = 0b00;
	U1MODEbits.STSEL = 0b00;
	U1STAbits.UTXEN = 1;
	U1STAbits.URXEN = 1;
	U1MODEbits.ON = 1;

	U1STAbits.URXISEL = 0b00;
	IEC0bits.U1RXIE = 1;
	IPC6bits.U1IP = 6;
	IEC0bits.U1EIE = 1;
	EnableInterrupts();

	while(1);
	return 0;
}

void _int_(24) isr_uart1(void){
	if(U1RXREG=='+'){
		dutycycle_percentage += 5;
		dutycycle();
	}else if(U1RXREG=='-'){
		dutycycle_percentage -= 5;
		dutycycle();
	}
	printInt10(dutycycle_percentage);
	IFS0bits.U1RXIF = 0;
}
