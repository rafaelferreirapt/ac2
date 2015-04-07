/*
Aulas resolvidas por Rafael Ferreira e Rodrigo Cunha.
https://github.com/gipmon/ac2
*/
#include <detpic32.h>

#define DisableUart1RxInterrupt() IEC0bits.U1RXIE = 0
#define EnableUart1RxInterrupt() IEC0bits.U1RXIE = 1
#define DisableUart1TxInterrupt() IEC0bits.U1TXIE = 0
#define EnableUart1TxInterrupt() IEC0bits.U1TXIE = 1
#define BUF_SIZE 8
#define INDEX_MASK   (BUF_SIZE - 1)
#define VECTOR_UART1 24

typedef struct{
    unsigned char data[BUF_SIZE];
    unsigned int head;
    unsigned int tail;
    unsigned int count;
} circularBuffer;

volatile circularBuffer txb; // Transmission buffer
volatile circularBuffer rxb; // Reception buffer

void comDrv_flushRx(void){
	rxb.head = 0;
	rxb.tail = 0;
	rxb.count = 0;

	int i;

	for(i = 0; i<BUF_SIZE; i++){
		rxb.data[i] = 0;
	}
}


void comDrv_flushTx(void){
	txb.head = 0;
	txb.tail = 0;
	txb.count = 0;

	int i;

	for(i = 0; i<BUF_SIZE; i++){
		txb.data[i] = 0;
	}
}

void comDrv_putc(char ch){
	// Wait while buffer is full
	while(txb.count == BUF_SIZE);

	// buffer at position "tail"
	// Copy character to the transmission
	txb.data[txb.tail] = ch;

	// Increment "tail" index
	// (mod. BUF_SIZE)
	txb.tail = (++txb.tail) & INDEX_MASK;

	// Begin of critical section
	DisableUart1TxInterrupt();

    // Increment "count" variable
    txb.count++;

    // End of critical section
	EnableUart1TxInterrupt();
}

void comDrv_puts(char *s){
	for(; *s!='\0'; s++){
		comDrv_putc(*s);
	}
}

/*
VECTOR_UART1 = 24
rotina de serviço à interrupção de
transmissão da UART1
*/
void _int_(VECTOR_UART1) isr_uart1(void){
	if(IFS0bits.U1TXIF){
		if(txb.count>0){
			U1TXREG = txb.data[txb.head];
			txb.head = (txb.head + 1) & INDEX_MASK;
			txb.count--;
		}
		if(txb.count==0){
			DisableUart1TxInterrupt();
		}
		IFS0bits.U1TXIF = 0;
	}
	if(IFS0bits.U1RXIF){
		rxb.data[rxb.tail] = U1RXREG; // Read character from UART and
		// write it to the "tail" position
	    //  of the reception buffer

	    // Increment "tail" variable (mod BUF_SIZE)
	    rxb.tail = (rxb.tail + 1) & INDEX_MASK;

		// If reception buffer is not full (e.g. count < BUF_SIZE) then increment "count" variable
		if(rxb.count < BUF_SIZE){
			rxb.count++;
		}else{
			// increment "head" variable (discard oldest character)
			rxb.head++;
		}

        // reset UART1 RX interrupt flag
        IFS0bits.U1RXIF = 0;
	}
}

void comDrv_config(unsigned int baud, char parity, unsigned int Stopbits){
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

	DisableUart1TxInterrupt();
	DisableUart1RxInterrupt();

	/*
		a definição da prioridade devem ser configurados os 3 bits UxIP (a configuração de prioridade é comum a todas as fontes de interrupção de uma UART)
	*/
	IPC6bits.U1IP = 3;

	/*
		** UART configure interrupts **
		URXISEL<1:0> do registo UxSTA -> O modo como as interrupções são geradas;
	*/
	U1STAbits.URXISEL = 00;

	/*
		** UART configure interrupts **
		UrXISEL<1:0> do registo UxSTA -> O modo como as interrupções são geradas;
	*/
	U1STAbits.URXISEL = 00;
}

char comDrv_getc(char *pchar){
	// Test "count" variable (reception buffer) and return FALSE  || if it is zero
	if(rxb.count == 0){
		return 0;
	}

	DisableUart1RxInterrupt();

	// Begin of critical section
	// Copy character pointed by "head" to *pchar
	*pchar = rxb.data[rxb.head];

	// Decrement "count" variable
	rxb.count--;

	// Increment "head" variable (mod BUF_SIZE)

	rxb.head = (rxb.head + 1) & INDEX_MASK;

	EnableUart1RxInterrupt();
	// End of critical section

	return 1;
}

int main(void){
	comDrv_config(115200, 'N', 1); //default "pterm" parameters
	//with TX and RX interrupts disabled

	comDrv_flushRx();
	comDrv_flushTx();
	EnableInterrupts();
	while(1){
		comDrv_puts("Teste do bloco de transmissao do device driver ");
	}

}
