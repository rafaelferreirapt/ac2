#include <detpic32.h>

void delay(unsigned int);
void putc(unsigned char);
char getc(void);
void puts(unsigned char*);
void configUart(unsigned int, char, unsigned int);

int main(void){

	configUart(115200,'N',1);

	while(1){
		putc(getc());
	}
}

char getc(void){
	/*
		OERR: Receive Buffer Overrun Error Status bit.
		This bit is set in hardware and can only be cleared (= 0) in software.
		Clearing a previously set OERR bit resets the receiver buffer and RSR to empty state.
		1 = Receive buffer has overflowed
		0 = Receive buffer has not overflowed
	*/
	if(U1STAbits.OERR == 1){
		U1STAbits.OERR = 0;
	}

	/*
		URXDA: Receive Buffer Data Available bit (read-only)
		1 = Receive buffer has data, at least one more character can be read
		0 = Receive buffer is empty
	*/
	while(U1STAbits.URXDA == 0);

	/*
		PERR: Parity Error Status bit (read-only)
		1 = Parity error has been detected for the current character
		0 = Parity error has not been detected
	*/
	/*
		FERR: Framing Error Status bit (read-only)
		1 = Framing error has been detected for the current character
		0 = Framing error has not been detected
	*/
	if(U1STAbits.PERR == 1 || U1STAbits.FERR == 1){
		char var = U1RXREG;
		return 0;
	}

	/*
		Receive Buffer (UxRXREG)
		The UART receiver has a 9-bit-wide FIFO receive data buffer that is up to 8 levels deep.
		The UxRXREG is a memory mapped register that provides access to the output of the FIFO.
		It is possible for the FIFO to be full and the next word to begin shifting to the UxRSR register before a buffer overrun occurs.
	*/
	return U1RXREG;
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
