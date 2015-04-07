/*
Aulas resolvidas por Rafael Ferreira e Rodrigo Cunha.
https://github.com/gipmon/ac2
*/
#include <detpic32.h>

#define DisableUart1RxInterrupt()	IEC0bits.U1RXIE = 0
#define EnableUart1RxInterrupt()	IEC0bits.U1RXIE = 1
#define DisableUart1TxInterrupt()	IEC0bits.U1TXIE = 0
#define EnableUart1TxInterrupt()	IEC0bits.U1TXIE = 1

#define BUF_SIZE 8
#define INDEX_MASK (BUF_SIZE-1)

typedef struct{
    unsigned char data[BUF_SIZE];
    unsigned int head;
    unsigned int tail;
    unsigned int count;
} circularBuffer;

volatile circularBuffer txb;
volatile circularBuffer rxb;

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
	while(txb.count == BUF_SIZE);
	txb.data[txb.tail] = ch;
	txb.tail = (++txb.tail) & INDEX_MASK;
	DisableUart1TxInterrupt();
    txb.count++;
	EnableUart1TxInterrupt();
}

void comDrv_puts(char *s){
	for(; *s!='\0'; s++){
		comDrv_putc(*s);
	}
}

char comDrv_getc(char *pchar){
	if(rxb.count==0){
		return 0;
	}
	DisableUart1RxInterrupt();
	*pchar = rxb.data[rxb.head];
	rxb.count--;
	rxb.head = (++rxb.head) & INDEX_MASK;
	EnableUart1RxInterrupt();
	return 1;
}

void _int_(24) isr_uart1(void){
	if(IFS0bits.U1TXIF){
		if(txb.count>0){
			while(U1STAbits.UTXBF == 0 && txb.count != 0){
				U1TXREG = txb.data[txb.head];
				txb.head = (txb.head + 1) & INDEX_MASK;
				txb.count--;
			}
		}
		if(txb.count==0){
			DisableUart1TxInterrupt();
		}
		IFS0bits.U1TXIF = 0;
	}
	if(IFS0bits.U1RXIF){
		//data available && FIFO não está cheio
		while(U1STAbits.URXDA && rxb.count < BUF_SIZE){
			rxb.data[rxb.tail] = U1RXREG;
			rxb.tail = (rxb.tail + 1) & INDEX_MASK;
			if(rxb.count < BUF_SIZE){
				rxb.count++;
			}else{
				rxb.head++;
			}
			IFS0bits.U1RXIF = 0;
		}
	}
}

void comDrv_config(){
	U1BRG = ((20000000+8*115200)/(16*115200))-1;
	U1MODEbits.BRGH = 0;
	U1MODEbits.PDSEL = 0b00;
	U1MODEbits.STSEL = 0;
	U1STAbits.UTXEN = 1;
	U1STAbits.URXEN = 1;
	U1MODEbits.ON = 1;

	U1STAbits.URXISEL = 0b00;

	IEC0bits.U1RXIE = 1;
	IEC0bits.U1TXIE = 1;

	IPC6bits.U1IP = 3;
	IEC0bits.U1EIE = 1;
}

int main(void){
	comDrv_config();
	comDrv_flushRx();
	comDrv_flushTx();
	EnableInterrupts();
	comDrv_puts("PIC32 UART Device-driver\n");

	char tmp;

	while(1){
		if(comDrv_getc(&tmp)){
			comDrv_putc(tmp);
		}
	}
}
