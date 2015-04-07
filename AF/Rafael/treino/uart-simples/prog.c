/*
Aulas resolvidas por Rafael Ferreira e Rodrigo Cunha.
https://github.com/gipmon/ac2
*/
/*
** UART simples **
Imprime “Futebol Clube do Porto" sem syscall’s:
Num contador de 4 bits, se introduzir '+' deve incrementar no contador, se introduzir '-' deve decrementar um valor.
Quando o contador atingir o valor máximo ou mínimo, 15 e 0, respectivamente, deve escrever no ecrã (sem syscalls) "MAX" ou "MIN" e não pode baixar para baixo de 0 ou aumentar para cima de 15.
*/

#include "detpic32.h"

#define baudrate 115200

int contador = 0;

void configUart(){
	U1BRG = ((20000000+8*baudrate)/(16*baudrate)) - 1;
	U1MODEbits.BRGH = 0;
	U1MODEbits.PDSEL = 0;
	U1MODEbits.STSEL = 0;

	U1STAbits.UTXEN = 1;
	U1STAbits.URXEN = 1;
	U1MODEbits.ON = 1;

	U1STAbits.URXISEL = 0b00;

	IEC0bits.U1RXIE = 1;

	IPC6bits.U1IP = 3;

	IEC0bits.U1EIE = 1;
	EnableInterrupts();
}

void putc(unsigned char byte2send){
	while(U1STAbits.UTXBF);
	U1TXREG = byte2send;
}

void puts(char *str){
	while(str[0]!='\0'){
		putc(str[0]);
		str++;
	}
}

int main(void){
	configUart();
	puts("Futebol Clube do Porto!\n");
	while(1);
}

void contadorAlert(){
	if(contador<=0){
		puts("Min\n");
		contador = 0;
	}else if(contador>=15){
		puts("Max\n");
		contador = 15;
	}
}

void _int_(24) isr_uart1(void){
	char tmp = U1RXREG;
	if(tmp=='+'){
		contador++;
		contadorAlert();
	}else if(tmp=='-'){
		contador--;
		contadorAlert();
	}
	IFS0bits.U1RXIF = 0;
}
