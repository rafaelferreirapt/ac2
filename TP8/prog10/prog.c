#include <detpic32.h>

void delay(unsigned int);
void putc(unsigned char);
char getc(void);
void puts(unsigned char*);
void configUart(unsigned int, char, unsigned int);

int main(void){

	configUart(115200,'N',1);

	/*
		** UART configure interrupts **
		URXISEL<1:0> do registo UxSTA -> O modo como as interrupções são geradas;
	*/
	U1STAbits.URXISEL = 00;
	/*
		UxRXIE (receive interrupt enable)
		** se fosse para transmissão: **
		para a interrupção de transmissão o bit UxTXIE (transmit interrupt enable);
	*/
	IEC0bits.U1RXIE = 1;
	/*
		a definição da prioridade devem ser configurados os 3 bits UxIP (a configuração de prioridade é comum a todas as fontes de interrupção de uma UART)
	*/
	IPC6bits.U1IP=3;
	/*
		Cada UART pode ainda gerar uma interrupção quando é detetada uma situação de erro na receção de um caracter. Os erros detetados são de três tipos: erro de paridade, erro de framing
￼￼￼		- se pretender fazer a deteção destes erros por interrupção, então é
também necessário ativar essa fonte de interrupção, isto é, ativar o bit UxEIE
	*/
	IEC0bits.U1EIE = 1;

	EnableInterrupts();

	while(1);
}

void _int_(24) isr_uart1(void){
	putc(U1RXREG);
	/*
		U1RXIF = Receive Interrupt Flag Status Bit
	*/
	IFS0bits.U1RXIF = 0;
}

void configUart(unsigned int baud, char parity, unsigned int Stopbits){
	if(baud<600 || baud > 115200){
		baud = 115200;
	}
	if(parity!='N' && parity!='E' && parity!='O'){
		parity = 'N';
	}
	if(Stopbits!=1 && Stopbits!=2){
		Stopbits = 1;
	}

	U1BRG = ((20000000 + 8*baud)/ (16*baud))-1;
	U1MODEbits.BRGH = 0;

	/*
		PDSEL<1:0>: Parity and Data Selection bits
		11 = 9-bit data, no parity
		10 = 8-bit data, odd parity
		01 = 8-bit data, even parity
		00 = 8-bit data, no parity
	*/
	if(parity=='N'){
		U1MODEbits.PDSEL = 0;
	}else if(parity=='E'){
		U1MODEbits.PDSEL = 1;
	}else if(parity=='O'){
		U1MODEbits.PDSEL = 0b10;
	}

	/*
		STSEL: Stop Selection bit
		1 = 2 Stopbits
		0 = 1 Stopbit
	*/
	U1MODEbits.STSEL = (--Stopbits);

	/*
		UTXEN: Transmit Enable bit
		1 = UARTx transmitter is enabled. UxTX pin is controlled by UARTx (if ON = 1)
		0 = UARTxtransmitterisdisabled. Any pending transmission is aborted and buffer is reset. UxTX pin is
		controlled by port.
	*/
	U1STAbits.UTXEN = 1;

	/*
		URXEN: Receiver Enable bit
		1 = UARTx receiver is enabled. UxRX pin is controlled by UARTx (if ON = 1)
		0 = UARTx receiver is disabled. UxRX pin is ignored by the UARTx module. UxRX pin is controlled
		by port.
	*/
	U1STAbits.URXEN = 1;

	U1MODEbits.ON = 1;
}


void putc(unsigned char byte2send){
	/*
		UTXBF: Transmit Buffer Full Status bit (read-only)
		1 = Transmit buffer is full
		0 = Transmit buffer is not full, at least one more character can be written
	*/
	while(U1STAbits.UTXBF);

	//Transmit Buffer (UxTXREG)
	U1TXREG = byte2send;
}

void puts(unsigned char* string){
	int i = 0;
	while(string[i]!='\0'){
		putc(string[i++]);
	}
}
