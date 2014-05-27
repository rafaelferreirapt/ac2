#include <detpic32.h>


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

void configUart(){
	//115200, 'N', 1
	U1BRG = ((20000000+8*115200)/(16*115200))-1;
	U1MODEbits.BRGH = 0;
	U1MODEbits.PDSEL = 0b00;
	U1MODEbits.STSEL = 0;
	U1STAbits.UTXEN = 1;
	U1STAbits.URXEN = 1;
	U1MODEbits.ON = 1;
}

char getc(void){
	if(U1STAbits.OERR){
		U1STAbits.OERR = 0;
	}
	while(U1STAbits.URXDA==0);
	if(U1STAbits.FERR || U1STAbits.PERR){
		char tmp = U1RXREG;
		return 0;
	}
	return U1RXREG;
}
int main(void){
	configUart();
	while(1){
		putc(getc());
	}
	return 0;
}
