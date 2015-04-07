/*
Aulas resolvidas por Rafael Ferreira e Rodrigo Cunha.
https://github.com/gipmon/ac2
*/
/* Configurar UART2 e fazer o seguinte, sem usar syscalls:
- Imprimir nome e nMec
- Caso seja inserido o caracter 'space', mandar o valor lido nos portos RB0 a RB3 como argumento a uma função para imprimir esse mesmo valor nos displays (essa função já estava implementada)
- Caso seja inserido outro caracter, imprimir "Carácter inválido".
*/
#include "detpic32.h"

int value;

void configUART(){
	U1BRG = ((20000000+8*115200)/(16*115200))-1;
	U1MODEbits.BRGH  = 0;
	U1MODEbits.PDSEL = 0;
	U1MODEbits.STSEL = 0;
	U1STAbits.UTXEN = 1;
	U1STAbits.URXEN = 1;
	U1MODEbits.ON = 1;

	/*
		NÃO SE DEVE FAZER UTX PORQUE NÃO VAMOS
		FAZER TRANSMITTER VAMOS SO FAZER RECEPTION
	*/
	// U1STAbits.UTXISEL = 0b00;
	U1STAbits.URXISEL = 0b00;
	IEC0bits.U1RXIE = 1;
	// IEC0bits.U1TXIE = 1;
	IEC0bits.U1EIE = 1;
	IPC6bits.U1IP = 4;
}

void putc(char byte2send){
	while(U1STAbits.UTXBF);
	U1TXREG = byte2send;
}

void puts(char *str){
	while(str[0]!='\0'){
		putc(str[0]);
		str++;
	}
}

int main(){
	configUART();
	EnableInterrupts();
	puts("Rafael Ferreira, nmec: 44444\n");

	/* configurar como entradas */
	TRISEbits.TRISE4 = 1;
	TRISEbits.TRISE5 = 1;
	TRISEbits.TRISE6 = 1;
	TRISEbits.TRISE7 = 1;

	/* configuração do timer */
	T2CONbits.TCKPS = 0;
	PR2 = 39000;
	TMR2 = 0;

	IFS0bits.T2IF = 0;
	IPC2bits.T2IP = 2;
	IEC0bits.T2IE = 1;

	while(1);
	return 0;
}

void send2displays(unsigned char value){
	static unsigned char displayFlag = 0;
	static const unsigned char codes[] = {0x77, 0x41, 0x3B, 0x6B, 0x4D, 0x6E, 0x7E, 0x43, 0x7F, 0x6F, 0x5F, 0x7C, 0x36, 0x79, 0x3E, 0x1E};

	TRISB = TRISB & 0xFC00;
	LATB = LATB & 0xFC00;

	unsigned char first = value & 0x0F;
	first = codes[first];
	unsigned char second = (value >> 4);
	second = codes[second];

	if (!displayFlag){
		LATBbits.LATB8 = 0;
		LATBbits.LATB9 = 1;
		LATB = (LATB & 0xFF00) | first;
	}else{
		LATBbits.LATB8 = 1;
		LATBbits.LATB9 = 0;
		LATB = (LATB & 0xFF00) | second;
	}

	displayFlag = !displayFlag;
}

void _int_(24) isr_uart1(void){
	if(U1RXREG==' '){
		value = (PORTE & 0xF0) >> 4;
		T2CONbits.TON = 1;
	}else{
		puts("Caracter inválido!\n");
	}
	IFS0bits.U1RXIF = 0;
}

void _int_(8) isr_T2(void){
	send2displays(value);
	IFS0bits.T2IF = 0;
}
