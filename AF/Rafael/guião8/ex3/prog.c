#include <detpic32.h>

void putc(char byte2send){
	while(U1STAbits.UTXBF);
	U1TXREG = byte2send;
}

void configUart(){
	//115200, 'N', 1
	U1BRG = ((20000000+8*115200)/(16*115200))-1;
	U1MODEbits.BRGH = 0;
	U1MODEbits.PDSEL = 0b00;
	U1MODEbits.STSEL = 0;
	U1STAbits.UTXEN = 1;
	U1STAbits.URXEN = 1;
	U1MODEbits.ON = 1;
	//UARTE INTERRUPTS
	U1STAbits.URXISEL = 0b00;
	IEC0bits.U1RXIE = 1;
	IPC6bits.U1IP = 3;
	IEC0bits.U1EIE = 1;
}


int main(void){
	configUart();
	EnableInterrupts();
	while(1);
}

void _int_(24) isr_uart1(void){
	putc(U1RXREG);
	IFS0bits.U1RXIF = 0;
}
