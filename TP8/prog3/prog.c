/*
Aulas resolvidas por Rafael Ferreira e Rodrigo Cunha.
https://github.com/gipmon/ac2
*/
#include <detpic32.h>

void delay(unsigned int);
void putc(unsigned char);
void puts(unsigned char*);

int main(void){
	//configuration UART1

	U1BRG = ((20000000 + 8*115200)/ (16*115200))-1;
	U1MODEbits.BRGH = 0;

	U1MODEbits.PDSEL = 00;

	/*
	PDSEL<1:0>: Parity and Data Selection bits
	11 = 9-bit data, no parity
	10 = 8-bit data, odd parity
	01 = 8-bit data, even parity
	00 = 8-bit data, no parity
	*/

	U1MODEbits.STSEL = 0;

	/*
	STSEL: Stop Selection bit
	1 = 2 Stopbits
	0 = 1 Stopbit
	*/

	U1STAbits.UTXEN = 1;
	U1STAbits.URXEN = 1;

	U1MODEbits.ON = 1;

	while(1){
		puts("String de teste\n");
		delay(1000);
	}
}

void putc(unsigned char byte2send){
	while(U1STAbits.UTXBF);

	U1TXREG = byte2send;
}

void puts(unsigned char* string){
	int i = 0;
	while(string[i]!='\0'){
		putc(string[i++]);
	}
}
